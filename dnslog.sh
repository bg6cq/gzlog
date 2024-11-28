#!/bin/bash

# screen -S dnslog -d -m /usr/src/gzlog/dnslog.sh eth0
 
DEV=$1

cd /usr/src/gzlog

while true; do
	echo `date` dnslog starting >> run.log
	tcpdump -i $DEV -nn udp port 53 | /usr/src/gzlog/gzlog -p /natlog/dns.log
	sleep 1;
done
