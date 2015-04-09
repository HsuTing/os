#include <stdio.h>
#include <stdlib.h>
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
	char msg[MAXRCVLEN + 1];	//msg, buf store information
	char buf[MAXRCVLEN + 1];
	int mysocket;
	struct sockaddr_in server;
	FILE *fp;						//file pointer
	char ip[15];
	fd_set fd, fds;				//select set
	struct timeval timeout = {0, 0};	//select time

	printf("Input IP:");				//input IP number
	gets(ip);

	mysocket = socket(AF_INET, SOCK_STREAM, 0);	//set socket

	memset(&server, 0, sizeof(server));				//set socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(2343);

	int consocket = connect(mysocket, (struct sockaddr*)&server, sizeof(struct sockaddr));	// connect
	if(consocket == -1){
		perror("connect");
		exit(1);
	}

	printf("You say:");								//send message
	gets(msg);
   send(mysocket, msg, 501, 0);

	while(1){
		int iRet = 0;									//set select
		FD_ZERO(&fd);
		FD_ZERO(&fds);
		FD_SET(0, &fd);
	   FD_SET(mysocket, &fd);
		memcpy(&fds, &fd, sizeof(fds));
      iRet = select(mysocket + 1, &fds, NULL, NULL, &timeout);

      if(FD_ISSET(0, &fds)){						//select
      	gets(msg);
         if(strcmp(msg, "SEND") == 0){			//send file
            send(mysocket, msg, 501, 0);
            printf("File`s name:");
            gets(msg);								//choose file

            fp = fopen(msg, "rb");
            if(fp == NULL){						//file is not here
               printf("Can not find file\n");
            }
            else{
               char out[] = "Server Send a file to you,";
               strcat(out, msg);
               send(mysocket, out, 501, 0);

               send(mysocket, msg, 501, 0);
               fseek(fp, 0, SEEK_END);		//count file how many char
               int size = ftell(fp);
               fseek(fp, 0, SEEK_SET);
               sprintf(msg, "%d", size);
               send(mysocket, msg, 501, 0);

               int count = 0;
               while(count < size){			//send file
                  for(int i = 0; i < 501; i++, count++){
                     if(count >= size)
                        break;
                     fscanf(fp, "%c", &msg[i]);
                  }
                  send(mysocket, msg, 501, 0);
               }

               fclose(fp);
               printf("Success!!!\n");
            }
         }
         else{
            send(mysocket, msg, 501, 0);	//send message
            printf("You say: %s\n", msg);
         }
      }
      else if(FD_ISSET(mysocket, &fds)){
         recv(mysocket, buf, 501, 0);
			if(strcmp(buf, "SEND") == 0){		//get file
				recv(mysocket, buf, 501, 0);
				printf("%s\n", buf);

				recv(mysocket, buf, 501, 0);
				fp = fopen("out.txt", "wb");

				recv(mysocket, buf, 501, 0);
				int size = atoi(buf);
				int count = 0;

				while(count < size){				//get file
					recv(mysocket, buf, 501, 0);
					count = count + strlen(buf);
					fprintf(fp, "%s", buf);
				}
				fclose(fp);
				printf("Success!!!\n");
			}
			else{
         	printf("He say: %s\n", buf);		//read message
			}
    	}

      if(strcmp(buf, "ESC") == 0){
         printf("server shut down!\n");
         break;
      }
		if(strcmp(msg, "ESC") == 0)
			break;
	}

	close(mysocket);
	return 0;
}
