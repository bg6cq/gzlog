#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXLEN 2048

char TIME[MAXLEN], PROTO[MAXLEN], PROTONUM[MAXLEN], SIP[MAXLEN], DIP[MAXLEN];
char SPORT[MAXLEN], DPORT[MAXLEN], PKTS[MAXLEN], BYTES[MAXLEN];
char NATSIP[MAXLEN], NATDIP[MAXLEN];
char NATSPORT[MAXLEN], NATDPORT[MAXLEN], RPKTS[MAXLEN], RBYTES[MAXLEN];

char *copy_str_to(char *dst, char *src)
{
	char *p = src;
	char *d = dst;
	while (*p && (*p != ' ')) {
		*d = *p;
		d++;
		p++;
	}
	*d = 0;
	while (*p == ' ')
		p++;
	return p;
}

int main(void)
{
	char buf[MAXLEN];
	char *p;
	while (fgets(buf, MAXLEN, stdin)) {
		strcpy(SPORT, "0");
		strcpy(DPORT, "0");
		strcpy(NATSPORT, "0");
		strcpy(NATDPORT, "0");
		int len = strlen(buf);
		if (len < 20)
			continue;
		if (buf[len - 1] == '\n') {
			len--;
			buf[len] = 0;
		}
		sprintf(TIME, "%lu", (unsigned long)time(NULL));
		p = buf + 9;

		p = copy_str_to(PROTO, p);	// skip first [DESTROY] 
		p = copy_str_to(PROTO, p);
		p = copy_str_to(PROTONUM, p);
		if ((TIME[0] == 0) || (PROTO[0] == 0) || (PROTONUM[0] == 0))
			continue;
		if (strncmp(p, "src=", 4) != 0)
			continue;
		p = p + 4;
		p = copy_str_to(SIP, p);
		if (strncmp(p, "dst=", 4) != 0)
			continue;
		p = p + 4;
		p = copy_str_to(DIP, p);
		if (strncmp(p, "sport=", 6) == 0) {
			p = p + 6;
			p = copy_str_to(SPORT, p);
			if (strncmp(p, "dport=", 6) == 0) {
				p = p + 6;
				p = copy_str_to(DPORT, p);
			} else
				continue;
		} else if (strncmp(p, "type=", 5) == 0) {
			p = p + 5;
			p = copy_str_to(SPORT, p);
			if (strncmp(p, "code=", 5) == 0) {
				p = p + 5;
				p = copy_str_to(DPORT, p);
			} else
				continue;
			p = strstr(p, "packets=");
			if (p == NULL)
				continue;
		} else {
			strcpy(SPORT, "0");
			strcpy(DPORT, "0");
		}
		if (strncmp(p, "packets=", 8) != 0)
			continue;
		p = p + 8;
		p = copy_str_to(PKTS, p);
		if (strncmp(p, "bytes=", 6) != 0)
			continue;
		p = p + 6;
		p = copy_str_to(BYTES, p);

		p = strstr(p, "src=");
		if (p == NULL)
			continue;

		if (strncmp(p, "src=", 4) != 0)
			continue;
		p = p + 4;
		p = copy_str_to(NATDIP, p);
		if (strncmp(p, "dst=", 4) != 0)
			continue;
		p = p + 4;
		p = copy_str_to(NATSIP, p);
		if (strncmp(p, "sport=", 6) == 0) {
			p = p + 6;
			p = copy_str_to(NATDPORT, p);
			if (strncmp(p, "dport=", 6) == 0) {
				p = p + 6;
				p = copy_str_to(NATSPORT, p);
			} else
				continue;
		} else if (strncmp(p, "type=", 5) == 0) {
			p = p + 5;
			p = copy_str_to(NATSPORT, p);
			if (strncmp(p, "code=", 5) == 0) {
				p = p + 5;
				p = copy_str_to(NATDPORT, p);
			} else
				continue;
			p = strstr(p, "packets=");
			if (p == NULL)
				continue;
		} else {
			strcpy(NATSPORT, "0");
			strcpy(NATDPORT, "0");
		}
		if (strncmp(p, "packets=", 8) != 0)
			continue;
		p = p + 8;
		p = copy_str_to(RPKTS, p);
		if (strncmp(p, "bytes=", 6) != 0)
			continue;
		p = p + 6;
		p = copy_str_to(RBYTES, p);

		printf("%s %s %s %s %s "
		       "%s %s %s %s %s "
		       "%s %s %s %s %s\n", TIME, PROTO, PROTONUM, SIP, SPORT, DIP, DPORT, NATSIP, NATSPORT, NATDIP, NATDPORT, PKTS, BYTES, RPKTS, RBYTES);
	}
	return 0;
}
