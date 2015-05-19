#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
This implementation of simple queue in C is used from  (with slight modifications)
http://ben-bai.blogspot.com/2012/04/simple-queue-data-structure-in-ansi-c.html
*/

/**
int main () {
    Queue queue = createQueue();
    queue.display(&queue);

    printf("push item 2\n");
    queue.push(&queue, "Two");    
    printf("push item 3\n");
    queue.push(&queue, "Three");
    printf("push item 6\n");
    queue.push(&queue, "Six");

    queue.display(&queue);

    printf("peek item %s\n", queue.peek(&queue));
    queue.display(&queue);

    printf("pop item %s\n", queue.pop(&queue));
    printf("pop item %s\n", queue.pop(&queue));
    queue.display(&queue);

    printf("pop item %s\n", queue.pop(&queue));
    queue.display(&queue);
    printf("push item 6\n");
    queue.push(&queue, "six");

    queue.display(&queue);
    system("PAUSE");
}
*/

/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push (Queue* queue, char *item) {
    // Create a new node
    Nnode* n = (Nnode*) malloc (sizeof(Nnode));
    strcpy(n->item, item);
    //n->item = item;
    n->next = NULL;

    if (queue->head == NULL) { // no head
        queue->head = n;
    } else{
        queue->tail->next = n;
    }
    queue->tail = n;
    queue->size++;

}
/**
 * Return and remove the first item.
 */
char *pop (Queue* queue) {
    if(queue->size < 1){
        return "NOMORE";
    }
    
    // get the first item
    Nnode* head = queue->head;
    char *item = head->item;
    // move head pointer to next node, decrease size
    queue->head = head->next;
    queue->size--;
    // free the memory of original head
    free(head);
    return item;
}
/**
 * Return but not remove the first item.
 */
char *peek (Queue* queue) {
    Nnode* head = queue->head;
    return  head->item;
}
/**
 * Show all items in queue.
 */
void display (Queue* queue) {
    printf("\nDisplay: ");
    // no item
    if (queue->size == 0)
        printf("No item in queue.\n");
    else { // has item(s)
        Nnode* head = queue->head;
        int i, size = queue->size;
        printf("%d item(s):\n", queue->size);
        for (i = 0; i < size; i++) {
            if (i > 0)
                printf(", ");
            printf("'%s'", head->item);
            head = head->next;
        }
    }
    printf("\n\n");
}
/**
 * Create and initiate a Queue
 */
Queue createQueue () {
    Queue queue;
    queue.size = 0;
    queue.head = NULL;
    queue.tail = NULL;
    queue.push = &push;
    queue.pop = &pop;
    queue.peek = &peek;
    queue.display = &display;
    return queue;
}

