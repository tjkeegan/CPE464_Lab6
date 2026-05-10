# udpCode makefile
# written by Hugh Smith - Feb 2021
# Modified to place object files in build/ and executables in bin/

CC = gcc
CFLAGS = -g -Wall -std=gnu99

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

COMMON_SRC = networks.c gethostbyname.c safeUtil.c pollLib.c
COMMON_OBJS = $(COMMON_SRC:%.c=$(BUILD_DIR)/%.o)

TARGETS = $(BIN_DIR)/udpClient $(BIN_DIR)/udpServer

# uncomment next two lines if you're using sendtoErr() library
# LIBS += libcpe464.2.21.a -lstdc++ -ldl
# CFLAGS += -D__LIBCPE464_

.PHONY: all clean cleano dirs

all: dirs $(TARGETS)

dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(BIN_DIR)/udpClient: $(SRC_DIR)/udpClient.c $(COMMON_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON_OBJS) $(LIBS)

$(BIN_DIR)/udpServer: $(SRC_DIR)/udpServer.c $(COMMON_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON_OBJS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/*.h | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

cleano:
	rm -rf $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
