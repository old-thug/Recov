
SRC=src
BIN=bin
INC=include
LIB=lib

LIB_RECOV_SOURCES=$(wildcard $(LIB)/recov/*.c)
LIB_RECOV_OBJECTS=$(patsubst $(LIB)/recov/%.c, $(BIN)/%.obj, $(LIB_RECOV_SOURCES))
LIB_RECOV=$(BIN)/librecov.a

LIB_UI_SOURCES=$(wildcard $(LIB)/ui/*.c)
LIB_UI_OBJECTS=$(patsubst $(LIB)/ui/%.c, $(BIN)/%.obj, $(LIB_UI_SOURCES))
LIB_UI=$(BIN)/libui.a

CLI_SOURCES=$(wildcard $(SRC)/cli/*.c)
CLI_OBJECTS=$(patsubst $(SRC)/cli/%.c, $(BIN)/cli/%.obj, $(CLI_SOURCES))
CLI_APP=recov_cmd.exe

GUI_SOURCES=$(wildcard $(SRC)/gui/*.c)
GUI_OBJECTS=$(patsubst $(SRC)/gui/%.c, $(BIN)/gui/%.obj, $(GUI_SOURCES))

TARGET=recov.exe
all=$(TARGET)

FLAGS=-Wall -Wextra -Iinclude -ggdb
LD_FLAGS=-lrecov -L. -Lbin

$(TARGET): $(GUI_OBJECTS) $(LIB_RECOV) $(LIB_UI) $(CLI_APP)
	gcc $(GUI_OBJECTS) $(LD_FLAGS) -lSDL3 -lSDL3_ttf -lui -o $@

$(BIN)/cli/%.obj: $(SRC)/cli/%.c
	gcc $(FLAGS) $< -o $@ -c

$(BIN)/gui/%.obj: $(SRC)/gui/%.c
	gcc $(FLAGS) $< -o $@ -c

$(LIB_RECOV): $(LIB_RECOV_OBJECTS)
	ar rcs $@ $<

$(BIN)/%.obj: $(LIB)/recov/%.c
	gcc $(FLAGS) $< -o $@ -c

$(BIN)/%.obj: $(LIB)/ui/%.c
	gcc $(FLAGS) $< -o $@ -c

$(LIB_UI): $(LIB_UI_OBJECTS)
	ar rcs $@ $^

$(CLI_APP): $(CLI_OBJECTS) $(LIB_RECOV)
	gcc $(CLI_OBJECTS) $(LD_FLAGS) -o $@
