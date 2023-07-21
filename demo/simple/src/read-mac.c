#include <stdio.h>
#include <unistd.h>

#define MAC_FILE "mac.txt"

int read_mac(){
    FILE* in = NULL;
    char* buf[255];
    unsigned char mac[6] = {0};
    int retry = 0;

    in = fopen(MAC_FILE, "r");

    while (!in) {
        in = fopen(MAC_FILE, "r");
        if (retry > 3){
            break;
        }
        retry++;
        sleep(1);
    }
    if (!in) {
        printf("Unable to open %s\n", MAC_FILE);
        return -1;
    }
    fscanf(in, "%s", buf);

    //printf("%s", buf);

    int values[6];
    int i = 0;
    if (6 == sscanf(buf, "%x:%x:%x:%x:%x:%x%*c", &values[0], &values[1], &values[2], 
                                                 &values[3], &values[4], &values[5])){
        for (i=0; i<6; i++){
            mac[i] = (unsigned char) values[i];
        }
    }

    printf("Mac = %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    fclose(in);
    return 0;
}

int main(){
    read_mac();
    return 0;
}
