#include <stdio.h>
#include <string.h>

int main(void){
   char msg[50];
   char temp[50];
   char text[100];
   int num = 0;
   memset(msg, 0, sizeof(msg));
   memset(temp, 0, sizeof(temp));
   memset(text, 0, sizeof(text));

   printf("File`s name:");
   scanf("%s", msg);

   strcat(temp, "/proc/");
   strcat(temp, msg);

   FILE *fp = fopen(temp, "r");
   while(!feof(fp)){
      fscanf(fp, "%s %d\n", text, &num);
      printf("%s %d\n", text, num);
   }

   return 0;
}
