## Simple Demo
### 0. Prerequisites
* Install `docker`;
* Install `docker-compose`.
### 1. How to run?
To run this demo, you just need to run
```
docker-compose up -d
```
and then, two containers(recver and sender) are up.
### 2. Review the stdout
Run
```
docker logs -f recver
```
to see the stdout of container `recver`;   
run
```
docker logs -f sender
```
to see the stdout of container `sender`.  
Or read log files:
```
docker exec -it recver tail -f /udp_packets.log
docker exec -it sender tail -f /udp_packets.log
```
