#
#	Makefile for Vigenere program
#

OBJS = main.o  vigenere.o   
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)

main : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

main.o : main.cpp vigenere.h 				
	$(CC) $(CFLAGS) main.cpp

vigenere.o : vigenere.cpp vigenere.h 		
	$(CC) $(CFLAGS) vigenere.cpp

clean:
	rm *.o
	
	
