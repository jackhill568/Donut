CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -Iinclude -Ivendor/linmath.h 
LDFLAGS = -lm
BUILD   = build
TARGET  = main
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, $(BUILD)/%.o, $(SRC))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(TARGET)
