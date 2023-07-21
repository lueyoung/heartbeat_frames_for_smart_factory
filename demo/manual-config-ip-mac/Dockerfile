FROM ubuntu:16.04

RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime

RUN apt-get update
RUN apt-get install -y vim net-tools inetutils-ping

RUN apt-get clean && \ 
    rm -rf /var/lib/apt/lists/* && \
    rm -rf /tmp/* /usr/share/man /usr/share/doc 

RUN mkdir -p /workspace
ADD src/recver.c /workspace/
ADD src/sender.c /workspace/
WORKDIR /workspace

ADD entrypoint.sh /
RUN chmod a+x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
