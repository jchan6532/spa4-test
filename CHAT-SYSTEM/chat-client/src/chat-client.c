/* FILE :           chat-client.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    This file includes all the functions for command line parsing, ....
 *                  for the chat client portion of the chat system.
*/


// the chat window code reference
// TITLE    :   ncurses-01.c
// AUTHOR   :   Sam Hsu
// ...

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../inc/chat-client.h"

int main(int argc, char* argv)
{

    char* userID; // client name
    char* serverName; // server name either a name or ip

    // handling message window view
    WINDOW *chat_win, *msg_win;
    int chat_startx, chat_starty, chat_width, chat_height;
    int msg_startx, msg_starty, msg_width, msg_height, i;
    int shouldBlank;
    char buf[CHAT_MSG_BUFFER];

    // once valid arguments, attempt to connect

    initscr();
    cbreak();
    noecho();
    refresh();

    shouldBlank = 0;

    chat_height = 5;
    chat_width  = COLS - 2;
    chat_startx = 1;        
    chat_starty = LINES - chat_height;        
        
    msg_height = LINES - chat_height - 1;
    msg_width  = COLS;
    msg_startx = 0;        
    msg_starty = 0;

    // create message input window
    msg_win = createNewWindow(msg_height, msg_width, msg_starty, msg_startx);
    scrollok(msg_win, TRUE);

    // create the output window
    chat_win = createNewWindow(chat_height, chat_width, chat_starty, chat_startx);
    scrollok(chat_win, TRUE);

    sleep(5);

    // start thread for inputting text


    // destroys the chat window and message window
    delwin(chat_win);
    delwin(msg_win);

    endwin();
    return 0;
}


// functions:
// command line argument parsing
// thread function for incoming message handling
// thread function for outgoing message handling 

int parseArguments(int argc, char* firstArg, char* secondArg)
{
    return 0;
}


// creates the chat window and message window for the client
WINDOW *createNewWindow(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
     
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);               /* draw a box */
    wmove(local_win, 1, 1);             /* position cursor at top */
    wrefresh(local_win);     
    return local_win;
}










