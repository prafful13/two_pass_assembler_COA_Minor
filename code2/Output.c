#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int flag[8]={0};
//	flag[0]=negative flag | flag[1]=zero flag | flag[2]=carry flag


//Bitwise multiplication function
int multiply(int x, int y)
{
   	if(y == 0)
    	return 0;

   	if(y > 0 )
    	return (x + multiply(x, y-1));
  
   	if(y < 0 )
    	return -multiply(x, -y);
}


//Bitwise addition function
int Add(int x, int y)
{
	int carry=0,temp=1;
	if(y<0)	temp=0;
    while (y != 0)
    {
		carry = x & y;
        x = x ^ y; 
        y = carry << 1;
        if(y!=0)	flag[2]=carry;
    }
    if(temp==0)	flag[2]=0;
    return x;
}

//Function to convert binary to integer
int bintoint(int a)	{
	int temp,val=0,i=0;
	while(a>0)	{
		temp=a%10;
		val+=temp*pow(2,i);
		i++;
		a/=10;
	}
	return val;
}

//Start of main
int main()	{
	//	Declaring File pointers. 
	FILE *bin,*op,*reg,*out,*insflag;
	
	char opcode[50][2][40];
	char regval[50][40];
	char alu_status[5]="0000";
	char ch;
	
	int i,j,regj,opj,line=1;
	int regist[15]={0},linepointer[100];
	int aror[100][100];

	//	opening different files used in the program
	op = fopen("Instructions.txt","r");
	reg = fopen("Register.txt","r");
	bin = fopen("bin.txt","r");
	out = fopen("out.txt","w");
	insflag = fopen("flow_of_control.txt","w");
	
	/*
		Storing line pointers in 1D array in linepointer
	*/
	do	{
		char st[100];
    	fgets(st,100,bin);
    	linepointer[line]=ftell(bin);
    	line++;
	}while( !feof(bin) );
	fclose(bin);
	bin = fopen("bin.txt","r");	
	
	/*
		Reading opcode and their binary code from instructions.txt file and 
		storing it in 3-D array named opcode
	*/
	
	i=0,j=0;
	do	{
		char st[100];
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
	
	/*
		Storing only binary codes of registors in regval[] line by line
	*/
	
	i=0,j=0;
	do	{
		char st[100];
		fscanf(reg,"%s",st);
		if(i==0)	i=1;
		else	{
			strcpy(regval[j],st);
			j++;
			i--;
		}
	}while( !feof(reg) );
	regj=j;

	do	{
		char st[100],opc[35],regc[10];
		int val,temp,len,k,l,m,n,a,b,c,fl=0;
		
		//	extracting a word from file (32 bit)
    	fscanf(bin,"%s",st);
    	
    	//	extracting first 17 bits from string
    	for(i=0;i<17;i++)
    		opc[i]=st[i];
    	opc[i]='\0';
    	
		for(j=0;j<opj;j++)	{
    		
    		//finding equivalent opcode
			if(strcmp(opc,opcode[j][1])==0)	{
				fl=1;
    			if(strcmp(opcode[j][0],"ADD1")==0)	{
    				
    				//performs the add operation reg1=reg2+reg3.
    				for(i=17,n=0;i<22;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				for(m=0;m<regj;m++)
    					if(strcmp(regc,regval[m])==0)	break;
    				for(i=22,n=0;i<27;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(k=0;k<regj;k++)
    					if(strcmp(regc,regval[k])==0)	break;
    				for(i=27,n=0;i<32;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(l=0;l<regj;l++)
    					if(strcmp(regc,regval[l])==0)	break;
    				a=atoi(regval[m]);
    				b=atoi(regval[k]);
    				c=atoi(regval[l]);
    				a=bintoint(a);
    				b=bintoint(b);
    				c=bintoint(c);
    				regist[a]=Add(regist[b],regist[c]);
    				strcpy(alu_status,"0001");
    				
				}
				else if(strcmp(opcode[j][0],"MOV1")==0)	{
					
					//performs the move opertion reg1[reg2]=reg3.
    				for(i=17,n=0;i<22;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				for(m=0;m<regj;m++)
    					if(strcmp(regc,regval[m])==0)	break;
    				for(i=22,n=0;i<27;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(k=0;k<regj;k++)
    					if(strcmp(regc,regval[k])==0)	break;
    				for(i=27,n=0;i<32;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(l=0;l<regj;l++)
    					if(strcmp(regc,regval[l])==0)	break;
    				a=atoi(regval[m]);
    				b=atoi(regval[k]);
    				c=atoi(regval[l]);
					a=bintoint(a);
    				b=bintoint(b);
    				c=bintoint(c);
    				aror[a][regist[b]]=regist[c];
				}
				else if(strcmp(opcode[j][0],"MOV2")==0)	{
					
					//performs the move operation reg1=reg2[reg3].
    				for(i=17,n=0;i<22;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				for(m=0;m<regj;m++)
    					if(strcmp(regc,regval[m])==0)	break;
    				for(i=22,n=0;i<27;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(k=0;k<regj;k++)
    					if(strcmp(regc,regval[k])==0)	break;
    				for(i=27,n=0;i<32;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(l=0;l<regj;l++)
    					if(strcmp(regc,regval[l])==0)	break;
    				a=atoi(regval[m]);
    				b=atoi(regval[k]);
    				c=atoi(regval[l]);
					a=bintoint(a);
    				b=bintoint(b);
    				c=bintoint(c);
    				regist[a]=aror[b][regist[c]];
				}
				else if(strcmp(opcode[j][0],"MOD")==0)	{
					
					//performs the mod operation reg1=reg2%reg3. 
    				for(i=17,n=0;i<22;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				for(m=0;m<regj;m++)
    					if(strcmp(regc,regval[m])==0)	break;
    				for(i=22,n=0;i<27;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(k=0;k<regj;k++)
    					if(strcmp(regc,regval[k])==0)	break;
    				for(i=27,n=0;i<32;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(l=0;l<regj;l++)
    					if(strcmp(regc,regval[l])==0)	break;
    				a=atoi(regval[m]);
    				b=atoi(regval[k]);
    				c=atoi(regval[l]);
    				a=bintoint(a);
    				b=bintoint(b);
    				c=bintoint(c);
    				if(regist[c]!=2)	regist[a]=regist[b]%regist[c];
    				else	regist[a]=regist[b] & 1;
					strcpy(alu_status,"0010");
				}
				else if(strcmp(opcode[j][0],"ARR")==0)	{
					
					//creates an array of size reg2 with initial value immediate.
					for(i=17,n=0;i<22;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				for(m=0;m<regj;m++)
    					if(strcmp(regc,regval[m])==0)	break;
    				for(i=22,n=0;i<27;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
					for(k=0;k<regj;k++)
    					if(strcmp(regc,regval[k])==0)	break;
    				for(i=27,n=0;i<32;n++,i++)
    					regc[n]=st[i];
    				regc[n]='\0';
    				a=atoi(regval[m]);
    				b=atoi(regval[k]);
    				c=atoi(regc);
    				a=bintoint(a);
    				b=bintoint(b);
    				c=bintoint(c);
    				for(i=0;i<regist[b];i++)	{
    					aror[a][i]=c;
					}
				}
			}
		}
		if(fl!=1)	{
			for(i=17;i<19;i++)
    			opc[i]=st[i];
    		opc[i]='\0';
    		
    		for(j=0;j<opj;j++)	{
    			if(strcmp(opc,opcode[j][1])==0)	{
    				fl=1;
    				if(strcmp(opcode[j][0],"POW")==0)	{
    					
    					//performs the pow operation
    					int x=0,temp;
    					for(i=19,n=0;i<24;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					for(m=0;m<regj;m++)
    						if(strcmp(regc,regval[m])==0)	break;
    					for(i=24,n=0;i<32;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					a=atoi(regval[m]);
    					a=bintoint(a);
    					b=atoi(regc);
    					b=bintoint(b);
    					temp=regist[a];
    					regist[a]=1;
    					for(x=1;x<=temp;x++)	{
    						regist[a]=multiply(regist[a],b);
						}
    					strcpy(alu_status,"0011");
					}
					else if(strcmp(opcode[j][0],"DIVI")==0)	{
						
						//performs the division operation
						for(i=19,n=0;i<24;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					for(m=0;m<regj;m++)
    						if(strcmp(regc,regval[m])==0)	break;
    					for(i=24,n=0;i<32;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					a=atoi(regval[m]);
    					a=bintoint(a);
    					b=atoi(regc);
    					b=bintoint(b);
    					if(b!=2)	regist[a]=regist[a]/b;
    					regist[a]=regist[a] >> 1;
    					strcpy(alu_status,"0100");
					}
					else if(strcmp(opcode[j][0],"MOVI")==0)	{
						
						//performs the move operation r1=immediate value
						for(i=19,n=0;i<24;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					for(m=0;m<regj;m++)
    						if(strcmp(regc,regval[m])==0)	break;
    					for(i=24,n=0;i<32;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					a=atoi(regval[m]);
    					a=bintoint(a);
    					b=atoi(regc);
    					b=bintoint(b);
    					regist[a]=b;
					}
					else if(strcmp(opcode[j][0],"CMPI")==0)	{
						
						//returns reg1-immediate.
						int temp;
						for(i=19,n=0;i<24;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					for(m=0;m<regj;m++)
    						if(strcmp(regc,regval[m])==0)	break;
    					for(i=24,n=0;i<32;n++,i++)
    						regc[n]=st[i];
    					regc[n]='\0';
    					a=atoi(regval[m]);
    					a=bintoint(a);
    					b=atoi(regc);
    					b=bintoint(b);
    					b=multiply(b,-1);
    					temp=Add(regist[a],b);
    					if(temp>0)	{
    						flag[0]=0;
    						flag[1]=0;
						}
						else if(temp==0)	{
							flag[0]=0;
							flag[1]=1;
						}
						else if(temp<0)	{
							flag[0]=1;
							flag[1]=0;
						}
					}
				}
    		}
			
			if(fl!=1)	{
				for(i=19;i<22;i++)
    				opc[i]=st[i];
    			opc[i]='\0';
    			
    			for(j=0;j<opj;j++)	{
    				if(strcmp(opc,opcode[j][1])==0)	{
    					fl=1;
    					if(strcmp(opcode[j][0],"CMP")==0)	{
    						
    						//returns reg1-reg2
    						int temp1,temp2;
    						for(i=22,n=0;i<27;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(k=0;k<regj;k++)
    							if(strcmp(regc,regval[k])==0)	break;
   	 						for(i=27,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(l=0;l<regj;l++)
    							if(strcmp(regc,regval[l])==0)	break;
    						b=atoi(regval[k]);
   	 						c=atoi(regval[l]);
    						b=bintoint(b);
    						c=bintoint(c);
    						temp2=regist[c];
    						temp2=multiply(temp2,-1);
    						temp1=Add(regist[b],temp2);
    						if(temp1>0)	{
    							flag[0]=0;
    							flag[1]=0;
							}
							else if(temp1==0)	{
								flag[0]=0;
								flag[1]=1;
							}
							else if(temp1<0)	{
								flag[0]=1;
								flag[1]=0;
							}
						}
						else if(strcmp(opcode[j][0],"DIV")==0)	{
							
							//divides reg1 by reg2
    						for(i=22,n=0;i<27;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(k=0;k<regj;k++)
    							if(strcmp(regc,regval[k])==0)	break;
   	 						for(i=27,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(l=0;l<regj;l++)
    							if(strcmp(regc,regval[l])==0)	break;
    						b=atoi(regval[k]);
   	 						c=atoi(regval[l]);
    						b=bintoint(b);
    						c=bintoint(c);
    						regist[b]=regist[b]/regist[c];
    						strcpy(alu_status,"0101");
						}
						else if(strcmp(opcode[j][0],"ADD")==0)	{
							
							//performs reg1=reg1+reg2
    						for(i=22,n=0;i<27;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(k=0;k<regj;k++)
    							if(strcmp(regc,regval[k])==0)	break;
   	 						for(i=27,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(l=0;l<regj;l++)
    							if(strcmp(regc,regval[l])==0)	break;
    						b=atoi(regval[k]);
   	 						c=atoi(regval[l]);
    						b=bintoint(b);
    						c=bintoint(c);
    						regist[b]=Add(regist[b],regist[c]);
    						strcpy(alu_status,"0110");
						}
						else if(strcmp(opcode[j][0],"MOV")==0)	{
							
							//performs move operation
    						for(i=22,n=0;i<27;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(k=0;k<regj;k++)
    							if(strcmp(regc,regval[k])==0)	break;
   	 						for(i=27,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(l=0;l<regj;l++)
    							if(strcmp(regc,regval[l])==0)	break;
    						b=atoi(regval[k]);
   	 						c=atoi(regval[l]);
    						b=bintoint(b);
    						c=bintoint(c);
    						regist[b]=regist[c];
						}
						else if(strcmp(opcode[j][0],"SUB")==0)	{
							
							//performs reg1=reg1-reg2
							int temp;
    						for(i=22,n=0;i<27;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(k=0;k<regj;k++)
    							if(strcmp(regc,regval[k])==0)	break;
   	 						for(i=27,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
							for(l=0;l<regj;l++)
    							if(strcmp(regc,regval[l])==0)	break;
    						b=atoi(regval[k]);
   	 						c=atoi(regval[l]);
    						b=bintoint(b);
    						c=bintoint(c);
    						temp=regist[c];
    						temp=multiply(temp,-1);
    						regist[b]=Add(regist[b],temp);
    						strcpy(alu_status,"0111");
						}
						else if(strcmp(opcode[j][0],"JUMP")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						fseek(bin,linepointer[a-1],SEEK_SET);
						}
						else if(strcmp(opcode[j][0],"JGT")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						if(flag[0]==0 && flag[1]==0)	fseek(bin,linepointer[a-1],SEEK_SET);
						}
						else if(strcmp(opcode[j][0],"JGE")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						if(flag[0]==0 || flag[1]==1)	fseek(bin,linepointer[a-1],SEEK_SET);
						}
						else if(strcmp(opcode[j][0],"JLT")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						if(flag[0]==1 && flag[1]==0)	fseek(bin,linepointer[a-1],SEEK_SET);
						}
						else if(strcmp(opcode[j][0],"JLE")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						if(flag[0]==1 || flag[1]==1)	fseek(bin,linepointer[a-1],SEEK_SET);
						}
						else if(strcmp(opcode[j][0],"JNE")==0)	{
							for(i=22,n=0;i<32;n++,i++)
    							regc[n]=st[i];
    						regc[n]='\0';
    						a=atoi(regc);
    						a=bintoint(a);
    						if(flag[1]==0)	fseek(bin,linepointer[a-1],SEEK_SET);
						}
					}
				}
				
				if(fl!=1)	{
					for(i=22;i<23;i++)
    					opc[i]=st[i];
    				opc[i]='\0';
    				
    				for(j=0;j<opj;j++)	{
    					if(strcmp(opc,opcode[j][1])==0)	{
    						fl=1;
    						if(strcmp(opcode[j][0],"IN")==0)	{
    							
    							//Take input
    							for(i=23,n=0;i<=27;n++,i++)
    								regc[n]=st[i];
    							regc[n]='\0';
								for(k=0;k<regj;k++)
    								if(strcmp(regc,regval[k])==0)	break;
    							b=atoi(regval[k]);
    							b=bintoint(b);
    							scanf("%d",&regist[b]);
							}
							else if(strcmp(opcode[j][0],"OUT")==0)	{
								
								//print ouput
								for(i=23,n=0;i<=27;n++,i++)
    								regc[n]=st[i];
    							regc[n]='\0';
								for(k=0;k<regj;k++)
    								if(strcmp(regc,regval[k])==0)	break;
    							b=atoi(regval[k]);
    							b=bintoint(b);
    							fprintf(out,"%d ",regist[b]);
							}
						}
					}
					
					if(fl!=1)	{
						for(i=23;i<27;i++)
    						opc[i]=st[i];
    					opc[i]='\0';
    					
    					for(j=0;j<opj;j++)	{
    						if(strcmp(opc,opcode[j][1])==0)	{
    							fl=1;
    							if(strcmp(opcode[j][0],"INC")==0)	{
    								
    								//incriment register
    								for(i=27,n=0;i<32;n++,i++)
    									regc[n]=st[i];
    								regc[n]='\0';
									for(k=0;k<regj;k++)
    									if(strcmp(regc,regval[k])==0)	break;
    								b=atoi(regval[k]);
    								b=bintoint(b);
    								regist[b]=Add(regist[b],1);
    								strcpy(alu_status,"1000");
								}
								else if(strcmp(opcode[j][0],"DEC")==0)	{
									
									//decrement dec
    								for(i=27,n=0;i<32;n++,i++)
    									regc[n]=st[i];
    								regc[n]='\0';
									for(k=0;k<regj;k++)
    									if(strcmp(regc,regval[k])==0)	break;
    								b=atoi(regval[k]);
    								b=bintoint(b);
    								regist[b]=Add(regist[b],-1);
    								strcpy(alu_status,"1001");
								}
							}
						}
						
						if(fl!=1)	{
							for(i=27;i<32;i++)
    							opc[i]=st[i];
    						opc[i]='\0';
    						
    						for(j=0;j<opj;j++)	{
   	 							if(strcmp(opc,opcode[j][1])==0)	{
    								fl=1;
    								if(strcmp(opcode[j][0],"NEWLINE")==0)	{
    									
    									//print newline
    									fprintf(out,"\n");
									}
									else if(strcmp(opcode[j][0],"HLT")==0)	{
										
										//exit
    									return 0;
									}
								}
							}
						}
					}
				}
			}		
		}
				
		//Printing in flow_of_control file the various register values
		fprintf(insflag,"LINE:%s\n",st);
    	for(i=0;i<13;i++)	fprintf(insflag,"\tR%d=%d ",i,regist[i]);
		fprintf(insflag,"\n");
		fprintf(insflag,"\tNegative Flag: %d\n",flag[0]);
		fprintf(insflag,"\tZero Flag: %d\n",flag[1]);
		fprintf(insflag,"\tCarry Flag: %d\n",flag[2]);
		fprintf(insflag,"\tProgram Counter: %d\n",(ftell(bin)/32)*4+2048);
		if(strcmp(alu_status,"0000")!=0)	fprintf(insflag,"\tALU Status Register: %s",alu_status);
		fprintf(insflag,"\n\n");
		strcpy(alu_status,"0000");
		flag[2]=0;
	}while( !feof(bin) );
	fclose(bin);
	fclose(op);
	fclose(insflag);
	fclose(reg);
	fclose(out);
	return 0;
}
