#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 16384

char file_prefix[MAXLEN];
int timestamp_t = 0;
int timestamp_f = 0;
int change_every_day = 1;
int change_every_hour = 0;

FILE *fp = NULL;

void signal_handler(int sig)
{
	if (sig == SIGHUP) {
		if (fp)
			pclose(fp);
		exit(0);
	}
}

void changefile(struct tm *ctm)
{
	char fnbuf[MAXLEN + 101];
	if (fp)
		pclose(fp);
	snprintf(fnbuf, MAXLEN + 100, "gzip > %s.%04d.%02d.%02d.%02d%02d%02d.gz",
		 file_prefix, ctm->tm_year + 1900, ctm->tm_mon + 1, ctm->tm_mday, ctm->tm_hour, ctm->tm_min, ctm->tm_sec);
	fp = popen(fnbuf, "w");
	if (fp == NULL) {
		fprintf(stderr, "popen %s for write error\n", fnbuf);
		exit(-1);
	}
}

void Log(char *s)
{
	time_t t;
	static time_t last_t = 0;
	static int tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec;
	time(&t);
	if (t != last_t) {
		static int lastday = -1;
		static int lasthour = -1;
		struct tm *ctm;
		last_t = t;
		ctm = localtime(&t);
		tm_year = ctm->tm_year;
		tm_mon = ctm->tm_mon;
		tm_mday = ctm->tm_mday;
		tm_hour = ctm->tm_hour;
		tm_min = ctm->tm_min;
		tm_sec = ctm->tm_sec;
		if (change_every_day) {
			if (ctm->tm_mday != lastday) {
				changefile(ctm);
				lastday = ctm->tm_mday;
			}
		}
		if (change_every_hour) {
			if (ctm->tm_hour != lasthour) {
				changefile(ctm);
				lasthour = ctm->tm_hour;
			}
		}

	}
	int len;
	if (timestamp_f)
		len = fprintf(fp, "%04d.%02d.%02d %02d:%02d:%02d %s\n", tm_year + 1900, tm_mon + 1, tm_mday, tm_hour, tm_min, tm_sec, s);
	else if (timestamp_t)
		len = fprintf(fp, "%02d:%02d:%02d %s\n", tm_hour, tm_min, tm_sec, s);
	else
		len = fprintf(fp, "%s\n", s);
	if (len < 0) {
		fprintf(stderr, "fprintf error\n");
		exit(-1);
	}
}

void usage(void)
{
	printf("gzip stdin log to file_prefix.*.gz\n");
	printf("Usage:\n");
	printf("gzlog -p file_prefix [ -t | -f | -d | -h ]\n");
	printf("  options:\n");
	printf("    -p file_prefix      log file prefix\n");
	printf("    -t                  add hour min sec timestamp\n");
	printf("    -f                  add year mon day hour min sec timestamp\n");
	printf("    -d                  change file every day (default) \n");
	printf("    -h                  change file every hour\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	int c;
	while ((c = getopt(argc, argv, "p:tfdh")) != EOF)
		switch (c) {
		case 'p':
			strncpy(file_prefix, optarg, MAXLEN);
			file_prefix[MAXLEN - 1] = '\0';
			break;
		case 't':
			timestamp_t = 1;
			break;
		case 'f':
			timestamp_f = 1;
			break;
		case 'd':
			change_every_day = 1;
			change_every_hour = 0;
			break;
		case 'h':
			change_every_hour = 1;
			change_every_day = 0;
			break;
		}
	if (file_prefix[0] == 0)
		usage();
	if (signal(SIGHUP, signal_handler) == SIG_ERR) {
		perror("signal(SIGHUB, signal_hander) error");
		fclose(fp);
		return -1;
	}
	char buf[MAXLEN];
	int len;
	while (fgets(buf, MAXLEN, stdin)) {
		buf[MAXLEN - 1] = 0;
		len = strlen(buf);
		if (len < 0)
			break;
		if (buf[len - 1] == '\n') {
			len--;
			buf[len] = 0;
		}
		Log(buf);
	}
	return 0;
}
