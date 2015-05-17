#include <stdio.h>

struct Node{
    char *var_name;
    int var_value;
    int flag; // flag 0 = old, flag 1 = new, flag 2 = update
    struct Node *next;
};

struct Node *node_init(char *var_name, int var_value, int flag);
int node_update(struct Node *new_node, int var_value);

struct list{
    struct Node *head;
    //struct Node *tail;
    int size;
};

struct list *list_init();
int list_add(struct list *new_list, const int index, char *var_name, int var_value, int flag);
struct Node *list_search(struct list *new_list, char *var_name);
void list_print(struct list *new_list);

