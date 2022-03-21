/* FILE :           chat-client.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include <stdio.h>
#include <string.h>
#include <ncurses.h>

int main(int argc, char* argv)
{

    // arguments [userID] [server name, as ip address or as the name of server e.g. ubuntu]

    // one thread for outgoing messages and one thread for handling incoming message
    initscr();
    endwin();
    return 0;
}
