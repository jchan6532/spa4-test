/* FILE :           MasterList.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :        Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    The header file contains the constant, message list, and client info in a struct,
 *                  and it is for the chat server part of the chat system. 
*/

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