#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

int main(){

   //getting the user's command line input
   string userInput;
   char buffer [100];

   printf("Input command line: ");
   fgets(buffer, 100, stdin);
   buffer[strcspn(buffer, "\n")] = 0;

   printf("User Input: %s", buffer);
   printf("hello\n");

   system(buffer);
   
   return 0;
} 