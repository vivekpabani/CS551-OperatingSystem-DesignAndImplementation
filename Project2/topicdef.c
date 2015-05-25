#include "topicdef.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_MODE 1

void topic_init()
{
    memset(TopicsList, 0, sizeof(TopicsList));
	numTopics = 0;
}

MessagingErrorCodes lookup_topics(char topic_names[])
{
    memset(topic_names, 0, sizeof(topic_names));

	for(int i = 0; i < numTopics; i++)
	{
	    strcat(topic_names, TopicsList[i].topic_name);
        strcat(topic_names, ";");
	}

    strcat(topic_names, "\0");
	return MESSAGING_SUCCESS;  //Success
}


MessagingErrorCodes add_topic(char topic_name[])
{
	if(topic_name == NULL || strlen(topic_name)== 0)
		return INP_ERR_NULL_TOPIC_NAME;

	if(strlen(topic_name) > MAX_TOPIC_NAME_LEN)
		return TOPIC_NAME_LEN_OVERFLOW;

	/*Check if topic_name already exists */
	for(int i = 0; i < numTopics; i++)
	{
		if(strncmp(TopicsList[i].topic_name, topic_name, strlen(topic_name)) == 0)
			return DUPLICATE_TOPIC;
	}

	if(numTopics == MAX_TOPICS)
		return TOPICS_FULL;

	strcpy(TopicsList[numTopics].topic_name, topic_name);
	TopicsList[numTopics].publishers.curr_num = 0;
	TopicsList[numTopics].subscribers.curr_num = 0;
	TopicsList[numTopics].iList.curr_num = 0;
	//Initialize all 5 items of all messages to deleted
	for(int i = 0; i < 5; i++)
		TopicsList[numTopics].iList.itemsList[i].deletedFlag = 1;
	numTopics++;
	return MESSAGING_SUCCESS;  //Success
}	

