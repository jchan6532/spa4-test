/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"

int parseMessage(char* message)
{
	const char s[2] = "~";
	char* token;
	
	token = strtok(message, s);
	
	while(token != NULL)
	{
		splitCounter++;
		switch(splitCounter)
		{
			case: 1
				// DO some stuff
				break;
			case: 2
				// DO some stuff
				break;
			case: 3
				// DO some stuff
				break;
			case: 4
				// DO some stuff
				break;
			case: 2
				// DO some stuff
				break;
			default:
			 // DO SOME STUFF
			 	
				
		}
		printf("%s\n", token);
		token = strtok(NULL, s);
	}
	splitCounter = 0;
	
	return 0;
}


