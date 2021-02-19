MIPS ASSEMBLER

CS3421 Assignment 2
Name: Samuel Milner

This is a simple MIPS assembler that can take in a subset of the MIPS ISA and convert it to hexadecimal

Assumptions:
 *  fixed max number of instructions and data words is 32768
 *  max of 80 characters per line
 *  no lables larger than 10 characters
 *  no spaces between commas in between arguments
 *  all lw/sw instructions are in the form label($gp)

 program works by taking in user input, each line should be a line of MIPS assembly
 program does not check if any if the cde is valid, it simply encodes the commands to hex

I'm not entirely sure I implement words properly
i.e
.word 0
.word 0,1,2,3
will both act identically in this program

running valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./assem
gives errors:

 Invalid write of size 4
==1020==    at 0x1093A2: main (assem.c:148)
==1020==  Address 0x1ffecfff5c is on thread 1's stack
==1020==  in frame #0, created by main (assem.c:148)
==1020==
==1020== Invalid write of size 8
==1020==    at 0x1093A8: main (assem.c:148)
==1020==  Address 0x1ffecfff50 is on thread 1's stack
==1020==  in frame #0, created by main (assem.c:148)
==1020==
==1020== Invalid write of size 8
==1020==    at 0x109594: main (assem.c:161)
==1020==  Address 0x1ffecfff48 is on thread 1's stack
==1020==  in frame #0, created by main (assem.c:148)

errors should be from reading out of bound somehwere(?), which would 
suggest undefined/unpredictable behavior, but this hasn't shown up in testing
and I don't know where these out of bound read/writes are at in code

Program has been tested, not thourouhgly though

Late edit:
Right before turning in the assignment I was put into panic when I copied c program to desktop, and it ran differently when testing. 
Suprised it broke when ran from different location, unaware that this was even possible, I quickly tried debuging it, and got it mostly working, although 
saving .data words is broken and as such lw and sw will be slightly off in the immediate encoding. This might not even be a problem on school computers where it is tested, but this is why final version is 15 minutes late