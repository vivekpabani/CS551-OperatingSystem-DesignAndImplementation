#include <stdio.h>
#include "l_list.h"

void var_handle (struct list *new_list, char **argv);
void cal_handle (struct list *new_list, char **argv);

void store_var (struct list *new_list);
void load_var (struct list *new_list);

void display_var(struct list *new_list, char **argv);
