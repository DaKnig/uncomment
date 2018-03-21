#include <stdio.h>
#include <stdlib.h>

void cpstr(FILE* in,FILE* out,char chr){
	fputc(chr,out);
	int buff=fgetc(in);
	while (buff != EOF && buff != chr && buff!='\n'){
		fputc(buff,out);
		buff=fgetc(in);
	}
	fputc(buff,out);
}

void skip_line(FILE* in){
	int buff;
	do
		buff=fgetc(in);
	while(buff != '\n' && buff != EOF);
	ungetc(buff,in);
}

void skip_comment(FILE* in, FILE* out){
	int buff[2];
	buff[1]='E';

	do{
		buff[0]=buff[1];
		buff[1]=fgetc(in);
		if(buff[1]=='\n')	fputc('\n',out);
	}while( !( (buff[1]==EOF) || ( (buff[0]=='*' && buff[1]=='/') ) ) );
}

int  main(int argc,char** argv){

	
	switch (argc){
		case 1: printf("default names \"input\", \"output\" would be used\n");
			break;
		case 2: printf("default name \"output\" would be used\n");
			break;
		default:printf("too many input arguments\n");
			return 0;
	}

	FILE* in = fopen( (argc>1?argv[1]:"input") ,"r");
	if (in==NULL){
		perror("cant open input file for reading");
		exit(1);	}
	FILE* out = fopen((argc>2?argv[2]:"output"),"w");
        if (out==NULL){
                perror("cant open output file for writing");
                exit(1);        }


	int buff[2];
	buff[0]='E',buff[1]=fgetc(in);

	while(buff[1] != EOF){
		switch(buff[1]){
			case ';':	fputc(';',out);	skip_line(in);	break;
			case '/':
				buff[0]=fgetc(in);
                        	switch (buff[0]){
                                	case '/': skip_line(in);        break;
                                	case '*': skip_comment(in,out); break;
                                	default : fputc('/',out);       ungetc(buff[0],in);     break;
				}
				break;
			case '\"': case '\'':
					cpstr(in,out,buff[1]);	break;
			default:	fputc(buff[1],out);	break;
		}
		buff[1]=fgetc(in);
	}
	fclose(in);fclose(out);
	return 0;
}
