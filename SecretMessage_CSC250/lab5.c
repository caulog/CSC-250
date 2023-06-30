/**
 * This program reads a pcap file and prints the secret message encoded in the ECN bit field of packets. 
 * The pcap file name is provided using command line arguments. If the file name is not provided or the 
 * file is not readable, the program will exit and provide an error message.
 *
 * @author Olivia Caulfield {@literal <caulog20@wfu.edu>}
 * @date Oct. 20, 2021
 * @assignment Lab 5
 * @course CSC 250
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <pcap.h>

int packet_OK(const unsigned char **packet, unsigned int capture_len);
void process_packet(const unsigned char *packet, int *numberBit, unsigned short int *characters, int *numChar, unsigned int *numEnd, int *checkEnd);  


int main(int argc, char *argv[]) {
    pcap_t *pcap;                       /* pcap file */
    const unsigned char *packet;        /* packet bytes */
    struct pcap_pkthdr header;          /* pcap header */
    char errbuf[PCAP_ERRBUF_SIZE];      /* buffer for error message */
    int num_pkt = 0;                    /* number of legal packets */
    int numberBit = 0;                  /* Adds one for each ECN read */
    unsigned short int characters = 0;  /* stores the char recovered from packet */
    int numChar = 0;                    /* total characters read */
    unsigned int numEnd = 0;            /* stores the last char */
    int checkEnd = 0;                   /* when :) occurs check end = 1 */

    /* we expect exactly one argument, the name of the file to dump. */
    if(argc < 2) {
        fprintf(stderr, "Usage: %s pcap_file \n", argv[0]);
        return 1;
    }

    pcap = pcap_open_offline(argv[1], errbuf);
    if(pcap == NULL) {
        fprintf(stderr, "error reading pcap file: %s\n", errbuf);
        return 1;
    }


    /* read all the packets in the pcap file */  
    while((packet = pcap_next(pcap, &header)) != NULL) {
        if(packet_OK(&packet, header.caplen)) {  
            process_packet(packet, &numberBit, &characters, &numChar, &numEnd, &checkEnd); 
            num_pkt++;  

            /* If statement excecutes to stop processing packets when :) is found */
            if (checkEnd == 1){
                break;
            }
        }
    }

    /* Print statement at end of reading packets to give stats */
    printf("Processed %d packets and recovered %u characters\n", num_pkt, numChar);

    return 0;
}


/**
 * Returns true if the packet is legit (a full capture), otherwise it returns false
 *
 * packet is a pointer to the packet pointer, it may be adjusted (hence the **)
 * capture_len is the packet length 
 **/
int packet_OK(const unsigned char **packet, unsigned int capture_len) {
    /* let's only process Ethernet frames */
    if(capture_len < sizeof(struct ether_header)) {
        return 0;
    }

    /* skip over the Ethernet frame  */
    *packet += sizeof(struct ether_header);
    capture_len -= sizeof(struct ether_header);

    /* make certain this is a legitimate packet */ 
    if(capture_len < sizeof(struct ip)) { 
        return 0;
    }

    /* we passed all the checks, so return true */  
    return 1;
}


/**
 * Processes a packet and reassembles secret characters from the ECN field
 * Most of the work in this program is done here...
 **/
void process_packet(const unsigned char *packet, int *numberBit, unsigned short int *characters, int *numChar, unsigned int *numEnd, int *checkEnd){
    unsigned int readECN = packet[1]; /* read from first packet */
    unsigned int actualECN = readECN & 3; /* shifts so only last two bits are read from ECN */
    *numberBit = *numberBit +1; 
    *characters = *characters + (actualECN << ((4-*numberBit)*2)); /* pushes it back to the character */

    if(*numberBit == 4){
        printf("%c", *characters); /*Prints characters to the screen*/

        /* These if statements check for the end of the message by searching for :)*/
        if(*numEnd == 58){
            if (*characters == 41){
                /* returns checkEnd as 1 if the :) is found */
                *checkEnd = 1;
                printf("\n");
            }
        }

        /* resets variables and returns the numEnd as character */
        *numEnd = *characters;
        *numberBit = 0;
        *characters = 0;
        *numChar = *numChar +1; /* records the number of characters processed */
    }   

}


