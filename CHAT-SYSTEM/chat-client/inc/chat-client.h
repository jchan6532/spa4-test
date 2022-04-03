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
#define MAX_MSG_PACKET_LENGTH 40
#define NON_VISIBLE_ASCII_BOUNDARY 31
#define DEL_CHAR_ASCII 127
#define BACK_SPACE_ASCII 8
#define ZERO_INDEX_ADJUSTMENT 1
#define USERNAME_LENGTH 5 
#define USER_LENGTH 6
#define SERVER_LENGTH 8
#define SERVERNAME_LENGTH 16
#define MAX_MSG_WIN_ROWS 10


int parseArguments(int argc, char* firstArg, char* secondArg, struct sockaddr_in* server_address_ptr, struct hostent** host_ptr, char* userID, char*serverName);
WINDOW* createNewWindow(int height, int width, int starty, int startx);
void inputMessage(WINDOW *win, char *word);
void blankWindow(WINDOW *win);
void blankLine(WINDOW* win, int, WINDOW*);
void displayWindow(WINDOW *win, char *word, int whichRow, int shouldBlank);
char* composeMessage(char* buffer, int messageLength, char* userID);
void makeMessagePackets(char* buffer, int messageLength, char* firstPacket, char* secondPacket);
void shiftLinesUp(WINDOW* win);



#endif