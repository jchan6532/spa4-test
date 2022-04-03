#ifndef MASTERLIST_H_
#define MASTERLIST_H_



#include "MessageList.h"
#include "ClientInfo.h"

#define MAXCLIENTS 10

typedef struct MasterListStruct{
    MESSAGELIST* msgListHead;
    CLIENTINFO allClients[MAXCLIENTS];
    
    int numMsgInList;
    int numClients;
} MASTERLIST;



#endif