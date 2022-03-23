#include <time.h>

typedef struct clientInfoStruct {
	int clientID;
	char IPAddress[16];
	char UserName[6];
	struct tm lastTimeUserSentMsg;
	bool isActive;
} CLIENTINFO;