/* FILE :           ClientInfo.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :        Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    The header file contains the client info struct and it is for the chat server part of the chat system. 
*/

#include <time.h>

typedef struct clientInfoStruct {
	int clientSocket;
	char IPAddress[16];
	char UserName[6];
	struct tm* lastTimeUserSentMsg;
	bool isActive;
} CLIENTINFO;