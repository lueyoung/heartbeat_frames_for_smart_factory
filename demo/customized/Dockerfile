FROM ubuntu:20.04 as build
RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
RUN mkdir -p /workspace
RUN apt-get update
RUN apt-get install -y gcc
ADD src/recver.c /workspace/
ADD src/sender.c /workspace/
WORKDIR /workspace
RUN gcc -Wall sender.c -o send 
RUN gcc -Wall recver.c -o recv

FROM ubuntu:20.04
RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
COPY --from=build /workspace/recv /
COPY --from=build /workspace/send /
