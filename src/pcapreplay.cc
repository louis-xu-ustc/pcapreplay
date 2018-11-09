//#include <pcap.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define TIMEVAL_DIFF(a,b) ((a.tv_sec - b.tv_sec) + ((a.tv_usec - b.tv_usec)/1000000.0))
#define TIMEVAL_TO_DOUBLE(a) ((a.tv_sec) + (a.tv_usec / 1000000.0))

#define PCAP_OFFSET 42

int skipTime_ = 0;

int main(int argc, char *argv[])
{

    static const char usage[] = 
      " [-s] [-p port] pcap\n"
      "\n"
      "  -s          enable skip time\n"
      "  -p port     specify the port number\n";

    pcap_t *pcap;
    const unsigned char *packet;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    int opt;
    int portNum_ = 50101;


    while ((opt = getopt(argc, argv, "sp:")) != -1)
    {
        switch (opt)
        {
            case 's':
                skipTime_ = 1;
                break;
            case 'p':
                portNum_ = std::stoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << usage << std::endl;
                return -1;
        }
    }
    

    pcap = pcap_open_offline(argv[1],errbuf);
    if(pcap == NULL)
    {
        fprintf(stderr,"error reading pcap file: %s\n",errbuf);
        return -1;
    }

    int sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd_ < 0)
    {
        perror("Error creating socket");
        exit(-1);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(portNum_);

    struct timeval last_ts;
    struct timeval last_tx;
    struct timeval currts;

    last_ts.tv_sec = -1;
    last_tx.tv_sec = -1;

    while((packet = pcap_next(pcap, &header)) != NULL)
    {
        //            for(int i=0; i<header.len; i++)
        //            {
        //                printf("%02X ",(unsigned int)packet[i]);
        //                if((i+1)%10 == 0)
        //                    printf("\n");
        //            }
        //            printf("\n");

        //            printf("Packet length=%d\n",header.len);

	if(!skipTime_)
	{
	        struct timeval ts = header.ts;
	        gettimeofday(&currts,NULL);

	        double d_tx = TIMEVAL_DIFF(currts,last_tx);
	        double d_rx = TIMEVAL_DIFF(ts,last_ts);

	        while((d_tx < d_rx) && (last_ts.tv_sec != -1))
	        {
	            gettimeofday(&currts,NULL);
	            d_tx = TIMEVAL_DIFF(currts,last_tx);
	        }

	        last_ts = ts;
	}

        unsigned int ret = sendto(sockfd_,packet+PCAP_OFFSET,header.len-PCAP_OFFSET,0,(struct sockaddr*)&sa,sizeof(sa));
        if(ret != header.len-PCAP_OFFSET)
        {
            printf("Error sending UDP. ret=%d, exp=%d",ret,header.len);
        }
        else
        {
            fprintf(stdout,"ts=%4.6f Sent %d bytes over UDP\n",TIMEVAL_TO_DOUBLE(currts),ret);
            fflush(stdout);
        }

        gettimeofday(&last_tx,NULL);
    }

    return 0;
}
