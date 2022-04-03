/* FILE :           processMessage.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/
#ifndef PROCESSMESSAGE_H_
#define PROCESSMESSAGE_H_





#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../inc/MasterList.h"


#define TRUE 1
#define FALSE 0
#define MAXMESSAGEBUFFER 79
#define MAXIPBUFFER 16
#define DELIMSIZE 16


#define MAXDELIMSPERMESSAGE 3
#define MAXMESSAGESIZE 81
#define MESSAGESIZEPERPACKET 41
#define IPLENGTH 16
#define MAXBYTESRECORDED 3
#define DELiMETERSIZE 2
#define MESSAGEDIRECTIONBUFFER 4
#define MAXUSERLENGTH 6

#define SUCCESS 1
#define CLIENTSAID_ADIOS -1
#define NOMORESPACE -2


int parseMessage(char* buffer, char* outgoingMessage, char* userName, char* clientIP);
void getUserMessage(char* unParsedMessage, char* parsedMsg);
int checkExistingClients(MASTERLIST* ml);





#endif
