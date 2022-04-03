typedef struct MessageListStruct{
	char Message[53];
	int ClientIDMsgBelongsTo;
	struct MessageListStruct* next;
} MESSAGELIST;