int getPublIndex(int topic_index, int pub_id)
{	
	int foundFlag = 0;
	int i;
	for(i = 0; i < TopicsList[topic_index].publishers.curr_num; i++)
	{
		if(TopicsList[topic_index].publishers.publList[i] == pub_id)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}

int getSubsIndex(int topic_index, int sub_id)
{	
	int foundFlag = 0;
	int i;
	for(i = 0; i < TopicsList[topic_index].subscribers.curr_num; i++)
	{
		if(TopicsList[topic_index].subscribers.subsList[i] == sub_id)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}

int findEmptyItemSlot(int topic_index)
{	
	int foundFlag = 0;
	int i;
	for(i = 0; i < 5; i++)
	{
		if(TopicsList[topic_index].iList.itemsList[i].deletedFlag == 1)
		{	
			foundFlag = 1;
			break;
		}

	}
	if(!foundFlag)
		return -1;
	return i;
}


MessagingErrorCodes add_publisher_to_topic(char topic_name[], int pubId)
{
	int pub_ind;
	int foundFlag = 0;
	int i;
	if(topic_name == NULL || strlen(topic_name)== 0)
		return INP_ERR_NULL_TOPIC_NAME;
			
	for(i = 0; i <numTopics; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;
#if DEBUG_MODE
	printf("Topic Index:%d\n", i);
#endif

	/* Topic name matched at index i. Now add publisher */

	/*Check if duplicate before adding */
	pub_ind = getPublIndex(i, pubId); 
	if(pub_ind != -1)
		return DUPLICATE_PUBLISHER;

	//Adding publisher
	TopicsList[i].publishers.publList[TopicsList[i].publishers.curr_num] = pubId;
#if DEBUG_MODE
	printf("Publisher Index:%d\n", TopicsList[i].publishers.curr_num);
#endif
	TopicsList[i].publishers.curr_num++;
#if DEBUG_MODE
	printf("Publisher Index:%d\n", TopicsList[i].publishers.curr_num);
#endif
	return MESSAGING_SUCCESS;
}

MessagingErrorCodes add_subscriber_to_topic(char topic_name[], int subId)
{
	if(topic_name == NULL)
		return INP_ERR_NULL_TOPIC_NAME;
			
	int foundFlag = 0;
	int i;
	for(i = 0; i <numTopics; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;

	/* Topic name matched at index i. Now add publisher */

	/*Check if duplicate before adding */
	int sub_ind = getSubsIndex(i, subId); 
	if(sub_ind != -1)
		return DUPLICATE_SUBSCRIBER;

	//Adding publisher
	TopicsList[i].subscribers.subsList[TopicsList[i].subscribers.curr_num] = subId;
	TopicsList[i].subscribers.curr_num++;
	return MESSAGING_SUCCESS;
}

MessagingErrorCodes publish_message(char topic_name[], char mesg[])
{     
	if(topic_name == NULL || strlen(topic_name)== 0)
		return INP_ERR_NULL_TOPIC_NAME;

	if(strlen(mesg) > MAX_MESG_LEN)
		return MSG_LEN_OVERFLOW_ERROR;
			
	int foundFlag = 0;
	int i;
	for(i = 0; i <numTopics; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;

	/* Topic name matched at index i. Now add mesg */
#if DEBUG_MODE
	printf("Topic Index:%d\n", i);
#endif

	int emptySlot = findEmptyItemSlot(i);
#if DEBUG_MODE
	printf("EmptySlot:%d\n", emptySlot);
#endif
	if(emptySlot == -1)
		return BUFFER_OVERFLOW;

	TopicsList[i].iList.itemsList[emptySlot].num_of_read_completed_subs = 0;
	TopicsList[i].iList.itemsList[emptySlot].deletedFlag = 0;
	strcpy(TopicsList[i].iList.itemsList[emptySlot].Mesg, mesg);
    
	return MESSAGING_SUCCESS;
}

MessagingErrorCodes get_next_message(char topic_name[], char *mesg[], int subsId)
{
    if(topic_name == NULL || strlen(topic_name)== 0)
		return INP_ERR_NULL_TOPIC_NAME;

	int foundFlag = 0;
	int i;
	for(i = 0; i <numTopics; i++)
	{
		if(strncmp(TopicsList[i].topic_name,topic_name, strlen(topic_name)) == 0)
		{
			foundFlag = 1;
			break;
		}
	}
	if(!foundFlag)
		return TOPIC_NOT_FOUND;
#if DEBUG_MODE
	printf("Topic Index:%d\n", i);
#endif
	int topic_index = i;
	/* Topic name matched at index i */

	/*check if the input subscriber is a valid subscriber */
	int found_flag = 0;
	for(int i = 0; i < TopicsList[topic_index].subscribers.curr_num; i++) 
	{
		if(TopicsList[topic_index].subscribers.subsList[i] == subsId)
		{
			found_flag = 1;
			break;
		}
	}

	if(!found_flag)
		return NOT_SUBSCRIBER_OF_TOPIC;

#if DEBUG_MODE
	printf("Valid Subscriber\n");
#endif
	/* Subscriber is valid subscriber */
	/* Parse through the item list and return the item which subscriber has not yet read */
	int item_present_to_read=0;
	for(int i = 0; i < 5 ; i++)
	{
		if(!TopicsList[topic_index].iList.itemsList[i].deletedFlag)
		{
		int isReadFlag = 0;
		for(int j = 0; j < TopicsList[topic_index].iList.itemsList[i].num_of_read_completed_subs; j++)
		{
			if(TopicsList[topic_index].iList.itemsList[i].SubsList[j] == subsId)
			{
				isReadFlag = 1;
				break; /* There is a match. Thus this mesg is already read by subscriber. So move to next item */
			}
		}
#if DEBUG_MODE
	printf("isReadFlag:%d i:%d \n", isReadFlag, i);
#endif
		if(!isReadFlag)
		{
			/* ith item is not read by subscriber. So, read this item */
			char temp[MAX_MESG_LEN];
			item_present_to_read = 1;
			strcpy(temp, TopicsList[topic_index].iList.itemsList[i].Mesg);
			strcpy(*mesg, temp);
			TopicsList[topic_index].iList.itemsList[i].SubsList[TopicsList[topic_index].iList.itemsList[i].num_of_read_completed_subs++] = subsId;
			/* Check if message is read by all subscribers. If so delete the message */
			if(TopicsList[topic_index].iList.itemsList[i].num_of_read_completed_subs == TopicsList[topic_index].subscribers.curr_num)
				TopicsList[topic_index].iList.itemsList[i].deletedFlag = 1;
			break;
		}
		}
	}
	if(!item_present_to_read)
		return BUFFER_UNDERFLOW;
	return MESSAGING_SUCCESS;
}

void print_buffer_snapshot()
{
	printf("------------------------------------------------------\n");
	printf("TOPICS LIST:\n");
	for(int i = 0; i < numTopics; i++)
	{
		printf("TOPIC %d:%s\n", i, TopicsList[i].topic_name); 
		printf("Publishers List:%d  ", TopicsList[i].publishers.curr_num);
		for(int j = 0; j < TopicsList[i].publishers.curr_num; j++)
		{
			printf("%d,", TopicsList[i].publishers.publList[j]);
		}
		printf("\n");
		printf("Subscribers List:");
		for(int j = 0; j < TopicsList[i].subscribers.curr_num; j++)
		{
			printf("%d,", TopicsList[i].subscribers.subsList[j]);
		}
		printf("\n");
		printf("Items List:\n");
		for(int j = 0; j < 5; j++)
		{
			if(!TopicsList[i].iList.itemsList[j].deletedFlag)
			{
				printf("Item %d:%s:\n", j,  TopicsList[i].iList.itemsList[j].Mesg);
				printf("Deleted Flag:%d   ", TopicsList[i].iList.itemsList[j].deletedFlag);
				printf("Num_read_subscribers:%d   ", TopicsList[i].iList.itemsList[j].num_of_read_completed_subs);
				for(int k = 0; k < TopicsList[i].iList.itemsList[j].num_of_read_completed_subs; k++)
				{
					printf("%d,",TopicsList[i].iList.itemsList[j].SubsList[k]);
				}
			}
	        	printf("\n");
		}
	        printf("------------\n");
	}
	printf("------------------------------------------------------\n");
}

void print_topic_snapshot(int topic_index)
{
	printf("------------------------------------------------------\n");
	int i=topic_index;
	printf("TOPIC %d:%s\n", i, TopicsList[i].topic_name); 
	printf("Publishers List:%d  ", TopicsList[i].publishers.curr_num);
	for(int j = 0; j < TopicsList[i].publishers.curr_num; j++)
	{
		printf("%d,", TopicsList[i].publishers.publList[j]);
	}
	printf("\n");
	printf("Subscribers List:");
	for(int j = 0; j < TopicsList[i].subscribers.curr_num; j++)
	{
		printf("%d,", TopicsList[i].subscribers.subsList[j]);
	}
	printf("\n");
	printf("Items List:\n");
	for(int j = 0; j < 5; j++)
	{
		if(!TopicsList[i].iList.itemsList[j].deletedFlag)
		{
			printf("Item %d:%s:\n", j,  TopicsList[i].iList.itemsList[j].Mesg);
			printf("Deleted Flag:%d   ", TopicsList[i].iList.itemsList[j].deletedFlag);
			printf("Num_read_subscribers:%d   ", TopicsList[i].iList.itemsList[j].num_of_read_completed_subs);
			for(int k = 0; k < TopicsList[i].iList.itemsList[j].num_of_read_completed_subs; k++)
			{
				printf("%d,",TopicsList[i].iList.itemsList[j].SubsList[k]);
			}
		}
        	printf("\n");
	}
        printf("------------\n");
}
