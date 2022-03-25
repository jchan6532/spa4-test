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
    int myserversocket, len, done;
    int whichClient;
    char* userID = "Erica"; // client name
    char* serverName = "localhost"; // server name either a name or ip
    struct sockaddr_in server_address;
    struct hostent* host;
    char buffer[CHAT_MSG_BUFFER];

    // not enough args
    /*if (argc != 3)
    {
        printf("USAGE: chat-client.c -userID -serverName\n");
        return 1;
    }*/

    parseArguments(argc, &argv[1], &argv[2], &server_address, &host, userID, serverName);

    // get host info
    host = gethostbyname(serverName);

    // initialize struct to get a socket to host
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy(&server_address.sin_addr, host->h_addr, host->h_length);
    server_address.sin_port = htons(PORT);

    // get server socket
    if((myserversocket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {

        printf ("[CLIENT] : Getting Client Socket - FAILED\n");
        return 3;
    }

    // attempt socket connection
    printf("[CLIENT] : Connecting to server\n");
    fflush(stdout);
    if (connect(myserversocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        printf("[CLIENT] : Connect to server - FAILED\n");
        close(myserversocket);
        return 4;
    }

    done =1;
    memset(buffer, 0, CHAT_MSG_BUFFER);
    if (done == 1) printf("Enter your [chat text]");

    while(done)
    {
        printf(">> ");
        fflush(stdout);
        fgets(buffer, CHAT_MSG_BUFFER, stdin);
        if(buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';

        write(myserversocket, buffer, strlen(buffer));
        //memset(buffer, 0, BUFSIZ);
        //len = read (myserversocket, buffer, CHAT_MSG_BUFFER);
        printf("SENT << %s\n", buffer);
        fflush(stdout);
        if(strcmp(buffer, "bye") == 0){
            done = 0;   
        }
    }


    close(myserversocket);
    printf("[CLIENT] : Client done.\n");




    /*
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

    // establishes the dimensions for the chat window
    shouldBlank = 0;
    chat_height = 5;
    chat_width  = COLS - 2;
    chat_startx = 1;        
    chat_starty = LINES - chat_height;        
    
    // establishes the dimensions for the msg window
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

    endwin();*/
    return 0;
}


// functions:
// command line argument parsing
// thread function for incoming message handling
// thread function for outgoing message handling 

int parseArguments(int argc, char* firstArg, char* secondArg, struct sockaddr_in* server_address, struct hostent** host, char* userID, char* serverName)
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










