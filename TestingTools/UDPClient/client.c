/* Sample UDP client */
//#include <winsock2.h>


#include <string.h>


#include  <sys/types.h>
//#include <time.h>
#include <sys/time.h>
#include <stdlib.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 80

int main(int argc, char**argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[] = "Hello UDP server! This is UDP client";
    char recvline[1000];
    char scantxt[100];


    sockfd = socket(PF_INET, SOCK_STREAM, 0); // socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof (servaddr));
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(32000);

    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    send(sockfd, sendline, strlen(sendline), 0);


    while (1) {

        send(sockfd, sendline, strlen(sendline), 0);
        
        n = recv(sockfd, recvline, 1000, 0);
        if(n < 1){
            break;
        }
        recvline[n] = '\0';
        printf("Time  %d: %s\n", n,recvline);
        //  sleep(1);

    }


    recvline[n] = 0;


    return 0;
}