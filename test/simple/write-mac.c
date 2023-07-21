#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>

#include <linux/if.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/udp.h>
#include <linux/if_packet.h>

#define IF_NAME		"eth0"
#define MAC_FILE	"mac.txt"

int share_mac(){
    unsigned char mac[6] = {0};
    int sock_raw;
    FILE* out;

    sock_raw = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
    if (sock_raw == -1) {
        perror("socket()");
        return -1;
    }

    struct ifreq if_req;
    memset(&if_req, 0, sizeof(struct ifreq));
    strncpy(if_req.ifr_name, IF_NAME, IFNAMSIZ - 1);

    if ((ioctl(sock_raw, SIOCGIFHWADDR, &if_req)) < 0) {
        perror("ioctl() with SIOCGIFHWADDR");
        return -1;
    }
    int i;
    for (i = 0; i < 6; ++i) mac[i] = (unsigned char)(if_req.ifr_hwaddr.sa_data[i]);

    printf("Mac = %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    out = fopen(MAC_FILE, "w");
    if (!out) {
        printf("Unable to open %s\n", MAC_FILE);
        close(sock_raw);
        return -1;
    }
    fprintf(out, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    close(sock_raw);
    fclose(out);
    return 0;
}

/*****************************************************
* Main 
*****************************************************/
int main() {
    share_mac();
    return 0;
}
