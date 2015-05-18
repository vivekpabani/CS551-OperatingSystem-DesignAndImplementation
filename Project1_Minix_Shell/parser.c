#include "parser.h"

/*Validates that the nesting for the pipe command is legal
*/
int validate(char *cmd){
    int i = 0;
    int iterations = strlen(cmd);
    iterations--;
    int level = 0;
    char c;
    //printf("Iterations %d for %s\n", iterations, cmd);
    while(i<iterations){
       c = cmd[i];
       i++;
       if(c=='('){
           level++;
           //printf("Level increased %d\n", level);
       }
       else if(c==')'){
           level--;
           //printf("Level decreased %d\n", level);
       }
       if (level<0){
           //printf("Level less than 0 %d\n", level);
           return 0;
       }
    }
    if(!(level==0)){
        return 0;
    }

    return 1;

}

/** Parses the command line and created a list of queues based on the 
nesting level of the command
*/
void parse_cmd(char *cmd, Queue *queues){
    int i = 0;
    int iterations = strlen(cmd) - 1;
    int level = 0;
    char buf[100] = "";
    memset(buf, 0, sizeof(buf));
    while(i<iterations){
       char c = cmd[i];
       i++;
       if(c=='(' || c==')' || c=='\n'){
           
           // Insert the command into the correct level queue
           if(strcmp(buf, "") && strcmp(buf, " ") ){
               //printf("Inserting '%s'", buf);
                queues[level].push(&queues[level], buf);
           }
            //printf("Pushed a new item %s",buf);
            //printf("Printing level %d", level);
            //queues[level].display(&queues[level]);

           if(c=='('){
            level++;
           }
           else if(c==')'){
            level--;
           }
           if(level>9){
            printf("Only 10 level nesting supported currently");
           }
       
            memset(buf, 0, sizeof(buf));
       }
       else{
           strcat(buf, &c);
       }
            
    }
        if(strlen(buf) > 0){
            if(strcmp(buf, "") && strcmp(buf, " ")){
                queues[0].push(&queues[0], buf);    
            }
        }
    return;
}
