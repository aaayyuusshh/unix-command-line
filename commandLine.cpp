#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

int parser(char aString[]){

    char *pipe = strstr(aString, "|");
    char *io1= strstr(aString, ">");
    char *io2= strstr(aString, "<");
    char *dollar= strstr(aString, "$");

    if(pipe || io1 || io2){ //case 2
        return 2;
    }
    else if(dollar){ //case 3
        return 3;
    }
    
    return 1; //case 1
}

int main(){

   /* getting the user's command line input from stdin & storing it into buffer */
   char buffer [50];
   printf("Input command line: ");
   fgets(buffer, 50, stdin);
   buffer[strcspn(buffer, "\n")] = 0; //removes the \n character from buffer 

   printf("Buffer: %s\n", buffer);
   printf("\n");
   //system(buffer);

    int cases= parser(buffer);

    /* -- CASE 1: execute a single cmd line w/ upto one arg -- */
    if(cases == 1){
        printf("CASE 1\n");

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
    }  

    /* -- CASE 2: Execute two commands connected w/ a pipe & redirection -- */
    else if(cases == 2){
        printf("CASE 2\n");

    }

   return 0;
} 