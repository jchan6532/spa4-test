/* FILE :           chat-client.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    The header file for the chat client part of the chat system.
*/
#define __CHATCLIENT_H__
#ifdef __CHATCLIENT_H__

#include <ncurses.h>

#define CHAT_MSG_BUFFER 80


int parseArguments(int, char*, char*);
WINDOW* createNewWindow(int height, int width, int starty, int startx);


#endif