#include <project2.h>
#include <stdio.h>

#define MAX_TOPICS 10
#define MAX_SUBSCRIBERS 10
#define MAX_PUBLISHERS 10
#define MAX_TOPIC_NAME_LEN 100
#define MAX_MESG_LEN 100
#define MAX_TOPICS_SIZE 1200

typedef int MessagingErrorCodes;

#define	MESSAGING_SUCCESS 0
#define	INP_ERR_NULL_TOPIC_NAME 1
#define	TOPIC_NAME_LEN_OVERFLOW 2
#define	TOPIC_NOT_FOUND 3
#define	DUPLICATE_PUBLISHER 4
#define	DUPLICATE_SUBSCRIBER 5
#define	MSG_LEN_OVERFLOW_ERROR 6
#define	BUFFER_OVERFLOW 7
#define	NOT_SUBSCRIBER_OF_TOPIC 8
#define	BUFFER_UNDERFLOW 9
#define	DUPLICATE_TOPIC 10
#define	TOPICS_FULL 11

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

void testNullTopic();
void testDuplicateTopic();
void testPublishTopicNotFound();
void testSubscribeTopicNotFound();
void testDuplicatePublisher();
void testDuplicateSubscriber();
void testBufferOverFlow();
void testBufferUnderFlow();


void testTopicCreate(){
    int status;
    sys_tinit();
    
    printf("\nTopic Create Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testTopicLookup(){
    int status;
    sys_tinit();
    
    printf("\nTopic Create Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding another topic : TOPIC_2\n");
    status = sys_tcreate("TOPIC_2");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding another topic : TOPIC_3\n");
    status = sys_tcreate("TOPIC_3");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Retrieving topics\n");
    char all_topics[MAX_TOPICS_SIZE];
    status = sys_tlookup(all_topics);
    
    printf("Retrieved topics : \n");
    char *token = strtok(all_topics, ";");
    while(token) {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
 
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testTopicPublisher(){
    int status;
    sys_tinit();
    
    printf("\nBecome a publisher of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testTopicSubscriber(){
    int status;
    sys_tinit();
    
    printf("\nBecome a subscriber of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_tsubscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testTopicPublish(){
    int status;
    sys_tinit();
    
    printf("\nPublish a message to a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testTopicRetrieve(){
    int status;
    sys_tinit();
    
    printf("\nMessage retrieval from a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_tsubscriber(2,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    char msg1[MAX_MESG_LEN];
    
    printf("Retrieving the first message from topic : TOPIC_1\n");
    status = sys_tretrieve("TOPIC_1", msg1, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg1);
        printf("Test Passed\n");
    }
    else{
        printf("Message Retrieval Failed\n");
        printf("Test Failed\n");
    }
    
}



void testNullTopic(){
    int status;
    sys_tinit();
    
    printf("\nNull Topic Test\n");
    
    printf("Adding a null topic\n");
    status = sys_tcreate("");
    
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 1){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testDuplicateTopic(){
    int status;
    sys_tinit();
    
    printf("\nDuplicate Topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Adding again the topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 10){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testPublishTopicNotFound(){
    int status;
    sys_tinit();
    
    printf("\nBecome a publisher of unavailable topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of unavailable topic : TOPIC_20\n");
    status = sys_tpublisher(1,"TOPIC_20");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 3){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testSubscribeTopicNotFound(){
    int status;
    sys_tinit();
    
    printf("\nBecome a subscriber of unavailable topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of unavailable topic : TOPIC_20\n");
    status = sys_tsubscriber(1,"TOPIC_20");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 3){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testDuplicatePublisher(){
    int status;
    sys_tinit();
    
    printf("\nDuplicate publisher of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking again to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 4){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testDuplicateSubscriber(){
    int status;
    sys_tinit();
    
    printf("\nDuplicate subscriber of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_tsubscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking again to become a subscriber of topic : TOPIC_1\n");
    status = sys_tsubscriber(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 5){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testBufferOverFlow(){
    int status;
    sys_tinit();
    
    printf("\nMessage buffer overflow of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_2 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_2");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_3 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_3");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_4 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_4");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_5 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_5");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_6 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_6");
    printf("Status : %s\n", return_codes[status]);
    
    
    if(status == 7){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}

void testBufferUnderFlow(){
    int status;
    sys_tinit();
    
    printf("\nMessage buffer underflow of a topic Test\n");
    
    printf("Adding a topic : TOPIC_1\n");
    status = sys_tcreate("TOPIC_1");
    
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a publisher of topic : TOPIC_1\n");
    status = sys_tpublisher(1,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_1 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_1");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Publishing a message : MESSAGE_2 to topic : TOPIC_1\n");
    status = sys_tpublish(1,"TOPIC_1","MESSAGE_2");
    printf("Status : %s\n", return_codes[status]);
    
    printf("Asking to become a subscriber of topic : TOPIC_1\n");
    status = sys_tsubscriber(2,"TOPIC_1");
    printf("Status : %s\n", return_codes[status]);
    
    char msg1[MAX_MESG_LEN];
    
    printf("Retrieving the first message from topic : TOPIC_1\n");
    status = sys_tretrieve("TOPIC_1", msg1, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg1);
    }
    else{
        printf("Message Retrieval Failed\n");
    }
    
    char msg2[MAX_MESG_LEN];
    printf("Retrieving the second message from topic : TOPIC_1\n");
    status = sys_tretrieve("TOPIC_1", msg2, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 0){
        printf("Message received : %s \n",msg2);
    }
    else{
        printf("Message Retrieval Failed\n");
    }
    
    char msg3[MAX_MESG_LEN];
    printf("Retrieving the third (unavailable) message from topic : TOPIC_1\n");
    status = sys_tretrieve("TOPIC_1", msg3, 2);
    printf("Status : %s\n", return_codes[status]);
    
    if(status == 9){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}


int main(int argc, char** argv)
{
    
    int input=-1;
    
    printf("\nProject 2 Testing\n");
    
    while(1)
    {
	printf("\n----------------Available Tests----------------\n");
        printf("1. Create a topic. \n");
        printf("2. Become a publisher of a topic. \n");
        printf("3. Become a subscriber of a topic. \n");
        printf("4. Publish a message to a topic. \n");
        printf("5. Retrieve a message from a topic. \n");
        printf("6. Create a null topic. \n");
        printf("7. Create a duplicate topic. \n");
        printf("8. Become a publisher of a topic which does not exist. \n");
        printf("9. Become a subscriber of a topic which does not exist. \n");
        printf("10. Duplicate Publisher. \n");
        printf("11. Duplicate Subscriber. \n");
        printf("12. Message Buffer Overflow (more than 5 messages.) \n");
        printf("13. Message Buffer Underflow (no more messages available.) \n");
        printf("14. Test lookup of topics \n");
        
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
                testNullTopic();
                break;
            case 7:
                testDuplicateTopic();
                break;
            case 8:
                testPublishTopicNotFound();
                break;
            case 9:
                testSubscribeTopicNotFound();
                break;
            case 10:
                testDuplicatePublisher();
                break;
            case 11:
                testDuplicateSubscriber();
                break;
            case 12:
                testBufferOverFlow();
                break;
            case 13:
                testBufferUnderFlow();
                break;
            case 14:
                testTopicLookup();
                break;
            case 0:
                exit(0);
                break;
            default :
                printf("Incorrect input. Try again. \n");
                break;
        }
    }
}
