#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include "bsd.h"

const char *const sys_signame[NSIG] = {
	[0] = "Signal 0",
	[SIGHUP] = "HUP", [SIGINT] = "INT", [SIGQUIT] = "QUIT",
	[SIGILL] = "ILL", [SIGTRAP] = "TRAP", [SIGABRT] = "ABRT",
#ifdef SIGEMT
	[SIGEMT] = "EMT",
#endif
	[SIGFPE] = "FPE", [SIGKILL] = "KILL", [SIGBUS] = "BUS",
	[SIGSEGV] = "SEGV", [SIGSYS] = "SYS", [SIGPIPE] = "PIPE",
	[SIGALRM] = "ALRM", [SIGTERM] = "TERM", [SIGURG] = "URG",
	[SIGSTOP] = "STOP", [SIGTSTP] = "TSTP", [SIGCONT] = "CONT",
	[SIGCHLD] = "CHLD", [SIGTTIN] = "TTIN", [SIGTTOU] = "TTOU",
	[SIGIO] = "IO", [SIGXCPU] = "XCPU", [SIGXFSZ] = "XFSZ",
	[SIGVTALRM] = "VTALRM", [SIGPROF] = "PROF", [SIGWINCH] = "WINCH",
#ifdef SIGINFO
	[SIGINFO] = "INFO",
#endif
	[SIGUSR1] = "USR1", [SIGUSR2] = "USR2",
};
const int sys_nsig = NSIG;

int
sig2str(int signum, char *str)
{
	const char *name;
	if (signum < 0 || signum > SIGRTMAX)
		return -1;
	if (signum < NSIG && sys_signame[signum] != NULL)
		name = sys_signame[signum];
	else if (signum == SIGRTMIN)
		name = "RTMIN";
	else if (signum == SIGRTMAX)
		name = "RTMAX";
	else
		return snprintf(str, SIG2STR_MAX, "%d", signum) < 0 ? -1 : 0;
	snprintf(str, SIG2STR_MAX, "%s", name);
	return 0;
}

int
str2sig(const char *str, int *pnum)
{
	char *end;
	long n;
	int sig;

	if (strncasecmp(str, "SIG", 3) == 0)
		str += 3;
	if (isdigit((unsigned char)*str)) {
		errno = 0;
		n = strtol(str, &end, 10);
		if (errno != 0 || *end != '\0' || n < 1 || n > SIGRTMAX)
			return -1;
		*pnum = (int)n;
		return 0;
	}
	if (strncasecmp(str, "RTMIN", 5) == 0) {
		n = 0;
		if (str[5] == '+' || str[5] == '-') {
			errno = 0;
			n = strtol(str + 5, &end, 10);
			if (errno != 0 || *end != '\0')
				return -1;
		}
		sig = SIGRTMIN + (int)n;
		if (sig < SIGRTMIN || sig > SIGRTMAX)
			return -1;
		*pnum = sig;
		return 0;
	}
	if (strncasecmp(str, "RTMAX", 5) == 0) {
		n = 0;
		if (str[5] == '+' || str[5] == '-') {
			errno = 0;
			n = strtol(str + 5, &end, 10);
			if (errno != 0 || *end != '\0')
				return -1;
		}
		sig = SIGRTMAX + (int)n;
		if (sig < SIGRTMIN || sig > SIGRTMAX)
			return -1;
		*pnum = sig;
		return 0;
	}
	for (sig = 1; sig < NSIG; sig++) {
		if (sys_signame[sig] != NULL &&
		    strcasecmp(sys_signame[sig], str) == 0) {
			*pnum = sig;
			return 0;
		}
	}
	return -1;
}

struct fw {
	void *cookie;
	int (*writefn)(void *, const char *, int);
};

static ssize_t
fw_write(void *c, const char *buf, size_t len)
{
	struct fw *fw = c;
	return fw->writefn(fw->cookie, buf, (int)len);
}

static int
fw_close(void *c)
{
	free(c);
	return 0;
}

FILE *
fwopen(void *cookie, int (*writefn)(void *, const char *, int))
{
	cookie_io_functions_t io;
	struct fw *fw = malloc(sizeof(*fw));
	if (fw == NULL)
		return NULL;
	fw->cookie = cookie;
	fw->writefn = writefn;
	io.read = NULL;
	io.write = fw_write;
	io.seek = NULL;
	io.close = fw_close;
	return fopencookie(fw, "w", io);
}
