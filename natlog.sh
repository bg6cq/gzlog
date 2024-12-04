#!/bin/bash

# screen -S natlog -d -m /usr/src/gzlog/natlog.sh

cd /usr/src/gzlog

while true; do
        echo `date` natlog starting >> run.log
	conntrack -E $@ -b 10240000 | /usr/src/gzlog/gzlog -p /natlog/natlog -t
	sleep 1
done
