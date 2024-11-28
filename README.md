
## 1. gzlog 从stdin收集文本内容，按小时或按天压缩存储

命令行：
```
gzlog -p file_prefix [ -t | -f | -d | -h ]
  options:
    -p file_prefix      log file prefix
    -t                  add hour min sec timestamp
    -f                  add year mon day hour min sec timestamp
    -d                  change file every day (default)
    -h                  change file every hour
```
如
```
gzlog -p /natlog/natlog 
```
从stdin收到的日志，存放到/nat/log/natlog开头的文件，每天一个压缩文件

## 2. natlog.sh 记录conntrack日志
使用方式：(需提前安装screen conntrack-tools)

在/etc/rc.d/rc.local中执行
```
screen -S natlog -d -m /usr/src/gzlog/natlog.sh
```
conntrack日志记录在/natlog/natlog.* 文件

默认记录了NEW、UPDATE、DESTROY 3种事件，如果想减少日志文件大小，可以使用如下命令启动natlog.sh仅仅记录需要的事件。
```
screen -S natlog -d -m /usr/src/gzlog/natlog.sh -e NEW,DESTROY
```

## 3. dnslog.sh 记录DNS查询日志

使用方式：(需提前安装screen tcpdump)

在/etc/rc.d/rc.local中执行
```
screen -S dnslog -d -m /usr/src/gzlog/dnslog.sh eth0
```
eth0的DNS查询日志记录在/natlog/dns.log.* 文件

## 4. 过时日志文件的清理

请参考crontab.txt
