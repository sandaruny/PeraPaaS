#include <stdio.h>

int main( int argc, char *argv[] ) {
	#define CHUNK 1024 /* read 1024 bytes at a time */
	char buf[CHUNK];
	int i,count;
	char appendStr[100];
	
	if(argc == 3){
		
		FILE *file;
		size_t nread;

		file = fopen(argv[2], "r");
		if (file) {
			
			while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
				fwrite(buf, 1, nread, stdout);
			if (ferror(file)) {
			/* deal with error */
			}
			fclose(file);
		}
		FILE *fp = fopen(argv[1], "ab");
		if (fp != NULL)
		{
			count = 0;
			for(i=0;i<strlen(buf);i++){
				if(i==0){
					appendStr = ()
						fputc(,fp);
				}else{	
					fputc(buf[i],fp);
				}	
			//fputs("\n",fp);
			//fputs(buf, fp);
			
			}
			fclose(fp);
		}
		
	}
	
	//printf("%s\n",buf);
	
	
}