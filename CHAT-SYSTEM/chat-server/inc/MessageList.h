typedef struct MessageListStruct{
	char Message[81];
	char SenderIP[16];
	int arrowIndex;
	struct MessageListStruct* next;
} MESSAGELIST;