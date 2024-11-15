#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
int strToInt(char a[]){
	int var = 0;
	for(int i=0 ; i<strlen(a) ; i++){
		int n = a[i];
		var = var*16 + ( n<58 ? n-48 : n-55 );
	}
	return  var;
}
void rev(char A[]){
    int i=0;
    int j=strlen(A)-1;
    while (i<j){
        char temp=A[i];
        A[i]=A[j];
        A[j]=temp;
        i++;
        j--;
    }
}
void convert(char A[],char B[]){
    strcpy(B,"");
    int num=strToInt(A);
    while(num!=0){
        num%2==0?strcat(B,"0"):strcat(B,"1");
        num/=2;
    }
    rev(B);
}
void main(){
	FILE *input;
	input=fopen("object.txt","r");
	char rec,name[10],mask[10],bit[13],code[10],addr[10];
	int st,add,txtLen,pgrAdd,pgrLen,address,len;
	fscanf(input,"%c",&rec);
    printf("Enter the starting address : ");
    scanf("%x",&st);
    while(rec!='E'){
        if(rec=='H'){
            fscanf(input,"%s\t%x\t%x",name,&pgrAdd,&pgrLen);
            printf("Name of program : %s\n",name);
            printf("Length of program : %X\n",pgrLen);
            printf("----------------------------\n");
            printf("ADDRESS\t\tCONTENT\n");
            printf("----------------------------\n");
        }
        else if(rec=='T'){
            fscanf(input,"%x\t%x\t%s\t%s\t%s",&address,&txtLen,mask,code,addr);
            convert(mask,bit);
            int index=0;
            address+=st;
            
            while(strcmp(code,"-")!=0){
                if(bit[index]=='1'){
                    add=strToInt(addr)+st;
                    printf("%X\t\t%s%X\n",address,code,add);
                }
                else{printf("%X\t\t%s%s\n",address,code,addr);}
                address+=3;
                index++;
                fscanf(input,"%s\t%s",code,addr);
            }
        }
        fscanf(input,"%c",&rec);
    }
	fclose(input);
}
/*
Enter the starting address : 4000
Name of program : COPY
Length of program : 107A
----------------------------
ADDRESS         CONTENT
----------------------------
4000            144033
4003            485039
4006            104036
4009            284030
400C            304015
400F            485061
4012            3C4003
4015            20402A
4018            1C4039
401B            30402D
6500            1D4036
6503            485061
6506            184033
6509            4C1000
650C            801000
650F            601003
*/
