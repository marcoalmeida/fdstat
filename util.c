/* Copyright (C) 2017 Marco Almeida <marcoafalmeida@gmail.com> */

/* This file is part of fdstat. */

/* fdstat is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License, or */
/* (at your option) any later version. */

/* fdstat is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with fdstat; if not, write to the Free Software */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "util.h"


void suicide(const char *action)
{
    fprintf(stderr, "Unrecoverable error %s: %s\n", action, strerror(errno));
    exit(EXIT_FAILURE);
}

int is_integer(const char *str)
{
    while (*str)
        if (! isdigit(*str++))
            return 0;

    return 1;
}

int sys_value(const char *path)
{
    char buf[MAX_BUF_SIZE];
    int value = -1;
    FILE *fp;

    if (! (fp=fopen(path, "r")))
	suicide(path);
    if ((fread(buf, 1, MAX_BUF_SIZE-1, fp) > 0) && (! ferror(fp)))
	value = atoi(buf);
    fclose(fp);

    return value;
}

__inline__ int max_fd()
{
    return sys_value("/proc/sys/fs/file-max");
}

__inline__ int max_pid()
{
    return sys_value("/proc/sys/kernel/pid_max");
}

int allocated_fd()
{
    char buf[MAX_BUF_SIZE];
    char *delim;
    FILE *fp;

    if (! (fp=fopen("/proc/sys/fs/file-nr", "r")))
	suicide("opening /proc/sys/fs/file-nr");
    if (fread(buf, 1, MAX_BUF_SIZE, fp) && (!ferror(fp))) {
	/* this file contains 3 integers: allocated, unused, maxiumum;
	 * we want the first */
	if ((delim=index(buf, '\t'))) {
	    *delim = 0;
	    return atoi(buf);
	}
    }
    fclose(fp);

    return -1;
}

int proc_cmdline(int pid, char *buf, int size)
{
    int success = 1;
    char path[MAX_PATH];
    FILE *fp;

    if (sprintf(path, "/proc/%d/cmdline", pid) < 0)
	suicide("creating /proc/<pid>/cmdline path");

    /* some processes are very short-lived; just ignore it in case it
     * happens to disappear */
    if ((fp=fopen(path, "r"))) {
	if (!fread(buf, 1, size, fp) || ferror(fp))
	    success = 0;
	fclose(fp);
    }

    return success;
}
