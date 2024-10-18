#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int label_exists(char *label, FILE *symtab){
    char line[100];
    while(fgets(line, sizeof(line), symtab)){
        char sym_label[100] = {0};
        sscanf(line, "%s", sym_label);
        if(strcmp(sym_label, label) == 0){
            return 1;
        }
    }
    return 0;
}

int valid_opcode(char *opcode, char *filename){
    FILE *optab = fopen(filename, "r");
    char line[100];
    while(fgets(line, sizeof(line), optab)){
        char optab_val[100] = {0};
        sscanf(line, "%s", optab_val);
        if(strcmp(optab_val, opcode) == 0){
            fclose(optab);
            return 1;
        }
    }
    fclose(optab);
    return 0;

}

void main(){
    char filename[100];
    printf("Enter the name of Assembly code file: ");
    scanf("%s", filename);

    FILE *input = fopen(filename, "r");
    FILE *symtab = fopen("symtab.txt", "w+");
    FILE *inter = fopen("inter.txt", "w");

    char line[100], line2[100], line3[100];

    int locctr = 0, flag = 0;
    while(fgets(line, sizeof(line), input)){
        char label[100] = {0}, opcode[100] = {0}, operand[100] = {0};
        if(strchr(line, ':') != NULL){
            sscanf(line, "%[^:]: %s %s", label, opcode, operand);

            rewind(symtab);
            if(label_exists(label, symtab)){
                printf("Duplicate label found: %s\n", label);
                exit(1);
            }

            if(valid_opcode(opcode, "optable.txt")){
                fprintf(symtab, "%s\t%d\n", label, locctr);
                fprintf(inter, "%s\t%s\t%s\t%d\n", label, opcode, operand, locctr);

                if(strcmp(opcode, "RESW")){
                    locctr += atoi(operand) * 3;
                }
                else if(strcmp(opcode, "RESB")){
                    locctr += atoi(operand);
                }
                else if(strcmp(opcode, "BYTE")){
                    locctr += strlen(operand) - 3;
                }
                else{
                    locctr += 3;
                }
            }
            else{
                printf("Invalid Opcode %s\n", opcode);
                exit(1);
            }

        }
        else{
            sscanf(line, "%s %s", opcode, operand);
            if(valid_opcode(opcode, "optable.txt")){
                if(strcmp(opcode, "START") == 0){
                    locctr = atoi(operand);
                }
                fprintf(inter, "\t\t%s\t%s\t%d\n", opcode, operand, locctr);
                locctr += 3;
            }
            else{
                printf("Invalid Opcode %s\n", opcode);
                exit(1);
            }
        }
    }


}
