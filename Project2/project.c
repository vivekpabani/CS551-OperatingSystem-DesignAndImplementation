#include <project2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOPICS 10
#define MAX_SUBSCRIBERS 10
#define MAX_PUBLISHERS 10
#define MAX_TOPIC_NAME_LEN 100
#define MAX_MESG_LEN 100
#define MAX_TOPICS_SIZE 1200


char *return_codes[]={
    "MESSAGING_SUCCESS",
    "INP_ERR_NULL_TOPIC_NAME",
    "TOPIC_NAME_LEN_OVERFLOW",
    "TOPIC_NOT_FOUND",
    "DUPLICATE_PUBLISHER",
    "DUPLICATE_SUBSCRIBER",
    "MSG_LEN_OVERFLOW_ERROR",
    "BUFFER_OVERFLOW",
    "NOT_SUBSCRIBER_OF_TOPIC",
    "BUFFER_UNDERFLOW",
    "DUPLICATE_TOPIC",
    "TOPICS_FULL"
};

void testTopicCreate();
void testTopicPublisher();
void testTopicSubscriber();
void testTopicPublish();
void testTopicRetrieve();
void testTopicLookup();

void testTopicCreate(){
    printf("\n----Topic Create----\n");

    int status;
    char topic[MAX_TOPIC_NAME_LEN];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    status = sys_tcreate(topic);
    
    printf("\nStatus : %s\n", return_codes[status]);

}

void testTopicLookup(){
    int status;
    
    printf("\n----Topic Lookup----\n");

    char all_topics[MAX_TOPICS_SIZE];
    status = sys_tlookup(all_topics);
    
    printf("\nRetrieved topics : \n");
    
    char *token = strtok(all_topics, ";");
    
    while(token) {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
}

void testTopicPublisher(){
    
    printf("\n----Become a publisher of a topic.----\n");
    
    int status;
    int pubID;
    
    char topic[MAX_TOPIC_NAME_LEN];
    
    printf("\nEnter your publisher ID :");
    scanf("%d",&pubID);
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);

    status = sys_tpublisher(pubID, topic);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void testTopicSubscriber(){
    
    printf("\n----Become a subscriber of a topic.----\n");
    
    int status;
    int subID;
    
    printf("\nEnter your subscriber ID :");
    scanf("%d",&subID);
    
    char topic[MAX_TOPIC_NAME_LEN];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);

    status = sys_tsubscriber(subID, topic);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void testTopicPublish(){
    
    printf("\n----Publish a message to a topic.----\n");
    
    int status;
    int pubID;
    
    printf("\nEnter your publisher ID :");
    scanf("%d",&pubID);
    
    char topic[MAX_TOPIC_NAME_LEN];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    char msg[MAX_MESG_LEN];
    
    printf("\nEnter a message :");
    scanf (" %[^\n]%*c", msg);
    
    status = sys_tpublish(pubID, topic, msg);
    printf("\nStatus : %s\n", return_codes[status]);
    
}

void testTopicRetrieve(){
    printf("\n----Message retrieval from a topic----\n");
    
    
    int status;
    int subID;
    
    printf("\nEnter your subscriber ID :");
    scanf("%d",&subID);
    
    char topic[MAX_TOPIC_NAME_LEN];
    
    printf("\nEnter a topic :");
    scanf (" %[^\n]%*c", topic);
    
    char msg1[MAX_MESG_LEN];
    
    status = sys_tretrieve(topic, msg1, subID);
    printf("\nStatus : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("\nMessage received : %s \n",msg1);
    }
    
}



int main(int argc, char** argv)
{
    
    int input=-1;
    sys_tinit();
    
    printf("\n\n------------Project 2----------\n\n");
    
    while(1)
    {
	printf("\n----------------Available Operations----------------\n");
        printf("1. Create a topic. \n");
        printf("2. Become a publisher of a topic. \n");
        printf("3. Become a subscriber of a topic. \n");
        printf("4. Publish a message to a topic. \n");
        printf("5. Retrieve a message from a topic. \n");
        printf("6. Topic lookup. \n");
        
        printf("0. Exit the Program \n\n");
        
        printf("Please choose one option : ");
        scanf(" %d",&input);
        
        switch(input)
        {
            case 1:
                testTopicCreate();
                break;
            case 2:
                testTopicPublisher();
                break;
            case 3:
                testTopicSubscriber();
                break;
            case 4:
                testTopicPublish();
                break;
            case 5:
                testTopicRetrieve();
                break;
            case 6:
                testTopicLookup();
                break;
            case 0:
                exit(0);
                break;
            default :
                printf("\nIncorrect input. Try again. \n");
                break;
        }
    }
}