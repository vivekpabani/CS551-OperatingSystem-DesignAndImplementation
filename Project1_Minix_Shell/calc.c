#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "file_handle.h"
#include "calc.h"

#define VAR_FILE "/root/variable_file"

void var_handle (struct list *new_list, char **argv){
    /*
     //search for the variable.
     //If exists, update the value.
     //If doesn't exist, add to the list.
     */

    char *temp_var_name, *temp_var_value;

    struct Node *temp;
    
    //expected command syntax : int <var_name> = <var_value>
    //extract the var_name and var_value

    if(!argv[1] || !argv[2] || !argv[3] || strcmp(argv[2], "=")!=0){
        printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
        printf("int <var_name> = <var_value>\n");
        return;
    }

    temp_var_name = argv[1];
    temp_var_value = argv[3];

    if (temp_var_name[0] == '$'){
        printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
        printf("int <var_name> = <var_value>\n");
        return;
    }

    size_t length = strlen(temp_var_value);

    for (int i=0; i<length; i++)
        if (!isdigit(temp_var_value[i]))
        {
            printf ("Entered value is not a number.\n");
            return;
        }

    if (new_list->size == 0){
        //if emplty list, just add the variable as head.

        list_add(new_list, -1, temp_var_name, (atoi)(temp_var_value), 1);

        return;
    }
    else {
        //if more than one element, search for the element in the list.
        temp = list_search(new_list, temp_var_name);

        if (temp == NULL){
            //if not found, add to the list.
            list_add(new_list, -1, temp_var_name, (atoi)(temp_var_value), 1);
            return;
        }
        else{
            //if found, updatet the value.
            node_update(temp, (atoi)(temp_var_value));
            return;
        }
    }
}

void cal_handle (struct list *new_list, char **argv){
    /*
     //function for calculator app.
     //accepts three variables and an operand in predefined syntax.
     //calculates the answer if variables already defined, otherwise gives error message.
     */

    char *ans_var, *first_var, *second_var, *operation;
    struct Node *ans_node, *first_node, *second_node;
    int ans, first, second;

    //expected command syntax : cal <result_var> = $<first_var> <operand> $<second_var>
    ans_var = argv[1];
    first_var = argv[3];
    second_var = argv[5];
    operation = argv[4];

    int f_num = 1, s_num = 1;
    size_t length;

    //Answer variable

    if (ans_var[0] == '$' || strcmp(argv[2], "=")!= 0){
        printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
        printf("cal <result_var> = [$<first_var> | <numeric_value>] <operation> [$<second_var> | <numeric_value>]\n");
        return;
    }

    //First variable

    length = strlen(first_var);

    //Check if numeric value.
    for (int i=0; i<length; i++)
        if (!isdigit(first_var[i]))
        {
            f_num = 0;
            break;
        }

    if (f_num == 0){
        //if non numeric, check syntax, and search for the variable in the list.
        if (first_var[0] == '$'){
            first_var = first_var + 1;
        }
        else {
            printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
            printf("cal <result_var> = [$<first_var> | <numeric_value>] <operation> [$<second_var> | <numeric_value>]\n");
            return;
        }

        if ((first_node = list_search(new_list, first_var)) == NULL) {
            printf("variable(s) not defined for the operation.\n");
            return;
        }
        else {
            first = first_node->var_value;
        }
    }
    else{
        first = atoi(first_var);
    }

    //Second variable

    length = strlen(second_var);

    //Check if numeric value.
    for (int i=0; i<length; i++)
        if (!isdigit(second_var[i]))
        {
            s_num = 0;
            break;
        }

    if (s_num == 0){
        //if non numeric, check syntax, and search for the variable in the list.
        if (second_var[0] == '$'){
            second_var = second_var + 1;
        }
        else {
            printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
            printf("cal <result_var> = [$<first_var> | <numeric_value>] <operation> [$<second_var> | <numeric_value>]\n");
            return;
        }

        if ((second_node = list_search(new_list, second_var)) == NULL) {
            printf("variable(s) not defined for the operation.\n");
            return;
        }
        else {
            second = second_node->var_value;
        }
    }
    else{
        second = atoi(second_var);
    }
    

    if(operation[1] == '\0'){
        //if operation correctly defined, and within defined range, calculate the answer.
        char op = operation[0];

        if (op == '+'){
            ans = first + second;
        }
        else if(op == '-'){
            ans = first - second;
        }
        else if(op == '*'){
            ans = first*second;
        }
        else if(op == '/'){
            ans = first/second;
        }
        else{
            // if operation not defined, return with error message.
            printf("operation is out of range. (Choose one of +, -, *, / ).\n");
            return;
        }
    }
    else{
        printf("operation not defined correctly.\n");
        return;
    }
    
    //search for the answer variable in the list.
    ans_node = list_search(new_list, ans_var);

    if (ans_node == NULL){
        // if answer variable not defined already, add to the list.
        list_add(new_list, -1, ans_var, ans, 1);
        return;
    }
    else {
        // if answer variable defined already, update the value.
        node_update(ans_node, ans);
        return;
    }
}

