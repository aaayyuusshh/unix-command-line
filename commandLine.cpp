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

    while(1){

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
        /* -- CASE 2: Execute two commands connected w/ a pipe & redirection -- */

        if(cases == 1 || cases == 2){
            printf("CASE 2\n");

            system(buffer);
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

            //    0     1     2    3     4
            // [ "ls" "pwd"  "$" "wc" "cat" ]

            int fd[2];
            pipe(fd);
    
            //process everything before the "$" and add the cmds output into fd[1]
            for(int i =0; i< dollar; i++){

                char *args[]= {inputArr[i], NULL};

                pid_t pid = fork(); //both ends of pipe are open
                if(pid == 0){ //child --> used to write into pipe
                    close(fd[0]);
                    dup2(fd[1],1);
                    close(fd[1]);
                    execvp(inputArr[i], args); 
                }

                else{
                    wait(NULL);
                }
            }

            //read from pipe fd[0] and write contents into file output.txt
            close(fd[1]);
            FILE *readFile = fdopen(fd[0], "r"); //open the read end of the fd
            FILE *temp = fopen("output.txt", "w");
            char toRead= fgetc(readFile);

            while(toRead != EOF){
                fprintf(temp, "%c", toRead);
                toRead = fgetc(readFile);
            }
            fclose(readFile);
            fclose(temp);

        }
    }

    

   return 0;
} 