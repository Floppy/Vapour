# Counter makefile
# 25/05/2001 - Warren Moore
# worthy enough to be dated now as I've been reading up on make today :o)

# Compiler vars
CC = g++
CFLAGS = -O -I../VAL/Application
#CFLAGS = -O -pedantic -Wall -I$(GD)
LIBS = -lm

# Set the Linux (zim) specific defines for locations, if necessary
ARCH = $(shell uname)
ifeq "$(ARCH)" "Linux"
CFLAGS += -DLINUX
endif

all: counter

# can't be arsed to update this bit...
counter: counter.o gd.o CGI.o
	$(CC) -o counter counter.o gd.o CGI.o

counter.o: counter.cpp
	$(CC) -c $(CFLAGS) counter.cpp

gd.o: gd.cpp mtables.cpp gd.h
	$(CC) -c $(CFLAGS) gd.cpp

CGI.o: ../VAL/Application/CGI.h ../VAL/Application/CGI.cpp
	$(CC) -c $(CFLAGS) ../VAL/Application/CGI.cpp

clean:
	rm -f counter *.o core *~
	
tar: clean
	cd ..; tar -czvf counter.tgz counter/*
	
# Made this detect whether it's on zim or sark
install:
	chmod 755 counter
	cp counter /www/vapournet/cgi-bin
ifneq "$(ARCH)" "Linux"
	chown web.web /www/vapournet/counter
endif
