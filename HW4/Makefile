CC=gcc
CFLAGS=-g
SUBMIT=sim
SUBMITC=$(SUBMIT:=.c)

all: $(SUBMIT) 

%: %.c
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean package

package: $(SUBMITC)
	tar -cvzf hw4.tgz $(SUBMITC) readme.txt Makefile

clean:
	rm -f $(SUBMIT) hw4.tgz
