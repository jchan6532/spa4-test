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
	
	
	strcpy(tmpMessage, "172.168.20.22~[mike]~>>~AAAAAAAAAA~(03:04:20)");
	parseMessage(tmpMessage);
	return 0;
}
