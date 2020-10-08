/*
 *  CS3421 Assignment 2
 *  Name: Samuel Milner
 *
 *  Simple MIPS assembler for subset of its instruction set
 *  Displays number of isntructions and number of words 
 * Then displays each MIPS instruction represented in hexidecimal
 * 
 *  Assumptions:
 *  fixed max number of instructions and data words is 32768
 *  max of 80 characters per line
 *  no lables larger than 10 characters
 *  no spaces between commas in between arguments
 *  all lw/sw instructions are in the form label($gp)
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum{ false, true} bool;

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 10

int instructions = 0;
int data_words = 0;
int current_address = 0;
bool data = false;
char file[32768][80];
int labels[32768];

union
{
    unsigned int x;
    struct
    {
        unsigned int funct : 6;
        unsigned int shamt : 5;
        unsigned int rd : 5;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;

    } RType;
} r;

union
{
    unsigned int x;
    struct
    {
        unsigned int immediate : 16;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } IType;
} i;

union
{
    unsigned int x;
    struct
    {
        unsigned int immediate : 26;
        unsigned int opcode : 6;
    } JType;
} j;

/* 
*  Parameter: string representing line of MIPS code
*  Return: Same string without the label's to make rest of program easier
*/
char *skiplabel(char *line)
{
    char label[10];
    char str[80];
    //str = malloc(100);
    if(sscanf(line, "%10[^:]: %25[^#] ", label, str) == 2) {
        strcpy(line, str);
        //free(str);
        printf("Line: %s !", line);
        return line;
    }
    //free(str);
    printf("Line: %s !", line);
    return line;
}

/* 
*  Parameter: string representing register name
*  Return: integer representing location in memory of register
*/
int find_register_location(char *reg)
{

    if (strcmp(reg, "$at") == 0)
    {
        return 1;
    }
    else if (strcmp(reg, "$gp") == 0)
    {
        return 28;
    }
    else if (strcmp(reg, "$sp") == 0)
    {
        return 29;
    }
    else if (strcmp(reg, "$fp") == 0)
    {
        return 30;
    }
    else if (strcmp(reg, "$ra") == 0)
    {
        return 31;
    }

    else if (reg[1] == 'f')
    {
        int num = reg[2] - '0';
        return num;
    }
    else if (reg[0] == 'v')
    {
        int num = reg[1] - '0';
        return num + 2;
    }
    else if (reg[1] == 'a')
    {
        int num = reg[2] - '0';
        return num + 4;
    }
    else if (reg[0] == 't')
    {
        int num = reg[1] - '0';
        if (num < 8)
        {
            return num + 8;
        }
        else if (num == 8)
        {
            return 24;
        }
        else
            return 25;
    }
    else if (reg[0] == 's')
    {
        int num = reg[1] - '0';
        return num + 16;
    }
    else if (reg[1] == 'k')
    {
        int num = reg[2] - '0';
        return num + 26;
    }
}

int main(int argc, char **argv)
{

    char directive[5], line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], *s;
    int lineNum = 0;
    while (fgets(line, MAXLINE, stdin))
    {
        if (line[0] == '#' || strcmp(".text\n", line) == 0)
        { /*skip line*/
        }
        else if (strcmp(".data\n", line) == 0)
        {
            data = true;
            strcpy(file[lineNum], line);
            lineNum++;
        }
        else if (line[0] != '#' && data == false)
        {
            instructions++;
            strcpy(file[lineNum], line);
            lineNum++;
        }
        else if (line[0] != '#' && data == true)
        {
            data_words++;
            strcpy(file[lineNum], line);
            lineNum++;
        }
        current_address +=  4;
    }
    printf("%d %d\n", instructions, data_words);

    //while(fgets(line, MAXLINE, input)) {
    for (int i = 0; i < lineNum; i++) {
        s = skiplabel(file[i]);
        printf("s: %s !", s);
        if (strcmp(file[i], ".data\n") == 0) { return 0; }

        else if (sscanf(s, "%10s $%5[^,],$%5[^,],$%5s", oper, rd, rs, rt) == 4) {
        }
        else if (sscanf(s, "%10s $%5[^,],$%5s", oper, rs, rt) == 3) {
            if (strcmp("mult", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = 0x0;
                r.RType.shamt = 0x0;
                r.RType.funct = 0x18;
            }
            else if (strcmp("div", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = 0x0;
                r.RType.shamt = 0x0;
                r.RType.funct = 0x1a;
                printf("op: %s -> %x, rs: %s -> %x, rt: %s -> %x, shamt: %x, funct: %x\n",
                 oper, r.RType.opcode, rs, r.RType.rs, rt, r.RType.rt, r.RType.shamt, r.RType.funct);
            }
            else {
                printf("Error2, command not recognized: %s\n", oper);
                return 0;
            } //not recognized command
        }
        else if (sscanf(s, "%10s $%5[^,]", oper, rd) == 2) {
            if (strcmp("mfhi", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = 0x0;
                r.RType.rt = 0x0;
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x10;
            }
            else if (strcmp("mflo", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = 0x0;
                r.RType.rt = 0x0;
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x12;
            }
            else {
                printf("Error1, command not recognized: %s\n", oper);
                return 0;
            } //not recognized command
        }
        else if (sscanf(s, "%10[^#]]", oper) == 2) {
        
        }
        else {
            printf("Error3, not recognized command: %s\n", file[i]); //not recognized command
            return 0;
        }
        printf("%08x\n", r.x);
    }
    return 0;
}