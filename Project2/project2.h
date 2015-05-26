#include <lib.h>
#include <unistd.h>

#define MAX_TNAME_SIZE 255
#define MAX_MESSAGE_SIZE 1024
#define MAX_TLIST_SIZE 255

int sys_tinit(void)
{
    
    message m;
    return ( _syscall(PM_PROC_NR, TINIT, &m) );
    
}

int sys_tlookup(char *topics)
{
    message m;
    
    m.m1_p1 = topics;
    
    return ( _syscall(PM_PROC_NR, TLOOKUP, &m) );
}

int sys_tcreate(char *topic_name)
{
    message m;

    m.m1_p1 = topic_name;
    
    return ( _syscall(PM_PROC_NR, TCREATE, &m) );
}

int sys_tpublisher(int pubId, char *topic_name)
{
    message m;
    
    m.m1_i1 = pubId;
    m.m1_p1 = topic_name;

    return ( _syscall(PM_PROC_NR, TPUBLISHER, &m) );
}

int sys_tsubscriber(int subId, char *topic_name)
{
    message m;
    
    m.m1_i1 = subId;
    m.m1_p1 = topic_name;
    
    return ( _syscall(PM_PROC_NR, TSUBSCRIBER, &m) );
}

int sys_tpublish(int pubId, char *topic_name, char *mesg)
{
    message m;
    
    m.m1_i1 = pubId;
    m.m1_p1 = topic_name;
    m.m1_p2 = mesg;

    return ( _syscall(PM_PROC_NR, TPUBLISH, &m) );
}

int sys_tretrieve(char *topic_name, char *mesg, int subId)
{
    message m;
    
    m.m1_i1 = subId;
    m.m1_p1 = topic_name;
    m.m1_p2 = mesg;
    
    int sta = _syscall(PM_PROC_NR, TRETRIEVE, &m);
    return sta;
}
