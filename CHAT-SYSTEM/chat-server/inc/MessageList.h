typedef struct MessageListStruct{
	char Message[81];
	int ClientIDMsgBelongsTo;
	MESSAGELIST* next;
} MESSAGELIST;