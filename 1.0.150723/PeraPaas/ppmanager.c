#include <stdio.h>

int main( int argc, char *argv[] ) {
	#define CHUNK 1024 /* read 1024 bytes at a time */
	char buf[CHUNK];
	int i,count;
	char appendStr[100];
	char *application = "Application/create";
	char *argv1 = argv[1];

	
	if(argc == 3){
		
		FILE *file;
		size_t nread;

		file = fopen(argv[2], "r");
		if (file) {
			
			while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
				buf[nread] = '\0';
				fwrite(buf, 1, nread, stdout);
			if (ferror(file)) {
			/* deal with error */
			}
			fclose(file);
		}
		FILE *fp;
		
		printf(argv[1]);
		
		if(strcmp(argv1, application) == 0){
			 fp = fopen("application.txt", "ab");
			if (fp != NULL)
			{
				count = 0;
				for(i=0;i<strlen(buf);i++){
					if(count == 0){
						fputc('{',fp);
						fputc(buf[i],fp);
						count++;
					}else if (buf[i+1] == '\0'){
						fputc('}',fp);
						fputc(buf[i],fp);
						//fputc(buf[i+1],fp);
						break;
				
					}else if (count == 3 && buf[i] == '\n'){
						fputc('}',fp);
						fputc(buf[i],fp);
						count =0;
					}else if (buf[i]=='\n'){	
						fputc(',',fp);
						count++;
					}else{	
				//fputs("\n",fp);
						fputc(buf[i], fp);
					}
				}
				fclose(fp);
			}
		
		
		}else{
			fp = fopen("service.txt", "ab");
			if (fp != NULL)
			{
				count = 0;
				for(i=0;i<strlen(buf);i++){
				/*	if(count == 0){
						fputc('{',fp);
						fputc(buf[i],fp);
						count++;
					}else if (buf[i+1] == '\0'){
						fputc('}',fp);
						fputc(buf[i],fp);
						//fputc(buf[i+1],fp);
						break;
				
					}else if (count == 3 && buf[i] == '\n'){
						fputc('}',fp);
						fputc(buf[i],fp);
						count =0;
					}else if (buf[i]=='\n'){	
						fputc(',',fp);
						count++;
					}else{*/	
				//fputs("\n",fp);
						fputc(buf[i], fp);
				//	}
				}
				fclose(fp);
			}
		}
		
	}
	
	//printf("%s\n",buf);
	
	
}
