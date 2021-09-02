#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1000
#define FORKERR "\033[0;31m"
#define RESET "\e[0m"
#define DELIMITER " \t\r\n"
#define NAME "MTL458:"
#define HISTORY_LEN 5

char *get_input();
char **get_args(char *);
int arg_run(char **);
char history[HISTORY_LEN][1024];
int last_inx = 0;  
int infLoop = 0;
char home_dir[BUFFER_SIZE]; 
char curr_dir[BUFFER_SIZE]; 
int cdValid = 1;
int cdHasQuot = 0;
  
int main(int argc, char *argv[]) {
   // printf("Hello, World!");
   char *line;
   char **args;
   // chdir("/mnt/c/Users/hetvi/Documents/want space name");
   if(getcwd(home_dir,sizeof(home_dir))==NULL){
      printf("home null!!!!! bad!!! exit");
      exit(EXIT_FAILURE);
   } else{
      // printf(home_dir);
         printf("Welcome to terminal for assignment 1\n");
   }
   if(getcwd(curr_dir,sizeof(curr_dir))==NULL){
         printf("cwd null!!!!! bad! exit");
         exit(EXIT_FAILURE);
   }


   while(infLoop>=0){
      printf(NAME);
      // else{
         // printf("cwd");
         if(strcmp(home_dir,curr_dir)==0){
            printf("~");
         } else{
            // printf("Length of given string is: %lu\n", strlen(home_dir));
            // printf("Length of given string is: %lu\n", strlen(curr_dir));   
            int tru = 0; 
            for(int i = 0; i<strlen(home_dir);i++){
               if(home_dir[i] && curr_dir[i] && home_dir[i]==curr_dir[i]){
                  if(tru>=0) tru=2;
               } else{
                  tru=-1;
               }
            }

            if(tru<0) printf(curr_dir); 
            else{ 
               // printf('~');
               printf("%c", '~');
               for(int i = strlen(home_dir); i<strlen(curr_dir); i++){
                  // printf(curr_dir[i]);
                  printf("%c", curr_dir[i]);
               }
            }
         }
         printf("$ ");

      line = get_input();

      if (line[0] != '\0'){
         if(strcmp(history[last_inx],line)!=0){
            last_inx=infLoop%HISTORY_LEN;
            strcpy(history[last_inx], line); 
            infLoop+=1;
         }
         args = get_args(line);
         arg_run(args);
         free(line);
         free(args);
      } 
      else{
         free(line);          
      }
   }
   return 0;
}

char *get_input(){
  int buffer_size = BUFFER_SIZE+2; //+2 just incase
  char *buffer = malloc(sizeof(char) * buffer_size);
  if (buffer==NULL) {
   printf("malloc returned null, exit proc \n");
   exit(EXIT_FAILURE);
  }

  int indx, char_v; indx=0;
   int isCD=0; int inQuot=0; int numSpac = 0;
  while (indx>=0){

    char_v = getchar();
     if(indx==0 && char_v=='c'){
        isCD++;
     } if(indx==1 && char_v=='d'){
        isCD++;
     } 
     if(isCD==2 && indx>2){
        if(char_v=='\'' || char_v=='\"'){
           inQuot++;
           inQuot=inQuot%2;
           cdHasQuot+=inQuot;
        } 
        if(inQuot!=1 && char_v==' '){
            numSpac++;
        }  
     } 

    if (char_v == '\n' || char_v == EOF){
       //terminate reading operation
      //  printf("in eof");
      int numCop = numSpac;
      for(int i = 0; i <numCop; i++){
         if(buffer[indx-1-i]==' '){
            numSpac--;
         }
      }
      buffer[indx] = '\0';
      break;
   } else buffer[indx] = char_v;

    if (indx>=buffer_size) {
       printf("buffer limit exceeded, exit proc \n");
       exit(EXIT_FAILURE);
    }
    indx++;
  }
   // printf("test print: ");
   // printf("%i",numSpac);
   if(numSpac>0){
        cdValid=0;
   } 

   return buffer;
}

