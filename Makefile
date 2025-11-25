
SRC=src
BIN=bin
INC=include
LIB=lib

LIB_RECOV_SOURCES=$(wildcard $(LIB)/*.c)
LIB_RECOV_OBJECTS=$(patsubst $(LIB)/%.c, $(BIN)/%.obj, $(LIB_RECOV_SOURCES))
LIB_RECOV=$(BIN)/librecov.a

SOURCES=$(wildcard $(SRC)/*.c)
OBJECTS=$(patsubst $(SRC)/%.c, $(BIN)/%.obj, $(SOURCES))

TARGET=recov.exe
all=$(TARGET)

FLAGS=-Wall -Wextra -Iinclude -ggdb
LD_FLAGS=-lSDL3 -lrecov -L. -Lbin

$(TARGET): $(OBJECTS) $(LIB_RECOV)
	gcc $(OBJECTS) $(LD_FLAGS) -o $@

$(BIN)/%.obj: $(SRC)/%.c
	gcc $(FLAGS) $< -o $@ -c

$(LIB_RECOV): $(LIB_RECOV_OBJECTS)
	ar rcs $@ $<

$(BIN)/%.obj: $(LIB)/%.c
	gcc $(FLAGS) $< -o $@ -c

