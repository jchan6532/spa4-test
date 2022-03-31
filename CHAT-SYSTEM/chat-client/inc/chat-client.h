/* FILE :           chat-client.h
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    The header file for the chat client part of the chat system.
*/
#define __CHATCLIENT_H__
#ifdef __CHATCLIENT_H__

#include <ncurses.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define CHAT_MSG_BUFFER 80
#define PORT 5000
#define MSG_TO_SERVER_SIZE 53
#define NULL_TERMINATION 1
#define MAX_MSG_PACKET_LENGTH 41
#define NON_VISIBLE_ASCII_BOUNDARY 31
#define DEL_CHAR_ASCII 127
#define BACK_SPACE_ASCII 8


int parseArguments(int, char*, char*, struct sockaddr_in*, struct hostent**, char*, char*);
WINDOW* createNewWindow(int height, int width, int starty, int startx);
void inputMessage(WINDOW *win, char *word);
void blankWindow(WINDOW *win);
void displayWindow(WINDOW *win, char *word, int whichRow, int shouldBlank);
char* composeMessage(char* buffer, int messageLength, char* userID);
void makeMessagePackets(char* buffer, int messageLength, char* firstPacket, char* secondPacket);


#endif