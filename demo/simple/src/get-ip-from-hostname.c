
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //for exit(0);
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>
/*
Get ip from domain name (gethostbyname)
    int hostname_to_ip(char *  , char *);
 */
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    if ( (he = gethostbyname( hostname ) ) == NULL) {
        perror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    int i;
    for(i = 0; addr_list[i] != NULL; i++) {
        strncat(ip, inet_ntoa(*addr_list[i]), strlen(inet_ntoa(*addr_list[i])));
        //strncat(ip, inet_ntoa(*addr_list[i]), strlen(ip)-1);
        if(addr_list[i+1] != NULL)
            strncat(ip, ",", 1);
        printf("%s\n", inet_ntoa(*addr_list[i]));
        printf("ip lst: %s\n", ip);
    }
    return 0;
}
int main(int argc , char *argv[])
{
    if(argc <2) {
        printf("Please provide a hostname to resolve.\n");
        exit(1);
    }
    char *hostname = argv[1];
    char ip[100];
    hostname_to_ip(hostname, ip);
    printf("\n%s resolved to %s\n", hostname, ip);
    return 0;
}
