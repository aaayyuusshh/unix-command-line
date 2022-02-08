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

    /* -- CASE 3: Special Pipes -- */

    // "cmd1 cmd2 $ cmd3 cmd4"
    else if(cases == 3){

        //STRING PARSING WORK : putting input(buffer) into inputArr
        char *token;
        token= strtok(buffer, " ");
        char *inputArr[20];
        int idx=0; //also servers as the length of the array
        while(token!= NULL){
            inputArr[idx] = token;
            idx++;
            token= strtok(NULL, " ");
        }

        //printing each element of inputArr and finding index of "$"
        int dollar;
        for(int i=0; i< idx; i++){
            printf("%d:%s\n",i, inputArr[i]);
            if(strncmp(inputArr[i],"$", 1) == 0){
                dollar = i;
            }    
        }

        // ls $ wc wc
        int fd[2];
        pipe(fd);

        //process everything before the "$" and add the cmds output into fd[1]
        for(int i=0; i<dollar; i++){ 

            char *args[] = {inputArr[i], NULL};
            pid_t pid= fork();

            if(pid == 0){ //child --> writes stdout(1) to the write end of the pip (fd[1])
                close(fd[0]);
                dup2(fd[1], 1); //put whats in stdout into fd[1]
                close(fd[1]);
                execvp(inputArr[i], args);
            }

            else{
                //wait(NULL);
            }
        }
    }

   return 0;
} 