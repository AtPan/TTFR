CC := gcc
CFLAGS_SDL := $(shell pkg-config sdl2 --cflags --libs)
CFLAGS := -std=c99 -iquote ./include/ $(CFLAGS_SDL) -lm
CFLAGS_DEBUG := -O0 -g2 -fsanitize=leak -fstack-protector-all -Wall -Wextra -D__DEBUG__
CFLAGS_RELEASE := -O2

CWD := $(shell pwd)
SRC := $(CWD)/src

MAIN_C := $(SRC)/main.c
C_SRC := $(wildcard $(SRC)/*/*.c) $(MAIN_C)
C_OBJ := $(C_SRC:%.c=%.o)
TARGET := $(CWD)/font


debug: CFLAGS := $(CFLAGS) $(CFLAGS_DEBUG)
debug: $(TARGET)

release: CFLAGS := $(CFLAGS) $(CFLAGS_RELEASE)
release: $(TARGET)

$(TARGET): $(C_OBJ)
	$(CC)  $^ -o $@ $(CFLAGS)

.o: .c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	@rm -f $(C_OBJ) $(TARGET)
