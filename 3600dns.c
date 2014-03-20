/*
 * CS3600, Spring 2014
 * Project 3 Starter Code
 * (c) 2013 Alan Mislove
 *
 */


#include "3600dns.h"

/**
 * This function will print a hex dump of the provided packet to the screen
 * to help facilitate debugging.  In your milestone and final submission, you 
 * MUST call dump_packet() with your packet right before calling sendto().  
 * You're welcome to use it at other times to help debug, but please comment those
 * out in your submissions.
 *
 * DO NOT MODIFY THIS FUNCTION
 *
 * data - The pointer to your packet buffer
 * size - The length of your packet
 */



static void dump_packet(unsigned char *data, int size) {
    unsigned char *p = data;
    unsigned char c;
    int n;
    char bytestr[4] = {0};
    char addrstr[10] = {0};
    char hexstr[ 16*3 + 5] = {0};
    char charstr[16*1 + 5] = {0};
    for(n=1;n<=size;n++) {
        if (n%16 == 1) {
            /* store address for this line */
            snprintf(addrstr, sizeof(addrstr), "%.4x",
               ((unsigned int)p-(unsigned int)data) );
        }
            
        c = *p;

        if (isprint(c) == 0) {
            c = '.';
        }

        /* store hex str (for left side) */
        snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
        strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

        /* store char str (for right side) */
        snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

        if(n%16 == 0) { 
            /* line completed */
            printf("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
            hexstr[0] = 0;
            charstr[0] = 0;
        } else if(n%8 == 0) {
            /* half line: add whitespaces */
            strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
            strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
        }
        p++; /* next byte */
    }

    if (strlen(hexstr) > 0) {
        /* print rest of buffer if not empty */
        printf("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
    }
}


char *getqname(char *path){
		/*
		1.  Tokenize the input string on "."
			// check out strtok
		 
		2.  For each token, determine the length
			int len = strlen (token);
		 
		3.  Copy in the length, followed by memcpy()ing the string data
			char tmp[offset] = len;
			memcpy(tmp + offset + 1, token, len);
		 
		4.  Increment offset, repeat for the next label.  At the very end, write a 0-length label.
			*/

  //char *pathtmp = (char *) malloc(2048);
	//memset(pathtmp, 0, 2048);
  //memcpy(pathtmp, path, strlen(path));
  char *qname;
	int i = 0;
  int srl = 0;
	char *token = strtok(path,".");
	while(token != NULL){
    srl = strlen(token);
  	qname[i] = srl;
	  memcpy(qname + i + 1, token, srl);
    i += srl;
    token = strtok(NULL, ".");
  }

	//determine length of each token
  return qname;

}


int main(int argc, char *argv[]) {
  /**
   * I've included some basic code for opening a socket in C, sending
   * a UDP packet, and then receiving a response (or timeout).  You'll 
   * need to fill in many of the details, but this should be enough to
   * get you started.
   */

  
  char *name;	//name of server
  char *server;			//ip address of server
  char *buffer;		//buffer for the name
	int port;				//port to connect to
    
  unsigned int length = 0;	

  // process the arguments

	if (argc > 3) {
		perror("Invalid number of arguments try again you fucking retard\n");
		return -1;
	}


  for(int i = 1; i < argc; i++){
		
    char *option = argv[i];	//input arg i

    if (option[0] == '@'){ 	
			//get the server and port values
      server = strtok(option, ":") + 1;
			option++;
      port = (int) strtok(NULL, ":"); 
			//if the port is not given, then set port to the default port
			if (port == NULL){
				port = 53;
			}
						
    }
    else{
			name = option;
   	}
		

  }
   

	
	//********************************
	//
  // construct the DNS request
	//
	//*************************************

	//setup packet
	char packet[65536]; // why this size specifically
	memset(packet, 0, 65536);

	//set up header
	dnsheader header;
  header.ID = htons(1337);
  header.QR = 0;
  header.OPCODE = 0;
  header.AA = 0;
  header.TC = 0;
  header.RD = 1;
  header.RA = 0;
  header.Z = 0;
  header.RCODE = 0;
  header.QDCOUNT = 1;
  header.ANCOUNT = 0;
  header.NSCOUNT = 0;
  header.ARCOUNT = 0;

	//move header into the packet at [0]
	memcpy(&packet, &header, sizeof(header));

	length = sizeof(header);


	printf("pre-parse name: %s\n", name);
	//move qname into packet at [96]
	char *qname = getqname(name);
	printf("qname: %s\n", qname);
	memcpy(&packet[length], qname , strlen(qname));
 
	length = length + strlen(qname) + 1;

	//setup question
	dnsquestion question;
	question.qtype = htons(1);
	question.qclass = htons(1);

	//copy the question into the packet at 97 + strlen(name)
	memcpy(&packet[length], &question, sizeof(question));

	length = length + sizeof(question);



	dump_packet(packet, length); //probably shouldnt be here
  // send the DNS request (and call dump_packet with your request)
 	// packet[96] = name;
  

	// UP TO THIS POINT IS ALL MILESTONE 1

  // first, open a UDP socket  
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // next, construct the destination address
  struct sockaddr_in out;
  out.sin_family = AF_INET;
  out.sin_port = htons( port ); 
  out.sin_addr.s_addr = inet_addr( server ); 

	//printf("packet to send: %s\n", packet);

  if (sendto(sock, &packet, sizeof(packet) , 0, &out, sizeof(out)) < 0) {
    // an error occurred
		//perror("sendto fucked up\n");
		//return -1;
  }

  // wait for the DNS reply (timeout: 5 seconds)
  struct sockaddr_in in;
  socklen_t in_len;

  // construct the socket set
  fd_set socks;
  FD_ZERO(&socks);
  FD_SET(sock, &socks);

  // construct the timeout
  struct timeval t;
  t.tv_sec = 5;
  t.tv_usec = 0;

	
  // wait to receive, or for a timeout
  if (select(sock + 1, &socks, NULL, NULL, &t)) {
    //if (recvfrom(sock, namebuff, length, 0, &in, &in_len) < 0) {
      // an error occured
    //}
  } else {
    // a timeout occurred
		printf("NORESPONSE\n");
  }


  // print out the result
	
  return 0;
}
