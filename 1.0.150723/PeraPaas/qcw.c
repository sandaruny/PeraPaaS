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

void * request_balancer();
void * request_handler(void * conn);
void * queue_com();
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

static int pool_size = 0, web_req_count = 0, msg_pointer = 0, read_pointer = 0;


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
struct message msg;
struct message msg_queue[1000];

int sockfd, n;

struct sockaddr_in servaddr, cliaddr;
socklen_t len;
char mesg[1000];


int connect_d;

int main(int argc, char** argv) {

    char addr_command[] = "mystock/findstock/eurousd";

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

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY); //inet_addr("127.0.0.1");
    servaddr.sin_port = htons(32000);

    int yes = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) == -1)
        error("Can't bind the port");


    len = sizeof (cliaddr);

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s", asctime(timeinfo));
    //	printf("IP %s\n", servaddr.sin_addr.s_addr);


    char* banner;


    struct timeval stop, start;

    signal(SIGCHLD, SIG_IGN);
    pid_t pid_req_handler = fork();

    //printf("PIDS %d", getpid());

    if (pid_req_handler == 0) {

        printf("PIDS %d \n", getpid());

        pthread_t t0;
        if (pthread_create(&t0, NULL, request_balancer, NULL) == -1) {
            printf("Error in pthread for request balance");
        }
    }


    if (listen(sockfd, 10) == -1)
        error("Can't listen");


    pid_t pid_qcw_handler = fork();

    if (pid_qcw_handler < 0) {
        error("Error in QCW Process");
    }

    int pipe_qcw[2];
    if (pipe(pipe_qcw)) {
        error("Create pipe");
        return -1;
    }


    // QCW Handler 
    if (pid_qcw_handler == 0) {

        while (1) {
            while (read_pointer != msg_pointer) {
                if (msg_queue[read_pointer].state == 'd') {
                    send(connect_d, reply, strlen(mesg), 0);
                    printf("Mesg Dequued %d\n", read_pointer);
                }
                read_pointer++;
            }

            printf("RP and MP %d : %d \n", read_pointer, msg_pointer);
            sleep(1);
        }

    } else if (pid_qcw_handler > 0) {
        pthread_t t_queue_com;
        if (pthread_create(&t_queue_com, NULL, queue_com, NULL) == -1) {
            printf("Error in pthread for request balance");
        }
    }



    while (1) {
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof (client_addr);

        int connect_e = accept(sockfd, (struct sockaddr *) &client_addr, &address_size);

        if (connect_e == -1) {
            printf("Cannot open a socket");
            break;
        }

        printf("Socket conn %d\n", connect_e);

        web_req_count++;
        pthread_t t_handler;
        if (pthread_create(&t_handler, NULL, request_handler, (void *) &connect_e) == -1) {
            printf("Error in pthread for request balance");
        }



        /*
                n = recvfrom(connect_d, mesg, 1000, 0, (struct
                        sockaddr*) &cliaddr, &len);
    
         */


        /*
                int pid = fork();



                if (pid == 0) {




                }
         */
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

void * request_handler(void * conn) {
    char* banner;
    int connect_e = *(int *) conn;

    printf("Handler conn %d\n", connect_e);
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);


    while (1) {
        banner = asctime(timeinfo);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        int no = recvfrom(connect_e, mesg, 1000, 0, (struct
                sockaddr*) &cliaddr, &len);

        if (no < 0) {
            error("Error in recieving");

        } else if (no == 0) {
            return 1;
        }


        mesg[no] = '\0';
        puts(mesg);

        if (msg_queue[msg_pointer].state != 's') {

            msg_queue[msg_pointer].details = 'hello';
            msg_queue[msg_pointer].state = 'd';
            msg_queue[msg_pointer].msg_id = msg_pointer;
            msg_queue[msg_pointer].connection_id = connect_e;

        }
        ++msg_pointer;

        printf("MSSG POINTER %d >>>>>>>>>> %d \n", no, msg_pointer);

        //  sleep(1);
        send(connect_e, reply, strlen(reply), 0);



        int line_cnt = 0;
        //      printf("Recieved %d %s\n", no, mesg);

        char * mk = strstr(mesg, ref);
        //  mk = strstr(mk, "//");
        printf("found>>> %s\n", mk);

    }
}

void * queue_com() {



}
