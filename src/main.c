#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
#include<arpa/inet.h>

#include<netinet/if_ether.h>
#include<netinet/ip.h>
*/
#include<netinet/in.h>
#include"initrawsock.h"
/*
struct ip_packet {
	struct ether_header eth_hdr;
	struct ip ip_hdr; 
};
*/

#if 1
//u_int16_t packet[1000] = {
//	0xffff, 0xffff, 0xffff, 0xdeb4,
//	0x6b16, 0x904c, 0x0800, 0x4500,	//up to service type
//	0x0014, 0x1000, 0x4000, 0x4006, //length, ID, Flags, offset, TTL, proto
//	0x16e2, 0x0a00, 0x0002, 0x0a00,
//	0x0001
//};
//source ip 10.0.0.2
//dest ip 10.0.0.1
#else
u_int16_t packet[1000] = {
	0xdeb4, 0x6b16, 0x904c, 0xffff,
	0xffff, 0xffff, 0x0800, 0x4500,	//up to service type
	0x0014, 0x1000, 0x4000, 0x4006, //length, ID, Flags, offset, TTL, proto
	0x16e2, 0x0a00, 0x0002, 0x0a00,
	0x0001
};
#endif

u_int16_t icmp_packet[50] = {
	0x0036, 0x9f3e, 0xa08a, 0xa036,
	0x9f3f, 0x6202, 0x0800, 0x4500,
	0x0054, 0x670b, 0x4000, 0x4001,
	0xbf91, 0x0a00, 0x0007, 0x0a00,
	0x0006, 0x0800, 0x3687, 0x7ca3,
	0x0001, 0x31d1, 0xdd5a, 0x0000,
	0x0000
};


u_int16_t arp_packet[100] = {
	0xffff, 0xffff, 0xffff, 0xdeb4,
	0x6b16, 0x904c, 0x0806, 
};


int main(int argc, char **argv){
	int sock, size, sock_eth3, sock_eth2;
	u_char buf[2048];

	//if (argc < 2){ 
  //  fprintf(stderr, "usage: ./main [dev-name]\n");
  //  exit(1);
  //}

  //if ((sock = initrawsock(argv[1], 1, 0)) < 0){ 
  if ((sock_eth3 = initrawsock("eth3", 1, 0)) < 0){ 
    //fprintf(stderr, "InitRawSocket:error:%s\n", argv[1]);
    fprintf(stderr, "InitRawSocket:error:eth3\n");
    exit(1);
  }
  if ((sock_eth2 = initrawsock("eth2", 1, 0)) < 0){ 
    //fprintf(stderr, "InitRawSocket:error:%s\n", argv[1]);
    fprintf(stderr, "InitRawSocket:error:eth3\n");
    exit(1);
  }

	//struct ip_packet packet;
	//write(sock, icmp_packet, strlen(icmp_packet));

	//struct sockaddr_in to;
	//to.sin_addr.s_addr = (u_long)0x0a000006;
	//to.sin_family = AF_INET;
	//to.sin_port = ;


	if (((size = read(sock_eth3, buf, sizeof(buf))) <= 0)){
		fprintf(stderr, "readerr\n");
		exit(1);
	}
	printf("read success\n");

	int n;
#if 1
	//if ((n = write(sock, icmp_packet, strlen(icmp_packet))) <= 0){
	if ((n = write(sock_eth2, buf, size)) <= 0){
		fprintf(stdout, "can not send packet\n");
		exit(1);
	}
	//fflush(sock_eth2);
	printf("write success\n");

#else
	if ((n = sendto(sock, (char *)icmp_packet, strlen(icmp_packet), 0, (struct sockaddr *)&to, sizeof(to))) <= 0){
		fprintf(stdout, "can not send packet\n");
		exit(1);
	}
#endif
	
	close(sock_eth2);
	close(sock_eth3);
}
