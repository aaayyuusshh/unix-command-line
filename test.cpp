#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

int main(){

    // FILE *file = fopen("error.txt", "w");
    // int  newstdout = open("error.txt" ,O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO); 
    // close(1); 
    // dup(newstdout); 
    // close(newstdout);



    if(system("ls |") != 0){

            printf("ERROR");
    }




}