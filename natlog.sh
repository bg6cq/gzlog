#!/bin/bash

# screen -S natlog -d -m /usr/src/gzlog/natlog.sh

# echo 1 > /proc/sys/net/netfilter/nf_conntrack_acct

cd /usr/src/gzlog

while true; do
        echo `date` natlog starting >> run.log
	conntrack -E $@ -b 20480000 | /usr/src/gzlog/natlog_to_flowlog | /usr/src/gzlog/gzlog -p /natlog/natlog
	sleep 1
done
