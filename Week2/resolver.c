#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

int get_ip(char * hostname , char* ip) 
{
	struct hostent *he;     
    struct in_addr **addr_list;     
    int i;     
    if ((he = gethostbyname(hostname)) == NULL)     
    {
    	//herror("gethostbyname");         
    	printf("Not found information1.\n");
    	ip = NULL;
    	return 1;
    }     
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++)
    {
    	strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    return 1;
}

int get_hostname(char *hostname, char *ip)
{
	struct in_addr addr;
	struct hostent* esu;
    inet_aton(ip, &addr);
    //printf("%s\n", addr);
    esu = gethostbyaddr(((const char*)&addr),sizeof(addr), AF_INET);
    if(esu == NULL)
    {
    	printf("Not found information2.\n");
    	hostname = NULL;
    	return 1;
    }
    strcpy(hostname, esu->h_name);
    return 1;
    //printf("%s\n", esu->h_name);
    //return 0;
}

int get_ip(char *  , char *);
int get_hostname(char *, char *);

int main(int argc , char *argv[]) {
	char ip[100];
	char * option = argv[1];
	if (strcmp(option,"1") == 0)
	{
		char *ip1 = argv[2];
		char hostName1[100];
		get_hostname(hostName1, ip1);
		if(hostName1 != NULL)
        {
    		printf("Hostname: %s\n", hostName1);
        }
	}
	else if (strcmp(option,"2") == 0)
	{
		char *hostName2 = argv[2];
		char ip2[100];
		get_ip(hostName2, ip2);
		if(ip2 != NULL)
        {
    		printf("IP: %s\n", ip2);
        }
	}
	
	return 0;
}