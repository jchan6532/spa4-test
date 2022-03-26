typedef struct MessageListStruct{
	char Message[81];
	int ClientIDMsgBelongsTo;
	struct MessageListStruct* next;
} MESSAGELIST;