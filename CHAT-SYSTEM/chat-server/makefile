#target: 
# 	

# link object files into final binary

./bin/chat-server: ./obj/chat-server.o 
	cc ./obj/chat-server.o -o ./bin/chat-server -lpthread

# get the desired file you want to generate an object file
# compile the file from src folder and enter where you want it generated in the directory

./obj/chat-server.o:	./src/chat-server.c 
	cc -c ./src/chat-server.c -o ./obj/chat-server.o



#
# other targets
#
all: ./bin/chat-server

clean:
	rm ./bin/*
	rm ./obj/*
