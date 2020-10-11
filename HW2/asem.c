#include <stdio.h>
#include <string.h>

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 10

char *skiplabel(char *line) {
    char label[11];
    char str[80];
    sscanf(line, "%11s", label);
    if(label[strlen(label) - 1] == ':') {
        sscanf(line, "%11s %[^#]", label, str);
        strcpy(line, str);
    }
    printf("label: %s, str: %s\n", label, str);
    return line;
}
int main(int argc, char **argv){
    char directive[5], line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], *s, file[5][80];
    int instructions = 0;
    while (fgets(line, MAXLINE, stdin)) {
        s = skiplabel(line);
        if(s[0] != '.' && s[0] != '#') { 
            printf("Before copy: %s\n", s); 
            strcpy(file[instructions], s); 
            printf("After copy: %s\n", file[instructions]); 
            instructions++;
        }
    }
    for (int i = 0; i < instructions; i++) {
        printf("instruction: %s", file[i]);
        if(sscanf(file[i], "%10s $%5[^,],$%5s", oper, rs, rt) == 3) {
            printf("2 args\n");
        } else printf( "not instruction\n");
    }
    return 0;
}