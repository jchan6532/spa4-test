/* FILE :           chat-client.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include <stdio.h>
#include "../inc/processMessage.h"

int main(void)
{
	char tmpMessage[MAXMESSAGEBUFFER] = {};
	
	sprintf(tmpMessage,"%d|172.168.20.22|[mikee]|>>|AAAAAAA|AA|K+DS", 10);
	printf("%s\n", tmpMessage);
	parseMessage(tmpMessage);
	return 0;
}
