
CC = gcc
CFLAGS = -Iinclude -Ilib -Isrc           # Header dir

# lib/tempLib.a: src/tempLib.o
# 	ar rcs $@ $^


# src/%.o: src/%.c
# 	$(CC) $(CFLAGS) -c -o $@ $<



OPTIONS = -framework GLUT -framework OpenGL
DEFINES = -D GL_SILENCE_DEPRECATION

MAIN_FILE ?= src/main.c


TARGET = main
# Source codes
SRCS = $(filter-out src/main%.c, $(wildcard src/**/*.c src/*.c)) # all .c under src exclude main.c
OBJS = $(SRCS:.c=.o)  # replace .c by .o
POSSIBLE_MAIN = src/main.o 

# Compile rules
$(TARGET): $(OBJS) $(MAIN_FILE:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS) $(OPTIONS) $(DEFINES)

# from .c to .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(DEFINES)


clean:
	rm -f $(TARGET) $(OBJS) $(POSSIBLE_MAIN)