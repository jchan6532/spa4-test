#target: 
# 	

# link object files into final binary

./bin/chat-client: ./obj/chat-client.o ./obj/ipc-client.o
	cc ./obj/chat-client.o	-lncurses ./obj/ipc-client.o -o ./bin/chat-client

# get the desired file you want to generate an object file
# compile the file from src folder and enter where you want it generated in the directory

./obj/chat-client.o:	./src/chat-client.c ./inc/chat-client.h
	cc -c ./src/chat-client.c -o ./obj/chat-client.o

./obj/ipc-client.o:		./src/ipc-client.c ./inc/chat-client.h
	cc -c ./src/ipc-client.c -o ./obj/ipc-client.o

#
# other targets
#
all: ./bin/chat-client

clean:
	rm ./bin/*
	rm ./obj/*
