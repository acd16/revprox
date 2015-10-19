SRC = $(wildcard *.cpp)
CC = g++
CFLAGS = -std=c++11 -pthread -g
prox: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY :clean

clean :
	rm -f prox 
