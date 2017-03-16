# Makefile

CC = gcc
PG = ntetrex
SOURCE = main.c
OBJECTS = ntetrex.o
CFLAGS = -Wall -O3
LDLIBS = -lncurses

${PG}: ${OBJECTS}
	${CC} ${CFLAGS} ${LDLIBS} -o ${PG} ${OBJECTS}
	rm ${OBJECTS}

${OBJECTS}: ${SOURCE}
	${CC} ${CFLAGS} -c ${SOURCE} -o ${OBJECTS}

clean:
	rm -f ${PG} ${OBJECTS}
