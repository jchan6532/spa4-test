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


// https://stackoverflow.com/questions/2283494/get-ip-address-of-an-interface-on-linux
// ...

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include "../inc/chat-client.h"

typedef struct threadInfoStruct 
{
    int socketNumber;
    WINDOW* msgWin;
} THREADDATA;

bool end;

void* acceptServerMsgs(void* data)
{
    THREADDATA threadData = *((THREADDATA*)data);
    struct timeval timeout;
    timeout.tv_sec = 5; // sec
    timeout.tv_usec = 0; // ms
    
    char buffer[CHAT_MSG_BUFFER];
    int len;
    int rowCount = 1;
    
    while(1)
    {
        setsockopt (threadData.socketNumber, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout));
        
        memset(buffer, 0, CHAT_MSG_BUFFER);
        len = read (threadData.socketNumber, buffer, CHAT_MSG_BUFFER);
        if(len > 0){
            rowCount++;
            displayWindow(threadData.msgWin, buffer, rowCount, 0);
            fflush(stdout);
            rowCount++;
        }
        len = 0;
        
        usleep(1);
        
        if(end == true)
        {
            break;
        }
    }
    
    pthread_exit((void*)1);
    return (void*)1;
}





// handling message window view
// moved these to global variables to make sure signal handler could close them if needed
WINDOW *chat_win, *msg_win;

// needed to handle when client exits, makes sure the windows are destroyed after any seg faults etc.
void clientExitSignalHandler(int signal_number)
{
    displayWindow(msg_win, "Exiting client...", 4, 0);
    sleep(2);
    delwin(chat_win);
    delwin(msg_win);
    endwin();
    if (errno != 0)
    {
        printf("Error: %s\n", strerror(errno));
    }
    exit(1);
}

// function for outgoing messages

