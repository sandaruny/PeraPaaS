#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char**argv)
{

	int sockfd,n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	char mesg[1000];


	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);//inet_addr("127.0.0.1");
	servaddr.sin_port=htons(32000);
	
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	len = sizeof(cliaddr);

	time_t rawtime;
  	struct tm * timeinfo;

  	time ( &rawtime );
  	timeinfo = localtime (&rawtime);
  	printf ( "Current local time and date: %s", asctime (timeinfo) );
  //	printf("IP %s\n", servaddr.sin_addr.s_addr);
	char* banner = "Hello UDP client! This is UDP server";
  	n=recvfrom(sockfd,mesg,1000,0,(struct
		sockaddr*)&cliaddr,&len);

	while(1){
		banner = asctime (timeinfo);
		time ( &rawtime );
 	 	timeinfo = localtime ( &rawtime );
 
 	 	sleep(1);
		sendto(sockfd,banner,strlen(banner),0,(struct sockaddr
			*)&cliaddr,sizeof(cliaddr));
		int line_cnt =0;
		printf("Recieved \n");

	}


	return 0;
}