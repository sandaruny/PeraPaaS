/* 
 * File:   ppmsg.h
 * Author: Sandaruwan
 *
 * Created on July 23, 2015, 10:25 PM
 */

struct Message
{
	char * from_addr;
	char * send_addr;
	char * details; 
	char state;
};