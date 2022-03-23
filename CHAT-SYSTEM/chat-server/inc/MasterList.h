#include "MessageList.h"
#include "ClientInfo.h"

#define MAXCLIENTS 10

typedef struct MasterListStruct{
    MESSAGELIST messageList;
    CLIENTINFO allClients[MAXCLIENTS];
    int numClients;
} MASTERLIST;