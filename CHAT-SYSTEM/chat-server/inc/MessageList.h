/* FILE :           MessageList.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :        Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    The header file contains the message list in a struct, it includes message, 
 *                    senderIP and pointer to next as a struct, and it is for the chat server 
 *                    part of the chat system. 
*/

typedef struct MessageListStruct{
	char Message[81];
	char SenderIP[16];
	int arrowIndex;
	struct MessageListStruct* next;
} MESSAGELIST;