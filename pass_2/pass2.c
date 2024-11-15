#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
void intToHex(int num,char number[])
{
    int temp;
    int tempB[10];
    int i=1;
    while (num != 0) { 
        temp = num % 16; 
        if (temp < 10) 
            temp = temp + 48; 
        else
            temp = temp + 55; 
        tempB[i++] = temp; 
        num = num / 16;
    }
    int index=0; 
    for (int j = i - 1; j > 0; j--) 
        number[index++]=tempB[j];
    number[index]='\0';
}
void main()
{
    char loc[10],opcode[10],label[10],operand[10],mnemonic[10],code[10],addr[10],objectcode[10];
    FILE *input,*output,*symtab,*optab,*length;
    int locctr,start,end,len,byte_len=0,count=0;
    char text[30][10];
    bool err = false,found_opcode,found_symbol;
    input=fopen("intermediate.txt","r");
    output=fopen("output.txt","w");
    optab=fopen("optab.txt","r");
    symtab=fopen("symtab.txt","r");
    length=fopen("length.txt","r");
    fscanf(length,"%X",&len);

    fscanf(input,"%s\t%s\t%s\t%s",loc,label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        fprintf(output,"H^%s^%s^%X\n",label,operand,len);
        start=atoi(operand);
        fscanf(input,"%s\t%s\t%s\t%s",loc,label,opcode,operand);
    }
    else{fprintf(output,"H^-^%s^%X\n",loc,len);start=atoi(loc);}
    locctr=start;
    while(strcmp("END",opcode)!=0)
    {
        end=atoi(loc);
        fseek(optab,SEEK_SET,0);
        fscanf(optab,"%s\t%s",code,mnemonic);
        found_opcode=false;
        while(strcmp(code,"END")!=0)
        {
            if(strcmp(code,opcode)==0)
            {
                if(strcmp(operand,"-")!=0)
                {
                    found_symbol=false;
                    char symbol[10],loc[10];
                    fseek(symtab,SEEK_SET,0);
                    do
                    {
                        fscanf(symtab,"%s\t%s",symbol,loc);
                        if(strcmp(symbol,operand)==0)
                        {
                            strcpy(addr,loc);
                            found_symbol=true;
                            break;
                        }
                    }while(!feof(symtab));
                    if(!found_symbol)
                    {
                        err=true;
                        printf("Invalid Symbol %s\n",operand);
                        exit(0);
                    }
                }
                else{strcpy(addr,"0000");}
                found_opcode=true;
                len=3;
                strcpy(objectcode,"");
                strcat(objectcode,code);
                strcat(objectcode,addr);
                break;
            }
            fscanf(optab,"%s\t%s",code,mnemonic);
        }
        if(strcmp(opcode,"BYTE")==0)
        {
            if(operand[0]=='X')
            {
                strcpy(objectcode,"");
                strncpy(objectcode,operand+2,2);
                len=1;
            }
            else
            {
                char temp[10],num[2];
                strncpy(temp,operand+2,strlen(operand)-3);
                strcpy(objectcode,"");
                len=strlen(operand)-3;
                if(strlen(operand)-3<3){
                    for(int i=0;i<3-strlen(operand)+3;i++){
                        strcat(objectcode,"00");
                    }
                }
                for (int i=0;i<strlen(operand)-3;i=i+1)
                {
                    intToHex(temp[i],num);
                    strcat(objectcode,num);
                }
            }
        }
        else if(strcmp("WORD",opcode)==0)
        {
            char num[10];
            len=3;
            intToHex(atoi(operand),num);
            strcpy(objectcode,num);
        }
        else if(strcmp(opcode,"RESB")==0){
            strcpy(objectcode,"-");
            len=atoi(operand);
        }
        else if(strcmp(opcode,"RESW")==0){
            strcpy(objectcode,"-");
            len=3*atoi(operand);
        }
        if(byte_len>30)
        {
            fprintf(output,"T^%d^%X",locctr,byte_len);
            for(int i = 0;i<count;i++)
            {
                fprintf(output,"^%s",text[i]);
            }
            fprintf(output,"\n");
            byte_len=len;
            count=0;
            locctr=atoi(loc);
            strcpy(text[count++],objectcode);
        }
        else
        {
            strcpy(text[count++],objectcode);
            byte_len+=len;
        }
        fscanf(input,"%s\t%s\t%s\t%s",loc,label,opcode,operand);
    }
    fprintf(output,"T^%d^%X",locctr,byte_len);
    for(int i = 0;i<count;i++)
    {
        fprintf(output,"^%s",text[i]);
    }
    fprintf(output,"\n");
    fprintf(output,"E^%d",start);
    fclose(output);
    fclose(symtab);
    fclose(optab);
    fclose(input);
    fclose(length);
}