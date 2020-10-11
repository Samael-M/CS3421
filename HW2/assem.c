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
#include <ctype.h>

typedef enum{ false, true} bool;

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 10

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
} ii;

union
{
    unsigned int x;
    struct
    {
        unsigned int immediate : 26;
        unsigned int opcode : 6;
    } JType;
} j;

typedef struct {
    char name[10];
    int location;
} Labels;

/* 
*  Parameter: string representing register name
*  Return: integer representing location in memory of register
*/
int find_register_location(char *reg)
{
    if(strcmp(reg, "zero") == 0)
    {
        return 0;
    }
    else if (strcmp(reg, "at") == 0)
    {
        return 1;
    }
    else if (strcmp(reg, "gp") == 0)
    {
        return 28;
    }
    else if (strcmp(reg, "sp") == 0)
    {
        return 29;
    }
    else if (strcmp(reg, "fp") == 0)
    {
        return 30;
    }
    else if (strcmp(reg, "ra") == 0)
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
    else if (reg[0] == 'a')
    {
        int num = reg[1] - '0';
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
    else if (reg[0] == 'k')
    {
        int num = reg[2] - '0';
        return num + 26;
    }
}


int main(int argc, char **argv)
{
    bool data = false;

    char file[32768][80];
    char directive[5][10], line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], label[10], junk[10];
    int lineNum = 0;
    int data_words = 0; 
    int instructions = 0; 
    int imm = 0;
    int labelCount = 0;

    Labels *labelNames;
    labelNames = (Labels *) malloc(32768 * sizeof(Labels));

    while (fgets(line, MAXLINE, stdin))
    {
        
        if (strcmp(".data\n", line) == 0)
        {
            data = true;
        }
        else if (line[0] != '#' && data == true)
        {
            strcpy(directive[data_words], line);
            data_words++;
        }

        char label[11];
        char str[80];
        //get rid of label from line and store it for later use, along with adress
        sscanf(line, "%11s", label);
        if(label[strlen(label) - 1] == ':') {
            sscanf(line, "%10[^:]: %[^#]", label, str);
            strcpy(line, str);

            strcpy(labelNames[labelCount].name, label);
            labelNames[labelCount].location = lineNum;
            labelCount++;
        }

        if (line[0] == '#' || strcmp(".text\n", line) == 0) { /*skip line*/ }
        else if (line[0] != '#' && data == false)
        {
            instructions++;
            strcpy(file[lineNum], line);
            lineNum++;
        }
    }
    printf("%d %d\n", instructions, data_words);

    for (int i = 0; i < lineNum; i++) {
        /* First three checks, check for R type commands based on how many argumetns they take */
        if (sscanf(file[i], "%10s $%5[^,],$%5[^,],$%5s", oper, rd, rs, rt) == 4) {
            if (strcmp("addu", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x21;
            }
            else if (strcmp("and", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x24;
               // printf("op: %s -> %x, rs: %s -> %x, rt: %s -> %x, rd: %s -> %x, shamt: %x, funct: %x\n",
                    //oper, r.RType.opcode, rs, r.RType.rs, rt, r.RType.rt, rd, r.RType.rd, r.RType.shamt, r.RType.funct);
            }
            else if (strcmp("or", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x25;
               // printf("op: %s -> %x, rs: %s -> %x, rt: %s -> %x, shamt: %x, funct: %x\n",
                 //oper, r.RType.opcode, rs, r.RType.rs, rt, r.RType.rt, r.RType.shamt, r.RType.funct);
            }
            else if (strcmp("slt", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x2a;
            }
            else if (strcmp("subu", oper) == 0) {
                r.RType.opcode = 0x0;
                r.RType.rs = find_register_location(rs);
                r.RType.rt = find_register_location(rt);
                r.RType.rd = find_register_location(rd);
                r.RType.shamt = 0x0;
                r.RType.funct = 0x23;
            }
            printf("%08x\n", r.x);
        }
        else if (sscanf(file[i], "%10s $%5[^,],$%5[^,]%s", oper, rs, rt, junk) == 3) {
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
            }
            else {
                printf("Error1, command not recognized: %s\n", oper);
            } //not recognized command
            printf("%08x\n", r.x);
        }
        else if (sscanf(file[i], "%10s $%5[^,]%s", oper, rd, junk) == 2) {
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
                printf("Error2, command not recognized: %s\n", oper);
            } //not recognized command
            printf("%08x\n", r.x);
        }
        /* Next set of if checks go over I and J type instructions. I check each instruction independantly */
        else if(strcmp(oper, "j") == 0) {
            sscanf(file[i], "%s %10s[^#]", oper, label);
            j.JType.opcode = 0x2;
            for(int k = 0 ; k < labelCount; k++) {
                if(strcmp(labelNames[k].name, label) == 0) {
                    j.JType.immediate = labelNames[k].location;
                }
            }
            printf("%08x\n", j.x);
        }
        else if(strcmp(oper, "bne") == 0) {
            sscanf(file[i], "%10s $%5[^,],$%5[^,],%5s", oper, rs, rt, label);
            ii.IType.opcode = 0x5;
            ii.IType.rs = find_register_location(rs);
            ii.IType.rt = find_register_location(rt);
           
            for(int j = 0 ; j < labelCount; j++) {
                if(strcmp(labelNames[j].name, label) == 0) {
                    ii.IType.immediate = labelNames[j].location - i;
                }
            }
            printf("%08x\n", ii.x);
        }
        else if(strcmp(oper, "beq") == 0) {
            sscanf(file[i], "%10s $%5[^,],$%5[^,],%5s", oper, rs, rt, label);
            ii.IType.opcode = 0x4;
            ii.IType.rs = find_register_location(rs);
            ii.IType.rt = find_register_location(rt);
           
            for(int j = 0 ; j < labelCount; j++) {
                if(strcmp(labelNames[j].name, label) == 0) {
                    ii.IType.immediate = labelNames[j].location - i;
                }
            }
            printf("%08x\n", ii.x);
        }
        else if(strcmp(oper, "lw") == 0) {
            sscanf(file[i], "%10s $%5[^,],%10[^(]($%[^)]", oper, rt, label, rs);
            ii.IType.opcode = 0x23;
            ii.IType.rs = find_register_location(rs);
            ii.IType.rt = find_register_location(rt);
            if(isdigit(label[0]) != 0) { ii.IType.immediate = atoi(label); }
            else {
                for(int j = 0 ; j < data_words; j++) {
                    char temp1[10];
                    char temp2[5];
                    char temp3[5];
                    sscanf(directive[j], "%5[^:]: .%5s %s", temp1, temp2, temp3);
                    if(strcmp(temp1, label) == 0) {
                        ii.IType.immediate = atoi(temp3);
                    }
                }
            }
            
            printf("%08x\n", ii.x);
        }
        else if(strcmp(oper, "sw") == 0) {
            sscanf(file[i], "%10s $%5[^,],%10[^(]($%[^)]", oper, rt, label, rs);
            ii.IType.opcode = 0x2b;
            ii.IType.rs = find_register_location(rs);
            ii.IType.rt = find_register_location(rt);
            if(isdigit(label[0]) != 0) { ii.IType.immediate = atoi(label); }
            else {
                for(int j = 0 ; j < data_words; j++) {
                    char temp1[10];
                    char temp2[5];
                    char temp3[5];
                    sscanf(directive[j], "%5[^:]: .%5s %s", temp1, temp2, temp3);
                    if(strcmp(temp1, label) == 0) {
                        ii.IType.immediate = atoi(temp3);
                    }
                }
            }
            printf("%x\n", ii.x);
        }
        else if(strcmp(oper, "addiu") == 0) {
            sscanf(file[i], "%10s $%5[^,],$%5[^,],%s", oper, rt, rs, junk);
            ii.IType.opcode = 0x9;
            ii.IType.rs = find_register_location(rs);
            ii.IType.rt = find_register_location(rt);
            ii.IType.immediate = atoi(junk);
            printf("%x\n", ii.x);
        }
        else if(strcmp(oper, "syscall") == 0) {
            printf("0000000c\n");
        }
        else {
            printf("Error4, not recognized command: %s\n", oper); //not recognized command
            return 0;
        }
    }
    free(labelNames);
    return 0;
}