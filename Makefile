CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm
LDFLAGS = `sdl2-config --cflags --libs`

BUILD_DIR = build

SRCS = main.c create_project.c settings.c window.c ui.c
OBJS = $(SRCS: %.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/Project_Creator

all:  $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %. c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
