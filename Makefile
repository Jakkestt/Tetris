CC		:= gcc
CFLAGS 	:=
LDFLAGS := -lSDL2 -isystem/usr/local/bin

SRC		:= ./src/
BUILD	:= ./build/
TARGETS := ${BUILD}Tetris
OBJ		:= $(addprefix ${SRC}, main.o window.o)
DEPS	:= 

.PHONY: all clean

all: ${TARGETS}
	${TARGETS}

$(TARGETS):$(OBJ) | $(BUILD)
	${CC} ${CFLAGS} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f $(BUILD)* $(SRC)*.o

$(SRC).c $(SRC).o:
	${CC} ${CFLAGS} -c $*.c

$(BUILD):
	mkdir $@