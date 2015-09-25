CC=g++
PRJ=client
CFLAGS=-g
LIBS=
SRC=client.cpp
OBJ=$(SRC:.cpp=.o)
all:$(PRJ)
$(PRJ):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(PRJ)
$(OBJ):$(SRC)
	$(CC) -c $(SRC)
clean:
	rm -rf $(PRJ) $(OBJ) ../bin/$(PRJ)
install:
	install $(PRJ) ../bin/
