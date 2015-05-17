#include "l_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 Implementation of linked list with features required to store the variables.
 
 */
struct Node *node_init(char *var_name, int var_value, int flag){
    
    struct Node *node = malloc(sizeof(struct Node));
    char *temp_var_name;

    temp_var_name = malloc(sizeof(char) * strlen(var_name));
    strcpy(temp_var_name,var_name);

    node->var_name = temp_var_name;
    node->var_value = var_value;
    node->flag = flag;
    node->next = NULL;

    free(temp_var_name);

    return node;
}

int node_update(struct Node *new_node, int var_value){
    new_node->var_value = var_value;
    if (new_node->flag == 0){
        new_node->flag = 2;
    }
    return 1;
}

struct list *list_init(){

    struct list *new_list = malloc(sizeof(struct list));

    new_list->head = NULL;
    new_list->size = 0;

    return new_list;

}

int list_add(struct list *new_list, int index, char *var_name, int var_value, int flag)
{
    if (new_list->size == 0){

        new_list->head = (struct Node *)malloc(sizeof(struct Node));

        new_list->head->var_value = var_value;

        new_list->head->var_name = malloc(sizeof(char) * strlen(var_name));
        strcpy(new_list->head->var_name ,var_name);

        new_list->head->flag = flag;
        new_list->head->next = NULL;

        ++new_list->size;

        return 1;
    }
    else if (index == -1) {

        struct Node *current = new_list->head;

        while (current->next != NULL){
            current = current->next;
        }

        current->next = (struct Node *)malloc(sizeof(struct Node));

        current->next->var_value = var_value;

        current->next->var_name = malloc(sizeof(char) * strlen(var_name));
        strcpy(current->next->var_name ,var_name);

        current->next->flag = flag;

        current->next->next = NULL;

        ++new_list->size;

        return 1;
    }
    else{
        return 0;
    }
}

struct Node *list_search(struct list *new_list, char *var_name){

    if (strcmp(new_list->head->var_name, var_name)==0) {
        return new_list->head;
    }
    else {
        struct Node *current = new_list->head->next;

        while (current != NULL) {

            if (strcmp(current->var_name, var_name) == 0) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }
}

void list_print(struct list *new_list){

    struct Node *current = new_list->head;
    
    while (current != NULL){
        printf("%s  ",current->var_name);
        printf("\n%d\n",current->var_value);
        current = current->next;
    }
}