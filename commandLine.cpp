#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <regex>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

 char *inputArr[20];
 int idx;
 int waitFlag;

int parser(char aString[]){

    char *pipe = strstr(aString, "|");
    char *io1= strstr(aString, ">");
    char *io2= strstr(aString, "<");
    char *dollar= strstr(aString, "$");
    char *wait = strstr(aString, "&");

    //0 = wait, 1= dont true
    waitFlag =0;
    if(wait){ //don't wait
        waitFlag = 1;
    }

    printf("WAIT FLAG: %d\n" ,waitFlag);

    //STRING PARSING WORK : putting input(buffer) into inputArr
    char *token;
    token= strtok(aString, " ");
    
    idx=0; //also serves as the length of the array
    while(token!= NULL && strcmp(token, "&") != 0){
        inputArr[idx] = token;
        idx++;
        token= strtok(NULL, " ");
    }
    inputArr[idx] = NULL;

    if(waitFlag == 1){
        return 4;
    }

    else if(pipe || io1 || io2){ //case 1 & 2
        return 2;
    }

    else if(dollar){ //case 3
        return 3;
    }

    return 1; //case 1
}

int main(){

    printf("\n            T     R     I      A                 \n               E     M      N     L              \n") ;

    while(1){

        /* getting the user's command line input from stdin & storing it into buffer */
        char buffer [50];
        printf("\nInput command line: ");
        fgets(buffer, 50, stdin);
        buffer[strcspn(buffer, "\n")] = 0; //removes the \n character from buffer 

        printf("\n");
        //system(buffer);
        char bufferCopy[50];
        strcpy(bufferCopy, buffer);

        int cases= parser(buffer);
        
        /* -- CASE 1: Execute a single cmd line w/ upto one arg -- */
        /* -- CASE 2: Execute two commands connected w/ a pipe & redirection -- */

        if(cases == 4){ //if there is an & in the command

            string bufferCopy2= bufferCopy;
            string toReplace= "&";
            size_t pos;

            printf("CASE 5\n");

            //bufferCopy2= regex_replace(bufferCopy2, regex("&"), "");

            while( (pos = bufferCopy2.find(toReplace)) != std::string::npos){
                bufferCopy2.replace(pos, toReplace.length(), "");
            }

            pid_t pid= fork();

            if(pid ==0){ //child
                system(bufferCopy2.c_str());

            }

            else{ //parent
                //if there is an instance of "&", don't wait
            }
        }

        else if(cases== 1 || cases == 2){
            //printf("CASE 1/2\n");

            int x= system(bufferCopy);

            if (x != 0){
                printf("[ERROR DETECTED]: re-type command below ↓\n"); 
            }
        }

        /* -- CASE 3: Special Pipes -- */

        // "cmd1 cmd2 $ cmd3 cmd4"
        else if(cases == 3){

            //printing each element of inputArr and finding index of "$"
            int dollar;
            for(int i=0; i< idx; i++){
                //printf("%d:%s\n",i, inputArr[i]);
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

            //read from pipe fd[0] and write contents into file
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

            //process everything after $ and make the file from above the stdin
            for(int i=dollar+1; i<idx; i++){
                char *args[]= {inputArr[i], NULL};

                pid_t pid = fork();
                
                if(pid == 0){
                    int newstdin = open("output.txt", O_RDONLY);

                    close(0); //close stdin and replace it with fd of output.txt
                    dup(newstdin);
                    close(newstdin);

                    execvp(inputArr[i], args);
                }

                else{
                    wait(NULL);
                }
            }
        }
    }

   return 0;
} 