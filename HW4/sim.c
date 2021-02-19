/*
 *  CS3421 Assignment 4
 *  Name: Samuel Milner
 *
 * simulator of MIPS over a subset of the instruciton set
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union R
{
    unsigned int hex;
    struct
    {
        unsigned int funct : 6;
        unsigned int shamt : 5;
        unsigned int rd : 5;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } RType;
};

union J
{
    unsigned int hex;
    struct
    {
        int immediate : 26;
        unsigned int opcode : 6;
    } JType;
};

union I
{
    unsigned int hex;
    struct
    {
        int immediate : 16;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } IType;
};

//this struct is used for executing
//in first loop and array of this struct will keep data for later
struct Instr
{
    char op[6];
    //value in register
    int rs;
    //name of register
    char rsVal[5];
    int rt;
    char rtVal[5];
    int rd;
    char rdVal[5];
    int immediate;
    // 0 = r, 1 = i, 2 = j
    int type;
};

// Gets name of register based on location
//takes in integer loc, and string reg
void get_reg(unsigned int loc, char *reg)
{
    char num;
    char v[] = "v";
    char a[] = "a";
    char t[] = "t";
    char s[] = "s";
    char k[] = "k";
    if (loc == 0)
    {
        strcpy(reg, "zero");
    }
    else if (loc == 1)
    {
        strcpy(reg, "at");
    }
    else if (loc == 28)
    {
        strcpy(reg, "gp");
    }
    else if (loc == 29)
    {
        strcpy(reg, "sp");
    }
    else if (loc == 30)
    {
        strcpy(reg, "fp");
    }
    else if (loc == 31)
    {
        strcpy(reg, "ra");
    }
    else if (1 < loc && loc < 4)
    {
        num = '0' + abs(2 - loc);
        strncat(v, &num, 1);
        strcpy(reg, v);
    }
    else if (3 < loc && loc < 8)
    {
        num = '0' + abs(4 - loc);
        strncat(a, &num, 1);
        strcpy(reg, a);
    }
    else if (7 < loc && loc < 16)
    {
        num = '0' + abs(8 - loc);
        strncat(t, &num, 1);
        strcpy(reg, t);
    }
    else if (15 < loc && loc < 24)
    {
        num = '0' + abs(16 - loc);
        strncat(s, &num, 1);
        strcpy(reg, s);
    }
    else if (23 < loc && loc < 26)
    {
        num = '0' + (abs(24 - loc) + 8);
        strncat(t, &num, 1);
        strcpy(reg, t);
    }
    else if (25 < loc && loc < 28)
    {
        num = '0' + abs(26 - loc);
        strncat(k, &num, 1);
        strcpy(reg, k);
    }
}

int main(int argc, char **argv)
{
    char testinput[12];
    char *rt;
    char *rs;
    char *rd;
    int maxpc;
    rt = malloc(5 * sizeof(char));
    rs = malloc(5 * sizeof(char));
    rd = malloc(5 * sizeof(char));
    struct Instr *instr;
    instr = malloc(32768 * sizeof(struct Instr));
    int textsize;
    int datasize;
    union R r;
    union J j;
    union I ii;
    FILE *inputf = fopen(argv[1], "r");
    FILE *outputf = fopen("log.txt", "w+");

    fgets(testinput, 12, inputf);
    sscanf(testinput, "%d %d", &textsize, &datasize);

    printf("Text: %d Data: %d\n", textsize, datasize);
    fprintf(outputf, "insts:\n");

    for (int i = 0; i < textsize; i++)
    {
        fgets(testinput, 12, inputf);
        sscanf(testinput, "%x", &(r.hex));
        sscanf(testinput, "%x", &(j.hex));
        sscanf(testinput, "%x", &(ii.hex));
        fprintf(outputf, "%d: ", i);
        maxpc = i;
        if (r.RType.opcode == 0)
        {
            // r type
            instr[i].type = 0;
            get_reg(r.RType.rt, rt);
            get_reg(r.RType.rs, rs);
            get_reg(r.RType.rd, rd);
            //store values for execution later
            strcpy(instr[i].rtVal, rt);
            instr[i].rt = r.RType.rt;
            strcpy(instr[i].rsVal, rs);
            instr[i].rs = r.RType.rs;
            strcpy(instr[i].rdVal, rd);
            instr[i].rd = r.RType.rd;

            if (r.RType.funct == 33)
            {
                //addu
                strcpy(instr[i].op, "addu");
                fprintf(outputf, "addu $%s,$%s,$%s\n", rd, rs, rt);
            }
            else if (r.RType.funct == 36)
            {
                //and
                strcpy(instr[i].op, "and");
                fprintf(outputf, "and $%s,$%s,$%s\n", rd, rs, rt);
            }
            else if (r.RType.funct == 26)
            {
                //div
                strcpy(instr[i].op, "div");
                fprintf(outputf, "div $%s,$%s\n", rs, rt);
            }
            else if (r.RType.funct == 16)
            {
                //mfhi
                strcpy(instr[i].op, "mfhi");
                fprintf(outputf, "mfhi $%s\n", rd);
            }
            else if (r.RType.funct == 18)
            {
                //mflo
                strcpy(instr[i].op, "mflo");
                fprintf(outputf, "mflo $%s\n", rd);
            }
            else if (r.RType.funct == 24)
            {
                //mult
                strcpy(instr[i].op, "mult");
                fprintf(outputf, "mult $%s,$%s\n", rs, rt);
            }
            else if (r.RType.funct == 37)
            {
                //or
                strcpy(instr[i].op, "or");
                fprintf(outputf, "or $%s,$%s,$%s\n", rd, rs, rt);
            }
            else if (r.RType.funct == 42)
            {
                strcpy(instr[i].op, "slt");
                fprintf(outputf, "slt $%s,$%s,$%s\n", rd, rs, rt);
            }
            else if (r.RType.funct == 35)
            {
                //subu
                strcpy(instr[i].op, "subu");
                fprintf(outputf, "subu $%s,$%s,$%s\n", rd, rs, rt);
            }
            else if (r.RType.funct == 12)
            {
                strcpy(instr[i].op, "syscall");
                fprintf(outputf, "syscall\n");
            }
        }
        else if (j.JType.opcode == 2)
        {
            // j type
            instr[i].type = 2;
            instr[i].immediate = j.JType.immediate;
            strcpy(instr[i].op, "j");
            fprintf(outputf, "j %d\n", j.JType.immediate);
        }
        else
        {
            // i type
            get_reg(ii.IType.rt, rt);
            get_reg(ii.IType.rs, rs);

            strcpy(instr[i].rtVal, rt);
            instr[i].rt = r.RType.rt;
            strcpy(instr[i].rsVal, rs);
            instr[i].rs = r.RType.rs;
            instr[i].type = 1;
            instr[i].immediate = ii.IType.immediate;

            if (ii.IType.opcode == 9)
            {
                strcpy(instr[i].op, "addiu");
                fprintf(outputf, "addiu $%s,$%s,%d\n", rt, rs, ii.IType.immediate);
            }
            else if (ii.IType.opcode == 4)
            {
                strcpy(instr[i].op, "beq");
                fprintf(outputf, "beq $%s,$%s,%d\n", rs, rt, ii.IType.immediate);
            }
            else if (ii.IType.opcode == 5)
            {
                strcpy(instr[i].op, "bne");
                fprintf(outputf, "bne $%s,$%s,%d\n", rs, rt, ii.IType.immediate);
            }
            else if (ii.IType.opcode == 35)
            {
                strcpy(instr[i].op, "lw");
                fprintf(outputf, "lw $%s,$%s,%d\n", rs, rt, ii.IType.immediate);
            }
            else if (ii.IType.opcode == 43)
            {
                strcpy(instr[i].op, "sw");
                fprintf(outputf, "sw $%s,$%s,%d\n", rs, rt, ii.IType.immediate);
            }
            else
            {
                printf("ERROR: Instruction not found");
                free(rs);
                free(rd);
                free(rt);
                free(instr);
                fclose(outputf);
                return 0;
            }
        }
    }
    fclose(inputf);

    int *data;
    int dataval;
    data = malloc(datasize * sizeof(int));
    for (int i = 0; i < datasize; i++)
    {
        fgets(testinput, 12, inputf);
        sscanf(testinput, "%x", &dataval);
        data[i] = dataval;
    }

    //execution
    int regs[32] = {0};
    regs[28] = maxpc + 1;
    int hi = 0;
    int lo = 0;
    unsigned int pc = 0;
    while (1 == 1)
    {
        //1. get instruction from instructopns [pc]
        //2. execute ruction
        if (pc > maxpc)
        {
            printf("ERROR: Invalid program counter");
            free(rs);
            free(rd);
            free(rt);
            free(instr);
            free(data);
            fclose(outputf);
            return 0;
        }
        if (strcmp(instr[pc].op, "addu\0") == 0)
        {
            if (instr[pc].rd != 0)
            {
                regs[instr[pc].rd] = regs[instr[pc].rt] + regs[instr[pc].rs];
            }
        }
        else if (strcmp(instr[pc].op, "and\0") == 0)
        {
            if (instr[pc].rd != 0)
            {
                regs[instr[pc].rd] = regs[instr[pc].rt] & regs[instr[pc].rs];
            }
        }
        else if (strcmp(instr[pc].op, "div\0") == 0)
        {
            if (regs[instr[pc].rt] == 0)
            {
                printf("ERROR: Divide by 0");
                free(rs);
                free(rd);
                free(rt);
                free(instr);
                free(data);
                fclose(outputf);
                return 0;
            }
            lo = regs[instr[pc].rs] / regs[instr[pc].rt];
            regs[instr[pc].rs] % regs[instr[pc].rt];
        }
        else if (strcmp(instr[pc].op, "mhfi\0") == 0)
        {
            if (instr[pc].rd != 0)
            {
                regs[instr[pc].rd] = hi;
            }
        }
        else if (strcmp(instr[pc].op, "mflo\0") == 0)
        {
            if (instr[pc].rd != 0)
            {
                regs[instr[pc].rd] = lo;
            }
        }
        else if (strcmp(instr[pc].op, "mult\0") == 0)
        {
            hi = regs[instr[pc].rt] * regs[instr[pc].rs];
        }
        else if (strcmp(instr[pc].op, "or\0") == 0)
        {
            regs[instr[pc].rd] = regs[instr[pc].rt] | regs[instr[pc].rs];
        }
        else if (strcmp(instr[pc].op, "slt\0") == 0)
        {
            if (regs[instr[pc].rs] < regs[instr[pc].rt])
                regs[instr[pc].rd] = 1;
            else
                regs[instr[pc].rd] = 0;
        }
        else if (strcmp(instr[pc].op, "subu\0") == 0)
        {
            if (instr[pc].rd != 0)
            {
                regs[instr[pc].rd] = regs[instr[pc].rs] - regs[instr[pc].rt];
            }
        }
        else if (strcmp(instr[pc].op, "syscall\0") == 0)
        {
            //syscall
            if (regs[2] == 1)
            {
                // print integer to screen
                printf("%d\n", regs[4]);
            }
            else if (regs[2] == 5)
            {
                // read input from user
                int temp;
                printf("enter integer\n");
                scanf("%d", &temp);
                regs[2] = temp;
            }
            else if (regs[2] == 10)
            {
                fprintf(outputf, "PC: %d\ninst: syscall\n", pc);
                fprintf(outputf, "exiting simulator\n");
                free(rs);
                free(rd);
                free(rt);
                free(instr);
                free(data);
                fclose(outputf);
                return 0;
            }
        }
        else if (strcmp(instr[pc].op, "addiu\0") == 0)
        {
            if (instr[pc].rt != 0)
            {
                regs[instr[pc].rt] = regs[instr[pc].rs] + instr[pc].immediate;
            }
        }
        else if (strcmp(instr[pc].op, "lw\0") == 0)
        {
            if (regs[instr[pc].rs] < regs[28])
            {
                printf("ERROR: Invalid Reference");
                free(rs);
                free(rd);
                free(rt);
                free(instr);
                free(data);
                fclose(outputf);
                return 0;
            }
            regs[instr[pc].rt] = data[(regs[28] - regs[instr[pc].rs]) + instr[pc].immediate];
        }
        else if (strcmp(instr[pc].op, "sw\0") == 0)
        {
            if (regs[instr[pc].rs] < regs[28])
            {
                printf("ERROR: Invalid Reference");
                free(rs);
                free(rd);
                free(rt);
                free(instr);
                free(data);
                fclose(outputf);
                return 0;
            }
            data[(regs[28] - regs[instr[pc].rs]) + instr[pc].immediate] = regs[2];
        }

        //Start printing into file
        fprintf(outputf, "\n\nPC: %d\n", pc);
        if (instr[pc].type == 0)
        {
            fprintf(outputf, "inst: %s\t$%s,$%s,$%s\n", instr[pc].op, instr[pc].rdVal, instr[pc].rsVal, instr[pc].rtVal);
        }
        else if (instr[pc].type == 1)
        {
            if (strcmp(instr[pc].op, "sw") == 0 || strcmp(instr[pc].op, "lw") == 0)
            {
                fprintf(outputf, "inst: %s\t$%s,%d($%s)\n", instr[pc].op, instr[pc].rtVal, instr[pc].immediate, instr[pc].rsVal);
            }
            else if (strcmp(instr[pc].op, "beq") == 0 || strcmp(instr[pc].op, "bne") == 0)
            {
                fprintf(outputf, "inst: %s\t$%s,$%s,%d\n", instr[pc].op, instr[pc].rsVal, instr[pc].rtVal, instr[pc].immediate);
            }
            else
                fprintf(outputf, "inst: %s\t$%s,$%s,%d\n", instr[pc].op, instr[pc].rtVal, instr[pc].rsVal, instr[pc].immediate);
        }
        else
        {
            fprintf(outputf, "inst: j\t%d\n", instr[pc].immediate);
        }
        fprintf(outputf, "regs:\n");
        fprintf(outputf, "\t$zero =\t%d\t $at =\t%d\t $v0 =\t%d\t $v1 =\t%d\n", regs[0], regs[1], regs[2], regs[3]);
        fprintf(outputf, "\t$a0 =\t%d\t $a1 = \t%d\t $a2 =\t%d\t $a3 =\t%d\n", regs[4], regs[5], regs[6], regs[7]);
        fprintf(outputf, "\t$t0 =\t%d\t $t1 =\t%d\t $t2 =\t%d\t $t3 =\t%d\n", regs[8], regs[9], regs[10], regs[11]);
        fprintf(outputf, "\t$t4 =\t%d\t $t5 =\t%d\t $t6 =\t%d\t $t7 =\t%d\n", regs[12], regs[13], regs[14], regs[15]);
        fprintf(outputf, "\t$s0 =\t%d\t $s1 =\t%d\t $s2 =\t%d \t $s3 =\t%d\n", regs[16], regs[17], regs[18], regs[19]);
        fprintf(outputf, "\t$s4 =\t%d\t $s5 =\t%d\t $s6 =\t%d \t$s7 =\t%d\n", regs[20], regs[21], regs[22], regs[23]);
        fprintf(outputf, "\t$t8 =\t%d\t $t9 =\t%d\t $k0 =\t%d \t$k1 =\t%d\n", regs[24], regs[25], regs[26], regs[27]);
        fprintf(outputf, "\t$gp =\t%d\t $sp =\t%d\t $fp =\t%d \t$ra =\t%d\n", regs[28], regs[29], regs[30], regs[31]);
        fprintf(outputf, "\t$lo =\t%d\t $hi =\t%d\n\n", lo, hi);
        for (int i = 0; i < datasize; i++)
        {
            fprintf(outputf, "data memory:\n\tdata[ %d] = %d\n", i, data[i]);
        }

        // j, beq and bne are after printing to file so pc isnt changed before we display whats happening
        if (strcmp(instr[pc].op, "j\0") == 0)
        {
            if (instr[pc].immediate < maxpc)
            {
                pc = instr[pc].immediate;
            }
            else
                pc++;
        }
        else if (strcmp(instr[pc].op, "beq\0") == 0)
        {
            if (regs[instr[pc].rs] == regs[instr[pc].rt] && instr[pc].immediate < maxpc)
            {
                pc = pc + instr[pc].immediate;
            }
            else
                pc++;
        }
        else if (strcmp(instr[pc].op, "bne\0") == 0)
        {
            if (regs[instr[pc].rs] != regs[instr[pc].rt] && instr[pc].immediate < maxpc)
            {
                pc = pc + instr[pc].immediate;
            }
            else
                pc++;
        }
        else
            pc++;
    }

    free(rs);
    free(rd);
    free(rt);
    free(instr);
    free(data);
    fclose(outputf);
    return 0;
}
