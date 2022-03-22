#target: 
# 	

# link object files into final binary

./bin/chat-server: ./obj/chat-server.o ./obj/processMessage.o
	cc ./obj/chat-server.o ./obj/processMessage.o -o ./bin/chat-server

# get the desired file you want to generate an object file
# compile the file from src folder and enter where you want it generated in the directory

./obj/chat-server.o:	./src/chat-server.c ./inc/processMessage.h
	cc -c ./src/chat-server.c -o ./obj/chat-server.o

./obj/processMessage.o: ./src/processMessage.c ./inc/processMessage.h
	cc -c ./src/processMessage.c -o ./obj/processMessage.o


#
# other targets
#
all: ./bin/chat-server

clean:
	rm ./bin/*
	rm ./obj/*
