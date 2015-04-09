#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define MAXRCVLEN 500

int main(void){
	char buf[MAXRCVLEN + 1];				//buf, msg save information
	char msg[MAXRCVLEN + 1];
	char file[MAXRCVLEN + 1];
	struct sockaddr_in client;
	struct sockaddr_in server;
	int mysocket;
	socklen_t length = sizeof(struct sockaddr_in);
	FILE *fp;									//file pointer
	fd_set fd, fds;							//select set
	struct timeval timeout = {0, 0};		//select time

	memset(&server, 0, sizeof(server));	//set socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(2343);

	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	if( bind(mysocket, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1 ){	//bind
		perror("bind");
		exit(1);
	}

	if( listen(mysocket, 20) == -1 ){				//listen
		perror("listen");
		exit(1);
	}

	int consocket = accept(mysocket, (struct sockaddr*)&client, &length);	//accept
	if( consocket == -1){
		perror("accept error");
		exit(1);
	}

   recv(consocket, buf, 501, 0);					//read message
	printf("%s come in.\n", inet_ntoa(client.sin_addr));
	printf("He say: %s\n", buf);

	while(1){
		int iRet = 0;									//select set
		FD_ZERO(&fd);
      FD_ZERO(&fds);
      FD_SET(0, &fd);
      FD_SET(consocket, &fd);
      memcpy(&fds, &fd, sizeof(fds));
      iRet = select(consocket + 1, &fds, NULL, NULL, &timeout);

		if(FD_ISSET(0, &fds)){						//select
      	gets(msg);
			if(strcmp(msg, "SEND") == 0){			//send file
				send(consocket, msg, 501, 0);
				printf("File`s name:");
				gets(msg);

				fp = fopen(msg, "rb");				//open file
				if(fp == NULL){
					printf("Can not find file\n");
				}
				else{
					char out[] = "Server Send a file to you,";
					strcat(out, msg);
					send(consocket, out, 501, 0); 

					send(consocket, msg, 501, 0);
					fseek(fp, 0, SEEK_END);			//count how many char
					int size = ftell(fp);
					fseek(fp, 0, SEEK_SET);
					sprintf(msg, "%d", size);
					send(consocket, msg, 501, 0);

					int count = 0;
					while(count < size){				//send message
						for(int i = 0; i < 501; i++, count++){
							if(count >= size)
								break;
							fscanf(fp, "%c", &msg[i]);
						}
						send(consocket, msg, 501, 0);
					}

					fclose(fp);
					printf("Success!!!\n");
				}
			}
			else{
				send(consocket, msg, 501, 0);		//send message
				printf("You say: %s\n", msg);
			}
		}
		else if(FD_ISSET(consocket, &fds)){
			recv(consocket, buf, 501, 0);
         if(strcmp(buf, "SEND") == 0){			//get file
            recv(consocket, buf, 501, 0);
            printf("%s\n", buf);

            recv(consocket, buf, 501, 0);
            fp = fopen("out.txt", "wb");

            recv(consocket, buf, 501, 0);
            int size = atoi(buf);
            int count = 0;

            while(count < size){				//get file
               recv(consocket, buf, 501, 0);
               count = count + strlen(buf);
               fprintf(fp, "%s", buf);
            }
            fclose(fp);
            printf("Success!!!\n");
         }
         else{
            printf("He say: %s\n", buf);	//get message
         }
		}

		if(strcmp(buf, "ESC") == 0){
			printf("client shut down!\n");
			break;
		}

		if(strcmp(msg, "ESC") == 0)
			break;
	}

	close(consocket);
	close(mysocket);

	return 0;
}
