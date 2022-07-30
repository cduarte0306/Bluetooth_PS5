# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o serial_controller.o
	$(CC) $(CFLAGS) -o bluetooth main.o serial_controller.o
 
# The main.o target can be written more simply
 
main.o: main.cpp serial_controller.h
	$(CC) $(CFLAGS) -c main.cpp
 
serial_controller.o: serial_controller.h