#include "MessageList.h"
#include "ClientInfo.h"

#define MAXCLIENTS 10

typedef struct MasterListStruct{
    MESSAGELIST* msgListHead;
    CLIENTINFO allClients[MAXCLIENTS];
    
    int numMsgInList;
    int numClients;
    int highestClientID;
} MASTERLIST;