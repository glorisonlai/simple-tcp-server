CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -std=c99 -g

OBJ_DIR := obj
SRC_DIR := src
BIN_DIR := bin

.PHONY: all server client clean

all: server client 

server: $(BIN_DIR)/server
client: $(BIN_DIR)/client

$(BIN_DIR)/server: $(OBJ_DIR)/server.o $(OBJ_DIR)/clientHandler.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/clientHandler.o: $(SRC_DIR)/clientHandler.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/client: $(SRC_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*


