/* 
 * File:   ppmsg.h
 * Author: Sandaruwan
 *
 * Created on July 23, 2015, 10:42 PM
 */
struct message
{
	char * from_addr;
	char * send_addr;
	char * details; 
	char state;
        int lastCompletedStep;
        int msg_id;
        int connection_id;
};


