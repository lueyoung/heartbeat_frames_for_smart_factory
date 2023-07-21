#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MAGIC "1234567890"
#define MAGIC_LEN 11
#define IP_BUFFER_SIZE 65536
#define RECV_TIMEOUT_USEC 100000
/*
__attrubte__ ((packed)) 的作用就是告诉编译器取消结构在编译过程中的优化对齐,按照实际占用字节数进行对齐
*/

//定义ICMP回环结构体 
struct __attribute__((__packed__)) icmp_echo {
    // header
    uint8_t type; //类型 
    uint8_t code; //代码 
    uint16_t checksum;//校验和 

    uint16_t ident;//标识符 
    uint16_t seq; //符号 

    // data
    double sending_ts;//发送时间 
    char magic[MAGIC_LEN]; //字符串 
};
// 获取时间戳

double get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + ((double)tv.tv_usec) / 1000000;
}
//计算校验和

uint16_t calculate_checksum(unsigned char* buffer, int bytes) {
    uint32_t checksum = 0;
    unsigned char* end = buffer + bytes;

    // odd bytes add last byte and reset end
    if (bytes % 2 == 1) {
        end = buffer + bytes - 1;
        checksum += (*end) << 8;
    }

    // add words of two bytes, one by one
    while (buffer < end) {
        checksum += (buffer[0] << 8) + buffer[1];

        // add carry if any
        uint32_t carray = checksum >> 16;
        if (carray != 0) {
            checksum = (checksum & 0xffff) + carray;
        }

        buffer += 2;
    }
    // negate it
    checksum = ~checksum;

    return checksum & 0xffff;
}
//发送回显请求 
int send_echo_request(int sock, struct sockaddr_in* addr, int ident, int seq) {
    // allocate memory for icmp packet
    struct icmp_echo icmp;
    bzero(&icmp, sizeof(icmp));

    // fill header files
    icmp.type = 8;
    icmp.code = 0;
    icmp.ident = htons(ident);
    icmp.seq = htons(seq);

    // fill magic string
    strncpy(icmp.magic, MAGIC, MAGIC_LEN);

    // fill sending timestamp
    icmp.sending_ts = get_timestamp();

    // calculate and fill checksum
    icmp.checksum = htons(
        calculate_checksum((unsigned char*)&icmp, sizeof(icmp))
    );

    // send it
    /*
        sendto() 用来将数据由指定的 socket 传给对方主机
        参数1：socket文件描述符
        参数2：发送数据的首地址
        参数3：数据长度
        参数4：默认方式发送
        参数5：存放目的主机的IP和端口信息
        参数6: 参数5的长度 
    */
    int bytes = sendto(sock, &icmp, sizeof(icmp), 0,(struct sockaddr*)addr, sizeof(*addr));
    if (bytes == -1) {
        return -1;
    }
    return 0;
}
//实现recv_echo_reply用于接收ICMP回显应答报文
int recv_echo_reply(int sock, int ident) {
    // allocate buffer
    //定义缓冲区 
    unsigned char buffer[IP_BUFFER_SIZE];
    //sockaddr_in 结构体
    struct sockaddr_in peer_addr;

    // receive another packet
    int addr_len = sizeof(peer_addr);
    /*
        recvfrom()本函数用于从（已连接）套接口上接收数据，并捕获数据发送源的地址 
        s：标识一个已连接套接口的描述字。 
        buf：接收数据缓冲区。 
        len：缓冲区长度。 
        flags：调用操作方式。 
        from：（可选）指针，指向装有源地址的缓冲区。 
        fromlen：（可选）指针，指向from缓冲区长度值。 
    */
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0,
        (struct sockaddr*)&peer_addr, &addr_len);
    if (bytes == -1) {
        // normal return when timeout
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }

        return -1;
    }
    //IP头部长度 
    int ip_header_len = (buffer[0] & 0xf) << 2;
    // find icmp packet in ip packet
    //从 IP 报文中取出 ICMP 报文
    struct icmp_echo* icmp = (struct icmp_echo*)(buffer + ip_header_len);

    // check type
    if (icmp->type != 0 || icmp->code != 0) {
        return 0;
    }

    // match identifier
    //ntohs()是一个函数名，作用是将一个16位数由网络字节顺序转换为主机字节顺序
    if (ntohs(icmp->ident) != ident) {
        return 0;
    }

    // print info
    printf("%s seq=%-5d %8.2fms\n",
        inet_ntoa(peer_addr.sin_addr),
        ntohs(icmp->seq),
        (get_timestamp() - icmp->sending_ts) * 1000
    );

    return 0;
}

int ping(const char *ip) {
    // for store destination address
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    // fill address, set port to 0
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    //inet_aton是一个计算机函数，功能是将一个字符串IP地址转换为一个32位的网络序列IP地址。
    if (inet_aton(ip, (struct in_addr*)&addr.sin_addr.s_addr) == 0) {
        fprintf(stderr, "bad ip address: %s\n", ip);
        return -1;
    };

    // create raw socket for icmp protocol
    //创建一个原始套接字，协议类型为 IPPROTO_ICMP 
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1) {
        perror("create raw socket");
        return -1;
    }

    // set socket timeout option
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = RECV_TIMEOUT_USEC;
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret == -1) {
        perror("set socket option");
        close(sock);
        return -1;
    }

    double next_ts = get_timestamp();
    int ident = getpid();//取得进程识别码
    int seq = 1;

    for (;;) {
        // time to send another packet
        double current_ts = get_timestamp();
        if (current_ts >= next_ts) {
            // send it
            ret = send_echo_request(sock, &addr, ident, seq);
            if (ret == -1) {
                perror("Send failed");
            }

            // update next sendint timestamp to one second later
            next_ts = current_ts + 1;
            // increase sequence number
            seq += 1;
        }

        // try to receive and print reply
        ret = recv_echo_reply(sock, ident);
        if (ret == -1) {
            perror("Receive failed");
        }
    }

    close(sock);

    return 0;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "no host specified");
        return -1;
    }
    return ping(argv[1]);
}
