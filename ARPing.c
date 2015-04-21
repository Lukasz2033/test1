#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ETH_FRAME_LEN 1504
#define ETH_ALEN 6

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

typedef u8 ethernet_frame[ETH_FRAME_LEN];

char* IP1 = "10.101.3.130";
char* IP2 = "10.101.3.132";
  
typedef struct configuration{
        u8  src_mac[6];
        u8  dst_mac[6];    
        u32 src_ip;
        u32 dst_ip;    
}cfg;

typedef struct arp_message{
	u16 hrd_val;
	u16 pro_typ;
	u8 hln;
	u8 pln;
	u16 op;
	u8 src_mac[6];
	u32 src_ip;
	u8 dst_mac[6];
	u32 dst_ip;
}arp_h;

void print_frame(u8 *f, u32 l){

u16 c;
for(c=0; c<l; c++)
    printf("%02x\n", f[c]);
printf("\n");

}

int main()
{
    printf("Creating ARP packet\n");

    ethernet_frame packet;
    u8 *frame, *begin;
    frame = begin = packet;
    arp_h *arp_p;
    cfg config;
    
    memset(&packet, 0, ETH_FRAME_LEN);
    
    //Convert IP string to decimal form
    if(inet_pton(AF_INET, IP1, &config.src_ip) == 0){
        perror("sendto error:");
        fprintf(stderr,"%s sendto: '%s'\n", strerror(errno));        
    return 1;} 
    
    if(inet_pton(AF_INET, IP1, &config.dst_ip) == 0){
        perror("sendto error:");
        fprintf(stderr,"%s sendto: '%s'\n", strerror(errno));        
    return 1;} 
    // Broadcast MAC
    config.dst_mac[0] = 0xFF;
    config.dst_mac[1] = 0xFF;
    config.dst_mac[2] = 0xFF;
    config.dst_mac[3] = 0xFF;
    config.dst_mac[4] = 0xFF;
    config.dst_mac[5] = 0xFF;
    
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strcpy(s.ifr_name, "eth1");
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        int i;
        for (i = 0; i < 6; ++i)
            config.src_mac[i] = s.ifr_addr.sa_data[i];
    }
    else{
        perror("sendto error:");
        fprintf(stderr,"%s sendto: '%s'\n", strerror(errno)); 
        return 1;}

    memcpy(frame, (void*)config.dst_mac, ETH_ALEN);
    frame += ETH_ALEN;
    
    memcpy(frame, (void*)config.src_mac, ETH_ALEN);
    frame += ETH_ALEN;
    
    u16 temp = 0x8100;                   //htons(0x8100); //IEEE 802.1Q-tagged frame
    memcpy(frame, &temp, 2);
    frame += 2;
    
    temp = 2;                            //htons(2);
    memcpy(frame, &temp, 2);
    frame += 2;
    
    temp = 0x0800;                       //htons(0x0800);
    memcpy(frame, &temp, 2);
    frame += 2;

    arp_p = (arp_h *)frame;
    arp_p->hrd_val = 0x0006;
    arp_p->pro_typ = 0x0800;
    arp_p->hln = 6;
    arp_p->pln = 4;
    arp_p->op = 0x1;
    memcpy(arp_p->src_mac, &config.src_mac, 6);
	arp_p->src_ip = config.src_ip;
	memcpy(arp_p->dst_mac, &config.dst_mac, 6);
	arp_p->dst_ip = config.dst_ip;
   
   
    print_frame(packet, 50);
    
    return 0;   

}
