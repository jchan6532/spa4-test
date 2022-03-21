#target: 
# 	

# link object files into final binary

./bin/chat-client: ./obj/chat-client.o 
	cc ./obj/chat-client.o	-lncurses -o ./bin/chat-client

# get the desired file you want to generate an object file
# compile the file from src folder and enter where you want it generated in the directory

./obj/chat-client.o:	./src/chat-client.c 
	cc -c ./src/chat-client.c -o ./obj/chat-client.o

#
# other targets
#
all: ./bin/chat-client

clean:
	rm ./bin/*
	rm ./obj/*
