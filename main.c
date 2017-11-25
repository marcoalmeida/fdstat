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
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <argp.h>

#include "arg_parser.h"
#include "util.h"


#define _POSIX_C_SOURCE >= 200809L


int count_fds(const char *pid)
{
    char path[MAX_PATH];
    int count = 0;
    DIR *dp;
    struct dirent *entry;

    if (sprintf(path, "/proc/%s/fd", pid) < 0)
        suicide("creating /proc/<pid>/fd path");

    dp = opendir(path);
    /* some processes are very short-lived; just ignore it in case it
     * happens to disappear */
    if (dp) {
        while((entry=readdir(dp))) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
                count++;
        }
    }
    closedir(dp);

    return count;
}

void count_proc_fds(int *procs)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    dp = opendir("/proc");
    if (!dp)
        suicide("reading /proc");

    while((entry=readdir(dp))) {
        stat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            if (is_integer(entry->d_name)) {
                procs[atoi(entry->d_name)] = count_fds(entry->d_name);
            }
        }
    }

    closedir(dp);
}

void print(int *proc_fds, int n_procs, int json, int cmdline)
{
    int pid;
    int i_printed = 0;
    char cmdline_buf[MAX_CMDLINE];
    char *begin = json ? "{" : "PID\tFDs\n";
    char *end = json ? "}\n" : "\n";
    char *pid_fmt = json ? "\"%d\": %d" : "%d\t%d\n";
    char *cmdline_fmt = json ? "\"%s\": %d" : "%s\t%d\n";

    printf(begin, "");
    for (pid = 1; pid < n_procs; pid++) {
        /* ignore processes with no open files */
        if (proc_fds[pid]) {
            /* avoid a trailing comma -- we don't know when the last
                 * line will be printed, so keep track of the first one
                 * and prepend commas instead of appending */
            if (json && i_printed)
                printf(",");
            if (cmdline) {
                if (proc_cmdline(pid, cmdline_buf, MAX_CMDLINE))
                    printf(cmdline_fmt, cmdline_buf, proc_fds[pid]);
                else
                    fprintf(stderr, "Failed to get cmdline for pid %d\n", pid);
            }
            else
                printf(pid_fmt, pid, proc_fds[pid]);
            i_printed = 1;
        }
    }
    printf(end, "");
}

void print_summary(int allocated, int total, int json) {
    if (json)
        printf("{\"allocated\": %d, \"total\": %d}\n", allocated, total);
    else {
        printf("Allocated\tMaximum available\n");
        printf("%d\t\t%d\n", allocated, total);
    }
}

int main(int argc, char *argv[])
{
    /* array to store the number of open file descriptors per process
     * and total number of running procs */
    int *proc_fds, n_procs=max_pid()+1;
    /* struct for storing argument's values; also initialize/set
     * defaults (conveniently all zero) */
    struct arguments arguments = { 0 };

    /* parse arguments */
    parse_args(argc, argv, &arguments);

    /* allocate the array to store the number of open file descriptors
     * per process */
    if (! (proc_fds=(int *)calloc(n_procs, sizeof(int))))
        suicide("allocating process table");

    while (1) {
        if (arguments.summary) {
            print_summary(allocated_fd(), max_fd(), arguments.json);
            break;
        }
        else {
            /* reset the process array */
            memset(proc_fds, 0, n_procs*sizeof(int));
            /* collect/count the number of file descriptors per process */
            count_proc_fds(proc_fds);
            /* print out the results */
            print(proc_fds, n_procs, arguments.json, arguments.cmdline);
        }

        /* if a COUNT was set, make sure we stop after those many */
        if (arguments.n_args == 2) {
            if (arguments.args[1]-- == 1)
                break;
        }
        /* if an INTERVAL was set, sleep for that period of time */
        if (arguments.n_args > 0)
            sleep(arguments.args[0]);
    }

    /* release memory allocated for */
    free(proc_fds);

    return 0;
}
