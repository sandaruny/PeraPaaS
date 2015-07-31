/* 
 * File:   qcw.c
 * Author: Sandaruwan
 *
 * Created on July 23, 2015, 10:16 PM
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include  <sys/types.h>
//#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "ppmsg.h"

/*
 * 
 */

static int pool_size = 0, web_req_count = 0;

int main(int argc, char** argv) {

    char addr_command[] = "mystock/findstock/eurousd";

    struct message msg;
    struct message msg_queue[3];

    int qcw_count, msg_count = 0;

    for (qcw_count = 0; qcw_count < 3; qcw_count++) {
        //  msg_queue[qcw_count] = malloc(sizeof(struct message));
        msg_queue[qcw_count].details = addr_command;
        msg_queue[qcw_count].state = 'd';
        msg_queue[qcw_count].msg_id = msg_count++;
    }

    for (qcw_count = 0; qcw_count < 3; qcw_count++) {
        printf("message %d\n", msg_queue[qcw_count].msg_id);
    }


    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char mesg[1000];


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY); //inet_addr("127.0.0.1");
    servaddr.sin_port = htons(32000);

    bind(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    len = sizeof (cliaddr);

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s", asctime(timeinfo));
    //	printf("IP %s\n", servaddr.sin_addr.s_addr);
    char* banner = "Hello UDP client! This is UDP server";


    struct timeval stop, start;


    pid_t timer_pid = fork();

    if (timer_pid == 0) {
        while (1) {
            sleep(1);
            printf("Req Count: %d\n", web_req_count);
        }
    }

    while (1) {


        n = recvfrom(sockfd, mesg, 1000, 0, (struct
                sockaddr*) &cliaddr, &len);

        int pid = fork();

        if (pid == 0) {

            while (1) {
                banner = asctime(timeinfo);
                time(&rawtime);
                timeinfo = localtime(&rawtime);

                sendto(sockfd, banner, strlen(banner), 0, (struct sockaddr
                        *) &cliaddr, sizeof (cliaddr));
                int line_cnt = 0;
                printf("Recieved \n");
                sleep(1);
            }
        }

    }
    return (EXIT_SUCCESS);
}