void store_var (struct list *new_list){
    /*
     //Function to store the variables on the disk.
     //Gets called when the shell quits.
     */

    createVarFile(VAR_FILE);

    SH_FileHandle fh;
    SH_VarHandle vh;

    int fileOpenSuccess = openVarFile(VAR_FILE, &fh);
    int writeSuccess, readSuccess;

    struct Node *temp_node;

    temp_node = new_list->head;

    while (temp_node != NULL) {
        //write all the variables from the list to the file.
        //each line will have one variable and its value.
        vh = (SH_VarHandle) calloc(BLOCK_SIZE, sizeof(char));
        char temp[15];

        sprintf(temp, "%d", temp_node->var_value);
        strcat (vh, temp_node->var_name);
        strcat (vh, " ");
        strcat (vh, temp);

        writeSuccess = writeVar(fh.totalNumVars-1, &fh, vh);
        fh.totalNumVars++;

        vh = "";
        temp_node = temp_node->next;
    }
    closeVarFile(&fh);
}

void load_var (struct list *new_list){
    /*
     //function to load all the variables stored on disk to the memory
     //gets called when the shell starts.
     */

    if(access(VAR_FILE, F_OK ) == -1) {
        //check if the files exist. if not, just return.
        return;
    }
    else{
        //if exist, read variables and add to the list.
        SH_FileHandle fh;
        SH_VarHandle vh;

        int fileOpenSuccess = openVarFile(VAR_FILE, &fh);

        int readVariables = 0;
        int readVarSuccess;


        char *var_name, *var_value;
        char temp[BLOCK_SIZE];

        for (int i = 0; i < fh.totalNumVars ; ++i) {
            vh = (SH_VarHandle) calloc(BLOCK_SIZE, sizeof(char));
            readVarSuccess = readVar(i, &fh, vh);
            strcpy(temp, vh);
            var_name = strtok(temp, " ");
            var_value = strtok(NULL, " ");
            list_add(new_list, -1, var_name, (atoi)(var_value), 0);
            free(vh);
        }
        return;
    }
}

void display_var(struct list *new_list, char **argv){
    //function to display the variable.
    //expected syntax : display $<var_name>
    
    char *temp_var_name;
    struct Node *temp;

    temp_var_name = argv[1];

    if (temp_var_name[0] == '$'){
        // if correct syntax, remove the $ sign.
        temp_var_name = temp_var_name + 1;
    }
    else{
        printf("variable syntax not correct. Please follow below syntax (space seperated):\n");
        printf("display $<var_name>\n");
        return;
    }

    //search for the variable in the list.
    temp = list_search(new_list, temp_var_name);

    if (temp == NULL){
        printf("variable has not been initialized.\n");
        return;
    }
    else{
        printf("%s : %d\n",temp_var_name, temp->var_value);
        return;
    }
}
