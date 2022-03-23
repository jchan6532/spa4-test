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

    printf("Client started...\n");

    parseArguments(argc, &argv[1], &argv[2], &server_address, &host, userID, serverName);

    // handling message window view
    WINDOW *chat_win, *msg_win;
    int chat_startx, chat_starty, chat_width, chat_height;
    int msg_startx, msg_starty, msg_width, msg_height, i;
    int shouldBlank;

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

        displayWindow(msg_win, "[CLIENT] : Getting Client Socket - FAILED", 0, 1);
        sleep(5);
        delwin(chat_win);
        delwin(msg_win);
        endwin();
        return 3;
    }

    // attempt socket connection
    displayWindow(msg_win, "[CLIENT] : Connecting to server", 0, 1);
    sleep(2);
    fflush(stdout);
    if (connect(myserversocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        displayWindow(msg_win, "[CLIENT] : Connect to server - FAILED", 1, 0);
        sleep(5);
        delwin(chat_win);
        delwin(msg_win);
        endwin();
        close(myserversocket);
        return 4;
    }

    done =1;
    memset(buffer, 0, CHAT_MSG_BUFFER);
    if (done == 1) displayWindow(chat_win, "Enter your [chat text]", 0, 0);

    int rowCount = 1;
    // client should exit when user types >> bye <<
    while(done)
    {
        inputMessage(chat_win, buffer);
        displayWindow(msg_win, buffer, rowCount, 0);
        fflush(stdout);


        if(buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
        
        // exit loop if user enters the exit command
        if (strcmp(buffer, ">>bye<<") == 0)
        {
            break;
        }

        write(myserversocket, buffer, strlen(buffer));
        memset(buffer, 0, CHAT_MSG_BUFFER);
        len = read (myserversocket, buffer, CHAT_MSG_BUFFER);
        /*char* msgReceived = (char*)malloc(sizeof(buffer) + strlen(">> "));
        strcpy(msgReceived, "<< ");
        strcat(msgReceived, buffer);*/
        rowCount++;
        displayWindow(msg_win, buffer, rowCount, 0);
        fflush(stdout);
        rowCount++;
    }


    close(myserversocket);
    printf("[CLIENT] : Client done.\n");
    fflush(stdout);


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


// gets input characters from the user
void inputMessage(WINDOW *win, char *word)
{
  int i, ch;
  int maxrow, maxcol, row = 1, col = 0;
     
  blankWindow(win);                          /* make it a clean window */
  getmaxyx(win, maxrow, maxcol);          /* get window size */
  bzero(word, CHAT_MSG_BUFFER);
  wmove(win, 1, 1);                       /* position cusor at top */
  for (i = 0; (ch = wgetch(win)) != '\n'; i++) 
  {
    word[i] = ch;                       /* '\n' not copied */
    if (col++ < maxcol-2)               /* if within window */
    {
      wprintw(win, "%c", word[i]);      /* display the char recv'd */
    }
    else                                /* last char pos reached */
    {
      col = 1;
      if (row == maxrow-2)              /* last line in the window */
      {
        scroll(win);                    /* go up one line */
        row = maxrow-2;                 /* stay at the last line */
        wmove(win, row, col);           /* move cursor to the beginning */
        wclrtoeol(win);                 /* clear from cursor to eol */
        box(win, 0, 0);                 /* draw the box again */
      } 
      else
      {
        row++;
        wmove(win, row, col);           /* move cursor to the beginning */
        wrefresh(win);
        wprintw(win, "%c", word[i]);    /* display the char recv'd */
      }
    }
  }
}  /* input_win */


// clears window if shouldBlank is 1
// otherwise, updates window with new info
void displayWindow(WINDOW *win, char *word, int whichRow, int shouldBlank)
{
  if(shouldBlank == 1) blankWindow(win);                /* make it a clean window */
  wmove(win, (whichRow+1), 1);                       /* position cusor at approp row */
  wprintw(win, word);
  wrefresh(win);
} /* display_win */


// clears all info from window
void blankWindow(WINDOW *win)
{
  int i;
  int maxrow, maxcol;
     
  getmaxyx(win, maxrow, maxcol);
  for (i = 1; i < maxcol-2; i++)  
  {
    wmove(win, i, 1);
    refresh();
    wclrtoeol(win);
    wrefresh(win);
  }
  box(win, 0, 0);             /* draw the box again */
  wrefresh(win);
}  /* blankWin */








