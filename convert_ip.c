#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>

int main()
{
  char* IP1 = "10.101.3.130";
  char* IP2 = "10.101.3.132";
  unsigned int ip_bin;
     
  if(inet_pton(AF_INET, IP1, &ip_bin) == 0)
  {
       perror("sendto error:");
       fprintf(stderr,"%s sendto: '%s'\n", strerror(errno));        
	return 1;
  } 
  
  printf("Binary form %u\n", ip_bin);
	    
}
