CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

INCLUDE_DIR = ../include

CRC_DIR = $(INCLUDE_DIR)/crc8
CRC_SRC = $(CRC_DIR)/crc8.c

TEST_DIR = .
TEST_CRC_SRC = $(TEST_DIR)/crc8-test.c

INCLUDES_CRC = -I$(CRC_DIR)

CRC8: 
	$(CC) $(CFLAGS) $(INCLUDES_CRC) $(TEST_CRC_SRC) $(CRC_SRC) -o test-crc8

all: CRC8

clean: 
	rm -f test-crc8

.PHONY: all CRC8 clean
