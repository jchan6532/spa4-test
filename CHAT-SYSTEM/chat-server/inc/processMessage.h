/* FILE :           processMessage.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include <stdio.h>
#include <string.h>


#define TRUE 1
#define FALSE 0
#define MAXMESSAGEBUFFER 79
#define MAXIPBUFFER 16
#define DELIMSIZE 16

int parseMessage(char* message);

void getUserMessage(char* message, char* msg);
