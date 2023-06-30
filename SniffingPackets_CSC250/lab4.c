/**
 * This program reads a pcap file and prints the a summery of each packet heder, then summary statistics
 * about all the packets. top 5 most frequent letters (upper or lower case). The pcap file name is 
 * provided using command line arguments. If the file name is not provided or the file is not readable, 
 * the program will exit and provide an error message.
 *
 * @author Olivia Caulfield {@literal <caulog20@wfu.edu>}
 * @date Oct. 6, 2021
 * @assignment Lab 4
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
void process_packet(const unsigned char *packet, int *numTCP, int *numUDP, double *avgTLen, double *avgTTL);  


int main(int argc, char *argv[]) {
    pcap_t *pcap;                    /* pcap file */
    const unsigned char *packet;     /* packet bytes */
    struct pcap_pkthdr header;       /* pcap header */
    char errbuf[PCAP_ERRBUF_SIZE];   /* buffer for error message */
    int num_pkt = 0;                 /* number of legal packets */
    int numTCP = 0;                  /* number of TCP packet protocol */
    int numUDP = 0;                  /* number of UDP packet protocol */
    double avgTLen = 0;              /* average total length of packet */
    double avgTTL = 0;               /* average time to live */

    /* we expect exactly one argument, the name of the pcap file */
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
            process_packet(packet, &numTCP, &numUDP, &avgTLen, &avgTTL); 
            num_pkt++;  
        }
    }

    /* print statement format */
    printf("---------------------------------------------------------------------------------------- \n");
    printf("%s, num pkt: %d, num TCP: %u, num UDP: %u, avg total len: %.1f, avg TTL: %.1f\n", argv[1], num_pkt, numTCP, numUDP, (avgTLen/num_pkt), (avgTTL/num_pkt));  

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
 * Processes a packet, displays the packst summary, and updates the pcap stats
 * Most of the work for this program is done here...
 **/
void process_packet(const unsigned char *packet, int *numTCP, int *numUDP, double *avgTLen, double *avgTTL) {
    /* create local variables equal to the passed pointers */
    int num_TCP = *numTCP;
    int num_UDP = *numUDP;
    double avg_TLen = *avgTLen;
    double avg_TTL = *avgTTL;

    /* get ttl */
    unsigned char ttl = packet[8];

    /* gets hlen from packet 0 */
    unsigned char hlen = (packet[0] & 0x0F) << 2;

    /* gets df and mf from packet 6 */
    unsigned char df = (packet[6] & 0x40) >> 6;
    unsigned char mf = (packet[6] & 0x20) >> 5;

    /* reads the proto from packet 4 and checks if UDP or TCP */
    char proto[4] = {'x', 'x', 'x', '\0'};
    if (packet[9] == 0x06){
        proto[0] = 'T';
        proto[1] = 'C';
        proto[2] = 'P';
        num_TCP++;
        *numTCP = num_TCP;
    }
    else{
        proto[0] = 'U';
        proto[1] = 'D';
        proto[2] = 'P';
        num_UDP++;
        *numUDP = num_UDP;
    }

    /* count of TTL to return to pointer */
    avg_TTL = avg_TTL + packet[8];
    *avgTTL = avg_TTL;

    /* count of Total length to return to pointer */
    avg_TLen = avg_TLen + packet[3];
    *avgTLen = avg_TLen; 

    /* print format for each payload header */
    printf("%u.%u.%u.%u -> %u.%u.%u.%u", packet[12], packet[13], packet[14], packet[15], packet[16], packet[17], packet[18], packet[19]);
    printf(" [hlen: %u, tlen: %u, DF: %u, MF: %u, TTL: %d, proto: %s] \n", hlen, packet[3], df, mf, ttl, proto);  
}