char **get_args(char * line) {
   char *saveptr;

  int buffer_size = BUFFER_SIZE+24; //+ just incase
  char **buffer = malloc(sizeof(char) * buffer_size);
  if (buffer==NULL) {
   printf("malloc returned null, exit proc \n");
   exit(EXIT_FAILURE);
  }
  int indx = 0; int print_true = 0;

  char *cl_arg = strtok_r(line, DELIMITER,&saveptr);
   if(strcmp(cl_arg,"cd")==0){
      print_true=1;
      if(cdHasQuot==0){
         while (cl_arg != NULL) {
            buffer[indx] = cl_arg;
            cl_arg = strtok_r(NULL, DELIMITER,&saveptr);
            indx++; 
         }         
      } else{
         while (cl_arg != NULL) {
            buffer[indx] = cl_arg;
            cl_arg = strtok_r(NULL, "\"\'",&saveptr);
            indx++; 
         }         
      }
    }
      else{
         while (cl_arg != NULL) {
            buffer[indx] = cl_arg;
            cl_arg = strtok_r(NULL, DELIMITER,&saveptr);
            indx++; 
         }
      }

      // printf("%i",indx);
  buffer[indx] = NULL;

   // if(print_true==1){
      // int i = 0;
      // while(buffer[i]!=NULL){
      //    printf(buffer[i]);
      //    printf("\nbreak\n");
      //    i++;
      // }
      // printf("%i",i);
   // }

  return buffer;
}


int arg_run(char **args) {

  pid_t child_pid;
  int status;

  if (strcmp(args[0], "exit") == 0) {
      if(args[1]!=NULL){             
         printf("!ERROR! command syntax wrong: %s\n", args[0]);
         return -1; }
      else{
        printf("exiting terminal..\n");
        exit(EXIT_SUCCESS);
      }
  }
   else if(strcmp(args[0], "cd")==0){
      //    printf("args1\n");
      //    printf(args[1]);
      //    printf("\nargs1\n");
      if(cdValid==0) printf("Invalid cd arguments\n"); 
      else if(args[1]==NULL) {
         // printf("to homedie\n"); printf(home_dir); 
         chdir(home_dir); 
         getcwd(curr_dir,sizeof(curr_dir));
         return 0;
      } 
      // if(args[1]!=NULL){
      else{
         if(strcmp(args[1],"~")==0){
         // printf("to homedie\n"); printf(home_dir); 
         chdir(home_dir); 
         getcwd(curr_dir,sizeof(curr_dir));
         return 0;
         }
         else if(cdHasQuot==0){
            // printf("strcat0");
            if(chdir(args[1])!=0){
            printf("chdir failed\n");
            } else getcwd(curr_dir,sizeof(curr_dir));
         } 
         else{
            int buffer_size = BUFFER_SIZE+2; //+2 just incase
            char *dest = malloc(sizeof(char) * buffer_size);
            if (dest==NULL) {
               printf("malloc returned null, exit proc \n");
               exit(EXIT_FAILURE);
            }

            int i=1;
            // printf("strcat");
            while(args[i]!=NULL){
                           // printf("strcat1");
               strcat(dest, args[i]);
                           // printf("strcat2");
               i++;
            }
            if(chdir(dest)!=0){
            printf("chdir failed\n");
            } else getcwd(curr_dir,sizeof(curr_dir));
         }      
      }

      return 0;
   }

  child_pid = fork();

  if (child_pid == 0) {

       if (strcmp(args[0], "history") == 0) {
            if(args[1]!=NULL)             
               printf("!ERROR! command syntax wrong: %s\n", args[0]);
         else{
            int counter = HISTORY_LEN-1;
            for(int i = last_inx+1; i<HISTORY_LEN;i++){
               if(infLoop-counter>0){
                  printf("%i ",infLoop-counter);
                  printf(history[i]);
                  printf(" \n");
               }
               counter--;
            }
            for(int i = 0; i<=last_inx;i++){
               if(infLoop-counter>0){
                  printf("%i ",infLoop-counter);
                  printf(history[i]);
                  printf(" \n");
               }
               counter--;
            }
         }
            // for(int i = last_inx; i>=0; i--){
            //    // printf(infLoop)
            //    printf(history[i]);
            //    printf(" \n");
            // } 
            // for(int i = HISTORY_LEN-1; i>last_inx;i--){
            //       printf(history[i]);
            //       printf(" \n");
            // }
      } 
         else if (execvp(args[0], args) < 0){
            printf("!ERROR! command not found: %s\n", args[0]);
         } 
         // else{
         //      printf("are we here?");
         //      printf("%i",execvp(args[0], args));
         // }
       exit(EXIT_FAILURE);
   } 

   else if (child_pid < 0){
      printf(FORKERR "Error forking"
         RESET "\n");}
   else {
      waitpid(child_pid, & status, WUNTRACED);
   }
   return 1;
}

