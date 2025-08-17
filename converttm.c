#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXLEN 2048

long last_tm, t;

int main(void)
{
	char buf[MAXLEN];
	char *p;
	struct tm *tm;
	while (fgets(buf, MAXLEN, stdin)) {
		p = buf;
		while (*p && (*p != ' '))
			p++;
		sscanf(buf, "%lu", &t);
		if (t != last_tm) {
			last_tm = t;
			tm = localtime(&t);
		}
		printf("%02d%02d %02d:%02d:%02d%s", tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, p);
	}
	return 0;
}
