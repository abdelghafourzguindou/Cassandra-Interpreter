###########################################################
## Makefile for cassandra program
## writed by ZGUINDOU Abdelghafour
## Thanks for https://www.youtube.com/watch?v=Wt6KGlMkLz0
###########################################################

TARGET = Cassandra

SRC = src
LIB = lib
BIN = bin

SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.c=.o)))

CC = gcc
DD = gdb
CFLAGS = -Wall -std=gnu99 -I$(LIB)

RED 	= \033[1;31m
GREEN 	= \033[1;32m
BLUE 	= \033[1;34m
YELLOW 	= \033[1;33m
NC      = \033[1;0m

$(BIN)/$(TARGET) : $(OBJECT)
	@echo "$(RED)Linking...$(NC)"
	$(CC) -o $@ $^ -ljansson
	@echo "$(BLUE)Finished!$(NC)"

$(BIN)/%.o : $(SRC)/%.c
	@echo "$(GREEN)Compiling...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : help run clean

run : $(BIN)/$(TARGET)
	@echo "$(YELLOW)Running...$(NC)"
	$(BIN)/$(TARGET)

debug :
	$(DD) $(TARGET)

clean :
	rm -f $(OBJECT) $(BIN)/$(TARGET)

help :
	@echo "src : $(SOURCE)"
	@echo "obj : $(OBJECT)"
