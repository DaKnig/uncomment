#include <stdio.h>
#include <stdlib.h>

void cpstr(FILE* in,FILE* out,char chr){
	fputc(chr,out);
	int buff_chr=fgetc(in);
	while (buff_chr != EOF && buff_chr != chr && buff_chr!='\n'){
		fputc(buff_chr,out);
		buff_chr=fgetc(in);
	}
	fputc(buff_chr,out);
}

void skip_line(FILE* in){
	int buff_chr;
	do
		buff_chr=fgetc(in);
	while(buff_chr != '\n' && buff_chr != EOF);
	ungetc(buff_chr,in);
}

void skip_comment(FILE* in, FILE* out){
	int buff_chr[2];
	buff_chr[1]='E';

	do{
		buff_chr[0]=buff_chr[1];
		buff_chr[1]=fgetc(in);
		if(buff_chr[1]=='\n')	fputc('\n',out);
	}while( !( (buff_chr[1]==EOF) || ( (buff_chr[0]=='*' && buff_chr[1]=='/') ) ) );
}

int  main(int argc,char** argv){

	FILE* in = fopen( (argc>1?argv[1]:"input") ,"r");
	if (in==NULL){
		perror("can't open input file for reading");
		exit(1);	}
	FILE* out = fopen( (argc>2?argv[2]:"output") ,"w");
      if (out==NULL){
      	perror("can't open output file for writing");
            exit(1);	}

	switch (argc){
		case 1: printf("default names \"input\", \"output\" would be used\n");
			break;
		case 2: printf("default name \"output\" would be used\n");
			break;
		default:printf("too many input arguments\n");
			return 0;
	}

	int buff_chr;
	buff_chr=fgetc(in);

	while(buff_chr != EOF){
		switch(buff_chr){
			case ';':
				fputc(';',out);	skip_line(in);	break;
			case '/':
				buff_chr=fgetc(in);
                  	switch (buff_chr){
                          	case '/': skip_line(in);        break;
                          	case '*': skip_comment(in,out); break;
                          	default : fputc('/',out);       ungetc(buff_chr,in);     break;
				}
				break;
			case '\"': case '\'':
				cpstr(in,out,buff_chr);	break;
			default:
				fputc(buff_chr,out);	break;
		}
		buff_chr=fgetc(in);
	}
	fclose(in);fclose(out);
	return 0;
}
