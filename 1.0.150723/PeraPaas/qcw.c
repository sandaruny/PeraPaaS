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
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <signal.h>

/*
 * 
 */

int listener_d;

void* request_balancer();
void error(char *msg);

void handle_shutdown(int sig);
void register_sigkill_fn();

void register_sigkill_fn() {

    if (signal(SIGUSR1, handle_shutdown) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
    if (signal(SIGKILL, handle_shutdown) == SIG_ERR)
        printf("\ncan't catch SIGKILL\n");
    if (signal(SIGSTOP, handle_shutdown) == SIG_ERR)
        printf("\ncan't catch SIGSTOP\n");

}


int const QUEUE_SIZE = 100;

static int pool_size = 0, *web_req_count = 0, msg_pointer = 0;

int main(int argc, char** argv) {

    char addr_command[] = "mystock/findstock/eurousd";


    struct message msg;
    struct message msg_queue[QUEUE_SIZE];

    int qcw_count, msg_count = 0;

    for (qcw_count = 0; qcw_count < QUEUE_SIZE; qcw_count++) {
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


    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY); //inet_addr("127.0.0.1");
    servaddr.sin_port = htons(32000);


    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) == -1)
        error("Can't bind the port");


    len = sizeof (cliaddr);

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s", asctime(timeinfo));
    //	printf("IP %s\n", servaddr.sin_addr.s_addr);
    char* banner = "Hello UDP client! This is UDP server";


    struct timeval stop, start;

    pthread_t t0;
    if (pthread_create(&t0, NULL, request_balancer, NULL) == -1) {
        printf("Error in pthread for request balance");
    }

    if (listen(sockfd, 10) == -1)
        error("Can't listen");



    while (1) {
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof (client_addr);

        int connect_d = accept(sockfd, (struct sockaddr *) &client_addr, &address_size);

        if (connect_d == -1) {
            printf("Cannot open a socket");
            break;
        }
        web_req_count++;

        /*
                n = recvfrom(connect_d, mesg, 1000, 0, (struct
                        sockaddr*) &cliaddr, &len);
    
         */

        char *reply =
                "HTTP/1.1 200 OK\n"
                "Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
                "Server: Apache/2.2.3\n"
                "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
                "ETag: \"56d-9989200-1132c580\"\n"
                "Content-Type: text/html\n"
                "Content-Length: 15\n"
                "Accept-Ranges: bytes\n"
                "Connection: close\n"
                "\n"
                "sdfkjsdnbfkjbsf";

   //     printf("Recieved: %d\n", web_req_count);
        char * ref = "Referer:";
        int pid = fork();



        if (pid == 0) {

            while (1) {
                banner = asctime(timeinfo);
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                n = recvfrom(connect_d, mesg, 1000, 0, (struct
                        sockaddr*) &cliaddr, &len);

                if (n == 0) {
                    return;
                }


                mesg[n] = '\0';
            //    puts(mesg);
                msg_queue[msg_pointer].details = addr_command;
                msg_queue[msg_pointer].state = 'd';
                msg_queue[msg_pointer].msg_id = msg_count;

                ++msg_pointer;
                
                printf("MSSG POINTER >>>>>>>>>> %d \n", msg_pointer);
                
                //  sleep(1);
                send(connect_d, reply, strlen(mesg), 0);



                int line_cnt = 0;
          //      printf("Recieved %d %s\n", n, mesg);

                char * mk = strstr(mesg, ref);
                //  mk = strstr(mk, "//");
                printf("found>>> %s\n", mk);

            }
        }

    }

    return (EXIT_SUCCESS);
}

void* request_balancer() {

    while (1) {
        sleep(1);
        printf("Req Count: %d\n", web_req_count);
    }

}

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void handle_shutdown(int sig) {
    if (listener_d)
        close(listener_d);
    fprintf(stderr, "Bye!\n");
    exit(0);
}
