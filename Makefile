CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm
LDFLAGS = `sdl2-config --cflags --libs`

SRCS = main.c create_project.c settings.c window.c ui.c 
OBJS = $(SRCS:.c=.o)

TARGET = Project_Creator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
