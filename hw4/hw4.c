#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int spawn(char *prog, char**arg_list);

int main(int argc, char *argv[]) {
	char *cmd, *temp;
	char line[MAX_LENGTH], temp_line[MAX_LENGTH], path[MAX_LENGTH], temp_path[MAX_LENGTH], name[MAX_LENGTH], ch;
	struct stat buf;
	FILE *fp;
	int i = 0, control_1 = 0, control_2 = 0, control_3 = 0, check = 0, count_i = 0, count_end = 0;
	clock_t start, end;

	while(1) {
		start = clock();
		getcwd(path, MAX_LENGTH);
		if(control_1 == 0) {
			memset(line, 0, sizeof(line));
			memset(line, 0, sizeof(temp_line));
			if(control_3 == 0) {
				printf("%s$$ ", path);
   				fgets(line, MAX_LENGTH, stdin);
			}
			else {
				fp = fopen(name, "r");
				int i = 0;
				for(; i < count_i; i++) {
					fgets(line, MAX_LENGTH, fp);
				}
				count_i++;
				fclose(fp);
				if(count_i == count_end + 1) {
					control_3 = 0;
				}
			}
			strcpy(temp_line, line);
		}
		else {
			control_2 = 1;
		}
		strcpy(temp_path, path);
		strcat(temp_path, "/");
		if(strstr(line, ">>") != NULL) {
			check = 1;
		}
		if(strstr(line, "#") != NULL) {
			continue;
		}

		if((cmd = strtok(line, DELIMS))) {
			if(strcmp(cmd, "ps") == 0) {
				DIR *dir;
				struct dirent *file;

				printf(" PID 	  CMD\n");
				dir = opendir("/proc");
				while((file = readdir(dir)) != NULL) {
					if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "net") == 0 || strcmp(file->d_name, "self") == 0) {
						continue;
					}
					strcpy(temp_path, "/proc/");
					strcat(temp_path, file->d_name);
					if(stat(temp_path, &buf) == 0) {
						if(buf.st_mode & S_IFDIR) {
							strcat(temp_path, "/stat");
							fp = fopen(temp_path, "r");
							if(fp == NULL) {
								continue;
							}
							else {
								int num = 1;
								char text[MAX_LENGTH];
								fscanf(fp, "%d %s", &num, text);
								printf("%-6d %s\n", num, text);
							}
							fclose(fp);
						}
					}
				}
				closedir(dir);
			}
			else if(strcmp(cmd, "ls") == 0) {
				temp = strtok(NULL, DELIMS);
				if(temp == NULL) {
					DIR *dir;
					struct dirent *file;

					dir = opendir(path);
					while((file = readdir(dir)) != NULL) {
						if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0) {
							continue;
						}
						printf("%s  ", file->d_name);
					}
					closedir(dir);
					printf("\n");
				}
				else {
					do {
						strcpy(temp_path, path);
						strcat(temp_path, "/");
						strcat(temp_path, temp);
						if(stat(temp_path, &buf) == 0) {
							if(buf.st_mode & S_IFDIR) {
								DIR *dir;
								struct dirent *file;

								printf("%s:\n", temp);
								dir = opendir(temp);
								while((file = readdir(dir)) != NULL) {
									if(strcmp(file->d_name, "..") == 0 || strcmp(file->d_name, ".") == 0) {
										continue;
									}
									printf("%s  ", file->d_name);
								}
								closedir(dir);
								printf("\n");
							}
							else if(buf.st_mode & S_IFREG) {
								printf("%s\n", temp);
							}
						}
						else {
							printf("ls: cannot access %s: No such file or directory\n", temp);
						}
						if((temp = strtok(NULL, DELIMS)) != NULL) {
							printf("\n");
						}
					}while(temp != NULL);
				}
			}
			else if(strcmp(cmd, "cat") == 0) {
				while((temp = strtok(NULL, DELIMS)) != NULL) {
					strcpy(temp_path, path);
					strcat(temp_path, "/");
  					strcat(temp_path, temp);
					if(stat(temp_path, &buf) == 0) {
						if(buf.st_mode & S_IFDIR) {
							printf("cat: %s: Is a directory\n", temp);
						}
						else if(buf.st_mode & S_IFREG) {
							fp = fopen(temp, "r");
							while((fscanf(fp, "%c", &ch) != EOF)) {
								printf("%c", ch);
							}
							printf("\n");
							fclose(fp);
						}
					}
					else {
						printf("cat: %s: No such file or directory\n", temp);
					}
				}
			}
			else if(strcmp(cmd, "pwd") == 0) {
				printf("%s\n", path);
			}
			else if(strcmp(cmd, "echo") == 0) {
				if(check == 0) {
					while((temp = strtok(NULL, DELIMS)) != NULL) {
						printf("%s ", temp);
					}
					printf("\n");
				}
				else {
					char *loc = strstr(temp_line, ">>");
					loc = loc + 3;
					int i = loc - temp_line, j = strlen(temp_line) - 1 - i;
					char file_name[j];
					memset(file_name, 0, sizeof(file_name));
					for(j = 0;i < strlen(temp_line) - 1; i++, j++) {
						file_name[j] = temp_line[i];
					}
					file_name[j] = '\0';
					
					fp = fopen(file_name, "w");
					temp = strtok(NULL, DELIMS);
					while(strcmp(temp , ">>") != 0) {
						fprintf(fp, "%s ", temp);
						temp = strtok(NULL, DELIMS);
					}
					fclose(fp);
				}
			}
			else if(strcmp(cmd, "date") == 0) {
				time_t now = time(0);
				char timebuf[255];
				strftime(timebuf, 255, "%G/%m/%d %R", localtime(&now));
				printf("%s\n", timebuf);				
			}
			else if(strcmp(cmd, "time") == 0) {
				if((temp = strtok(NULL, DELIMS)) != NULL) {
					control_1 = 1;
					memset(temp_line, 0, sizeof(line));
					do {
						strcat(temp_line, temp);
						strcat(temp_line, " ");
					}while((temp = strtok(NULL, DELIMS)) != NULL);
					memset(line, 0, sizeof(line));
					strcpy(line, temp_line);
				}
			}
			else if(strcmp(cmd, "du") == 0) {
				while((temp = strtok(NULL, DELIMS)) != NULL) {
					strcpy(temp_path, path);
					strcat(temp_path, "/");
					strcat(temp_path, temp);
					if(stat(temp_path, &buf) == 0) {
						printf("%d %s\n", (int)buf.st_size, temp);
					}
					else {
						printf("du: cannot access ‘%s’: No such file or directory\n", temp);
					}
				}
			}
			else if(strcmp(cmd, "mkdir") == 0) {
				while((temp = strtok(NULL, DELIMS)) != NULL) {
					strcpy(temp_path, path);
					strcat(temp_path, "/");
					strcat(temp_path, temp);
					if(stat(temp_path, &buf) == 0) {
						printf("mkdir: cannot create directory ‘%s’: File exists\n", temp);
					}
					else {
						mkdir(temp, 0755);
					}
				}
			}
			else if(strcmp(cmd, "cd") == 0) {
				temp = strtok(NULL, DELIMS);
				strcat(temp_path, temp);
				if(stat(temp_path, &buf) == 0) {
					if(buf.st_mode & S_IFDIR) {
						strcat(path, "/");
						strcat(path, temp);
						chdir(path);
					}
					else if(buf.st_mode & S_IFREG) {
						printf("cd: %s: Not a directory\n", temp);
					}
				}
				else {
					printf("cd: %s: No such file or directory\n", temp);
				}
			}
			else if(strcmp(cmd, "hw4") == 0) {
				control_3 = 1;
				count_i = 0;
				count_end = 0;
				temp = strtok(NULL, DELIMS);
				fp = fopen(temp, "r");
				while((fscanf(fp, "%c", &ch) != EOF)) {
					if(ch == '\n') {
						count_end++;
					}
				}
				memset(name, 0, sizeof(name));
				strcat(name, temp);
				fclose(fp);
			}
			else if(strcmp(cmd, "tar") == 0) {
				temp = strtok(NULL, DELIMS);
				char text[MAX_LENGTH];
				memset(text, 0, sizeof(text));
				strcpy(text, temp);
				strcat(text, ".tgz");
				char *arg_list[] = {"tar", "-czvf", text, temp, NULL};
				spawn("tar", arg_list);
			}
			else if(strcmp(cmd, "df") == 0) {
				char *arg_list[] = {"df", NULL};
				spawn("df", arg_list);
			}
			else if(strcmp(cmd, "exit") == 0) {
				break;
      		}
			else {
				printf("%s: command not found\n", cmd);
			}

			if(control_2 == 1) {
				end = clock();
				printf("%lf s\n", (end-start)/(double)(CLOCKS_PER_SEC));
				control_1 = 0;
				control_2 = 0;
			}
		}
	}

	return 0;
}

int spawn(char *prog, char**arg_list) {
	pid_t child;
	child = fork();

	if(child != 0) {
		return child;
	}
	else {
		execvp(prog, arg_list);
		fprintf(stderr, "spawn erroer\n");
		return -1;
	}
}