int main(int argc, char* argv[])
{
    end = false;
    
    int myserversocket, len, done;
    int whichClient;
    char userID[USER_LENGTH] = ""; // client name
    char serverName[SERVERNAME_LENGTH] = ""; // server name either a name or ip
    struct sockaddr_in server_address;
    struct hostent* host;
    char buffer[CHAT_MSG_BUFFER];
    int status;

    // if an error occurs, this makes sure the window exits correctly
    signal(SIGTERM, clientExitSignalHandler);
    signal(SIGSEGV, clientExitSignalHandler);
    signal(SIGINT, clientExitSignalHandler);

    // check argcs
    if (argc != 3)
    {
        printf("USAGE: chat-client.c -userID -serverNameOrIP\n");
        return 1;
    }
    else
    {
        status = parseArguments(argc, argv[1], argv[2], &server_address, &host, userID, serverName);
        if(status == 1)
        {
            exit(1);
        }

        printf("Client started...\n");
    }


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

    // check null
    if (host == NULL)
    {
        exit(1);
    }


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
    //sleep(2);
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
    
    THREADDATA threadData;
    threadData.socketNumber = myserversocket;
    threadData.msgWin = msg_win;
    pthread_t readingThreadID;
    pthread_create(&readingThreadID, NULL, acceptServerMsgs, (void*)&threadData);
    usleep(10); 

    done =1;
    memset(buffer, 0, CHAT_MSG_BUFFER);
    if (done == 1) displayWindow(chat_win, "Enter your [chat text]", 0, 0);

    int rowCount = 1;
    // client should exit when user types >> bye <<
    while(done)
    {
        inputMessage(chat_win, buffer);
        //displayWindow(msg_win, buffer, rowCount, 0);
        fflush(stdout);

        // handle message input newline character
        // inputMessage function does not handle the return key entered
        if(buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
        char tmpBuffer[128] = {};
        

// 		  sprintf(tmpBuffer,"%d|[%s]|<<|%s",(int)strlen(buffer),userID, buffer);
//      write(myserversocket, tmpBuffer, strlen(tmpBuffer));

        int messageLength = strlen(buffer);

        // check if message length is greater than 40
        if (messageLength > MAX_MSG_PACKET_LENGTH)
        {
            char* firstPacket = (char*)malloc(MAX_MSG_PACKET_LENGTH + NULL_TERMINATION);
            char* secondPacket = (char*)malloc(messageLength - (int)MAX_MSG_PACKET_LENGTH + NULL_TERMINATION);
            makeMessagePackets(buffer, messageLength, firstPacket, secondPacket);


            char* firstPacketMessageToServer = composeMessage(firstPacket, MAX_MSG_PACKET_LENGTH, userID);
            write(myserversocket, firstPacketMessageToServer, strlen(firstPacketMessageToServer));
            //displayWindow(msg_win, firstPacketMessageToServer, rowCount++, 0);
            char* secondPacketMessageToServer = composeMessage(secondPacket, strlen(secondPacket), userID);
            write(myserversocket, secondPacketMessageToServer, strlen(secondPacketMessageToServer));
            //displayWindow(msg_win, secondPacketMessageToServer, rowCount++, 0);

            free(firstPacketMessageToServer);
            free(firstPacket);
            free(secondPacket);
            free(secondPacketMessageToServer);
        }
        else
        {
            
            // attach headers / footers to message for server
            char* messageToServer = composeMessage(buffer, messageLength, userID);
            // sends message to server
            write(myserversocket, messageToServer, strlen(messageToServer));
            //displayWindow(msg_win, messageToServer, rowCount++, 0);
            free(messageToServer);
        }

        // exit loop if user enters the exit command
        if (strcmp(buffer, ">>bye<<") == 0)
        {
            end = true;
            break;
        }
        
        //memset(buffer, 0, CHAT_MSG_BUFFER);
        //len = read (myserversocket, buffer, CHAT_MSG_BUFFER);
        /*char* msgReceived = (char*)malloc(sizeof(buffer) + strlen(">> "));
        strcpy(msgReceived, "<< ");
        strcat(msgReceived, buffer);*/
        //rowCount++;
        
        // moves the sent text to the upper chat window
        //displayWindow(msg_win, buffer, rowCount, 0);


        //fflush(stdout);
        //rowCount++;
    }
    

    // join the incoming message thread
    int joinStatus = 0;
    joinStatus = pthread_join(readingThreadID, NULL);


    close(myserversocket);
    printf("[CLIENT] : Client done.\n");
    fflush(stdout);


    sleep(5);

    // destroys the chat window and message window
    delwin(chat_win);
    delwin(msg_win);

    endwin();
    return 0;
}



// command line argument parsing
int parseArguments(int argc, char* firstArg, char* secondArg, struct sockaddr_in* server_address_ptr, 
                    struct hostent** host_ptr, char* userID, char*serverName)
{
    struct hostent *he;
    struct in_addr **addr_list;
    
    char userCmd[USER_LENGTH] = "-user";
    char serverCmd[SERVER_LENGTH] = "-server"; 
    int user_len = strlen(userCmd);
    int server_len = strlen(serverCmd);

    // user name min len validation
    if(strlen(firstArg) <= user_len)
    {
        printf("User name not provided\n"); 
        return 1; 
    }

    // user name max len validation
    if(strlen(firstArg) > user_len + USERNAME_LENGTH)
    {
        printf("User name can be up to 5 characters\n"); 
        return 1; 
    }

    // server name or IP validation
    if(strlen(secondArg) <= server_len)
    {
        printf("Server not provided\n"); 
        return 1; 
    }

    strcpy(userID, firstArg + user_len); 
    strcpy(serverName, secondArg + server_len);  

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
  bzero(word, CHAT_MSG_BUFFER); // sets all the bytes in word to 0
  wmove(win, 1, 1);                       /* position cursor at top */
  keypad(win, TRUE);

  // this will keep getting input from the user until they press enter
  for (i = 0; (ch = wgetch(win)) != '\n'; i++) 
  {
    
    // user has not yet reached the end of 80 character input buffer
    if (i != (CHAT_MSG_BUFFER - ZERO_INDEX_ADJUSTMENT))
    {
        if (isprint(ch) != 0)
        {
            // adds character on the end
            word[i] = ch;                       /* '\n' not copied */
            
        }
        else if (ch == KEY_BACKSPACE)
        {              // replace the end of the line with null term
            word[i-1] = '\0';
            
            
            i--;
            i--;
        }

        // if user enters characters within the first line
        if (col++ < maxcol-2)               /* if within window */
        {
          
            // only display the character if it is visible
            if (isprint(ch) != 0)
            {
              wprintw(win, "%c", word[i]);      /* display the char recv'd */
            }
            else if (ch == KEY_BACKSPACE)
            {

              wmove(win, row, col--);
              wclrtoeol(win);
              wrefresh(win);
              wmove(win, row, col--);
              wdelch(win);
              wrefresh(win);
            }
        }
        // user has written enough characters to extend to second line of chat window
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

    // should only accept a backspace character
    else
    {
        if (ch == KEY_BACKSPACE)
        {
            word[i-1] = '\0';
            
            i--;
            i--;

            wmove(win, row, col-=2);
            wclrtoeol(win);
            wrefresh(win);
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


// creates the message for the server with the delimiters

// structure:
// messagelength|[username]|>>|message
char* composeMessage(char* buffer, int messageLength, char* userID)
{
  char* messageToServer = (char*)malloc(MSG_TO_SERVER_SIZE + NULL_TERMINATION); 
  // 53 chars - messageLength|[user]|>>|message (max 40 chars)


  // add messagelength
  char messageLenStr[3] = "";
  sprintf(messageLenStr, "%d", messageLength);


  strcpy(messageToServer, messageLenStr);
  strcat(messageToServer, "|[");
  strcat(messageToServer, userID);
  strcat(messageToServer, "]|>>|");
  strcat(messageToServer, buffer);


  
  return messageToServer;
}


// splits up longer message into two separate 40 char packets
void makeMessagePackets(char* buffer, int messageLength, char* firstPacket, char* secondPacket)
{

  // check if the last character in the buffer is a space 
  if (buffer[MAX_MSG_PACKET_LENGTH - NULL_TERMINATION] == ' ')
  {
      // copy the partial buffer into the first packet
      strncpy(firstPacket, buffer, MAX_MSG_PACKET_LENGTH - NULL_TERMINATION);
      strcat(firstPacket, "\0");
      

      // copy the rest of the buffer into the second packet
      strncpy(secondPacket, &buffer[MAX_MSG_PACKET_LENGTH], messageLength - MAX_MSG_PACKET_LENGTH - NULL_TERMINATION);
      strcat(secondPacket, "\0");
      
  }
  else
  {
      // find last space before max packet length is reached

      // set lastSpaceIndex to the last possible in the buffer
      int lastSpaceIndex = MAX_MSG_PACKET_LENGTH - ZERO_INDEX_ADJUSTMENT;

      // set the last space index to the last possible space in the string
      for (int i=0; i<MAX_MSG_PACKET_LENGTH; i++)
      {
          if (buffer[i] == ' ')
          {
              lastSpaceIndex = i;
          }
      } 

      
      int maxFirstPacketLength = lastSpaceIndex + ZERO_INDEX_ADJUSTMENT;
      char msgLastSpaceIndex[3] = "";

      sprintf(msgLastSpaceIndex, "%d", maxFirstPacketLength);

      

      // copies the first portion of the string into the first packet
      strncpy(firstPacket, buffer, maxFirstPacketLength);
      strcat(firstPacket, "\0");
      


      strncpy(secondPacket, &buffer[maxFirstPacketLength], messageLength - maxFirstPacketLength);
      strcat(secondPacket, "\0");
      




  }
}
