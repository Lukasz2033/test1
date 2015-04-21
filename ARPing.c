#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <netinet/in.h>

#define ETH_FRAME_LEN 1504
#define ETH_ALEN 6

typedef unsigned char ethernet_frame[ETH_FRAME_LEN];

ethernet_frame packet;

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

u8* frame, begin;

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

arp_h* arp_p;

int main()
{
    printf("Creating ARP packet\n");
    frame = begin = packet;
    
    // 
    cfg config;
    
    config.dst_mac[0] = 0xFF;
    config.dst_mac[1] = 0xFF;
    config.dst_mac[2] = 0xFF;
    config.dst_mac[3] = 0xFF;
    config.dst_mac[4] = 0xFF;
    config.dst_mac[5] = 0xFF;
    
    config.src_mac[0] = 0xFF;
    config.src_mac[1] = 0xFF;
    config.src_mac[2] = 0xFF;
    config.src_mac[3] = 0xFF;
    config.src_mac[4] = 0xFF;
    config.src_mac[5] = 0xFF;
    
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

    arp_p = (*arp_h)frame;
    arp_p->hrd_val = 0x0006;
    arp_p->pro_typ =0x0800;
    arp_p->hln = 6;
    arp_p->pln = 4;
    arp_p->op = 0x1;
  
	
    system("PAUSE");
    return 0;   
}
