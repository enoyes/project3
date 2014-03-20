/*
 * CS3600, Spring 2014
 * Project 2 Starter Code
 * (c) 2013 Alan Mislove
 *
 */

#ifndef __3600DNS_H__
#define __3600DNS_H__

#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct dnsheader{
	unsigned int ID:16;


	unsigned int RD:1;
	unsigned int TC:1;
	unsigned int AA:1;
	unsigned int OPCODE:4;
	unsigned int QR:1;

	unsigned int RCODE:4;
	unsigned int Z:3;
	unsigned int RA:1;

	unsigned int QDCOUNT:16;
	unsigned int ANCOUNT:16;
	unsigned int NSCOUNT:16;
	unsigned int ARCOUNT:16;
} dnsheader;

typedef struct dnsquestion{

	unsigned int qtype:16;
	unsigned int qclass:16;
	
} dnsquestion;


#endif

