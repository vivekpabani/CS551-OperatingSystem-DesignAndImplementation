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
#define	RETRY_LATER 12


struct PublList
{
	int publList[MAX_PUBLISHERS];
	int curr_num; /* current number of publishers */
};

struct SubsList
{
	int subsList[MAX_SUBSCRIBERS]; 
	int curr_num; /* current number of subscribers */
};

struct Item //Represents each message
{
	char Mesg[MAX_MESG_LEN];
	int SubsList[MAX_SUBSCRIBERS]; //List of Subscribers who have already read the message
	int num_of_read_completed_subs;
	int deletedFlag; // If 1 it means that the message has been deleted
};

struct ItemsList
{
	struct Item itemsList[5]; // 5 because it is explicitly stated in project description
	int curr_num;
};

struct MsgListPerTopic //LIst of 5 messages per topic
{
	char topic_name[MAX_TOPIC_NAME_LEN + 1];
	struct PublList publishers;
	struct SubsList subscribers;
	struct ItemsList iList; //Hardcoded to 5 since it has been explicitly stated in the project desc
};

struct MsgListPerTopic TopicsList[MAX_TOPICS];
int numTopics;

/* Utility functions */
void topic_init();
int getPublInd(int topic_index, int pub_id);
int getSubsInd(int topic_index, int pub_id);
int findEmptyItemSlot(int topic_index);
int get_next_mesg_from_topic(int topic_index);
void print_buffer_snapshot();
void print_topic_snapshot(int topic_index);
/* End of Utility functions */

MessagingErrorCodes add_topic(char topic_name[]);
MessagingErrorCodes add_publisher_to_topic(char topic_name[], int pubId);
MessagingErrorCodes add_subscriber_to_topic(char topic_name[], int subId);
MessagingErrorCodes publish_messsage(char topic_name[], char mesg[]);
MessagingErrorCodes get_next_message(char topic_name[], char **mesg, int subsId); 
MessagingErrorCodes lookup_topics(char *topics);
