#!/bin/bash

echo 'root:9ol.8ik,' | chpasswd 
#echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config
sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/g' /etc/ssh/sshd_config

/etc/init.d/ssh restart

tail -f /dev/null
