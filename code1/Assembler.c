#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()	{
	FILE *code,*sym,*bin,*op,*reg;
	char opcode[50][2][40];
	char regis[50][2][40];
	char label[50][2][40];
	char st[100];
	int line=1,i,j,symj,regj,opj,stlen=0;

    code = fopen("code.txt","r");
	sym = fopen("sym.txt","w");
	fgets(st,100,code);
	do	{
		int len;
		len=strlen(st);							
		for(i=0;i<len;i++)						//loop for finding colon(label identifier) in the string
			if(st[i]==':')	{					//and then writing in symbol table file its corresponding line number.
				st[i]='\0';
				fprintf(sym,"%s %d\n",st,line+1);
			}
    	line++;
    	fgets(st,100,code);
	}while( !feof(code) );
	fclose(code);
	fclose(sym);
	
	code = fopen("code.txt","r");
	sym = fopen("sym.txt","r");
	op = fopen("Instructions.txt","r");
	reg = fopen("Register.txt","r");
	bin = fopen("bin.txt","w");
	
	i=0,j=0;
	do	{										//reading opcodes and their binary representation and writing them in array
		fscanf(op,"%s",st);
		if(i==0)	{
			strcpy(opcode[j][i],st);
			i++;
		}
		else	{
			strcpy(opcode[j][i],st);
			j++;
			i--;
		}
	}while( !feof(op) );						
	opj=j;
	
	i=0,j=0;
	do	{										//reading registers, ports and their binary representation and writing them in array
		fscanf(reg,"%s",st);
		if(i==0)	{
			strcpy(regis[j][i],st);
			i++;
		}
		else	{
			strcpy(regis[j][i],st);
			j++;
			i--;
		}
	}while( !feof(reg) );
	regj=j;
	
	i=0,j=0;
	do	{										//reading labels and their line values from symbol table file
		fscanf(sym,"%s",st);
		if(i==0)	{
			strcpy(label[j][i],st);
			i++;
		}
		else	{
			strcpy(label[j][i],st);
			j++;
			i--;
		}
	}while( !feof(sym) );
	symj=j;
		
	fscanf(code,"%s",st);
	do	{										//writing binary representation of assembly code.
		char temp1[15];
		int val,temp,len,fl=0;
		len=strlen(st);
		if(st[len-1]==':')	{					//skip if label instruction.(leaving label line blank in binary file)
			fprintf(bin,"\n");
			stlen=0;
		}
		else	{
			for(j=0;j<opj;j++)						//comparing string from file and opcode array and then writing binary form in file
				if(strcmp(st,opcode[j][0])==0)	{
					fprintf(bin,"%s",opcode[j][1]);
					fl=1;
					stlen+=strlen(opcode[j][1]);
					break;
				}
			if(fl==0)	{
				for(j=0;j<regj;j++)	{				//comparing string from file and register array and then writing binary form in file
					if(strcmp(st,regis[j][0])==0)	{
						fprintf(bin,"%s",regis[j][1]);
						fl=1;
						stlen+=strlen(regis[j][1]);
						break;
					}
				}
			}
			if(fl==0)	{
				for(j=0;j<symj;j++)	{				//reading line number from symbol table file and converting it to binary and then writing in binary file
					if(strcmp(st,label[j][0])==0)	{			//only when jump instructions are used
						val=atoi(label[j][1]);
						strcpy(temp1,"0000000000");
						j=9;
						while(val>0)	{				
							temp=val%2;
							val/=2;
							temp1[j]=temp+'0';
							j--;
						}
						fprintf(bin,"%s",temp1);
						fl=1;
						break;
					}
				}
			}
			if(fl==0 && strcmp(st,";")!=0)	{		//when immediate values are encountered in assembly code
				temp=32-stlen;
				if(temp==8)	{						//for 8 bit immediate values(used in 2 address instructions)
					val=atoi(st);
					strcpy(temp1,"00000000");		
					j=7;
					while(val>0)	{				//converts immediate number into its binary representation
						temp=val%2;
						val/=2;
						temp1[j]=temp+'0';
						j--;
					}
					
					fprintf(bin,"%s",temp1);
				}
				else	{							//for 5 bit immediate values(used in 3 address instructions)
					val=atoi(st);
					strcpy(temp1,"00000");
					j=4;							
					while(val>0)	{
						temp=val%2;
						val/=2;
						temp1[j]=temp+'0';
						j--;
					}
					fprintf(bin,"%s",temp1);
				}
			}
			if(strcmp(st,";")==0){					//checking for end of instruction
				fprintf(bin,"\n");
				stlen=0;
			}
		}
		fscanf(code,"%s",st);
	}while( !feof(code) );
	fclose(code);
	fclose(sym);
	fclose(bin);
	fclose(op);
	fclose(reg);
	return 0;
}
