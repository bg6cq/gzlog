all: gzlog natlog_to_flowlog converttm

gzlog: gzlog.c
	gcc -o gzlog -g -Wall gzlog.c 

natlog_to_flowlog: natlog_to_flowlog.c
	gcc -o natlog_to_flowlog -g -Wall natlog_to_flowlog.c

converttm: converttm.c
	gcc -o converttm -g -Wall converttm.c

indent: gzlog.c
	indent gzlog.c natlog_to_flowlog.c converttm.c -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 \
		-cli0 -d0 -di1 -nfc1 -i8 -ip0 -l160 -lp -npcs -nprs -npsl -sai \
		-saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1
