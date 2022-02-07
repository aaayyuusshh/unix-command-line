#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

int main(){

   /* getting the user's command line input from stdin & storing it into buffer */
   char buffer [50];
   printf("Input command line: ");
   fgets(buffer, 50, stdin);
   buffer[strcspn(buffer, "\n")] = 0; //removes the \n character from buffer 

   printf("Buffer: %s\n", buffer);
   printf("\n");
   //system(buffer);

   /* --CASE 1: execute a single cmd line w/ upto one arg-- */

   char *token;
   token = strtok(buffer, " "); //getting every individual word from user input split by delimeter " "
   int idx= 0;
   char *args[3];  //char *args [] = {"ls", "-l", NULL};

   while(token != NULL){
       printf("Token %d:%s\n", idx, token);
       args[idx] = token;  //populating args with individual words from user input split by " "
       idx++;
       token = strtok(NULL, " ");   
   }
   args[2] = NULL;
   printf("\n");
 
   //if user input = "ls -l" then buffer = "ls -l" then args = ["ls", "-l", NULL] 
   execvp(args[0], args);  

   return 0;
} 