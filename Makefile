SRC := src
OBJ := obj
BIN := bin
EXECUTABLE:= shell
MYTIMEOUT := mytimeout

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
INCS := -Iinclude/
DIRS := $(OBJ)/ $(BIN)/
EXEC := $(BIN)/$(EXECUTABLE)
MYT := $(BIN)/$(MYTIMEOUT)

CC := gcc
CFLAGS := -g -Wall -std=c99 $(INCS)
LDFLAGS :=

all: $(EXEC) $(MYT)

$(MYT): obj/mytimeout.o obj/lexer.o
	$(CC) $(CFLAGS) -o $(MYT) $^

$(EXEC): obj/main.o obj/lexer.o obj/piping.o
	$(CC) $(CFLAGS) -o $(EXEC) $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	$(EXEC)

clean:
	rm $(OBJ)/*.o $(EXEC) $(MYT)

$(shell mkdir -p $(DIRS))

.PHONY: run clean all
