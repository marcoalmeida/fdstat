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


#ifndef UTIL_H_
#define UTIL_H_

#define MAX_PATH 320
#define MAX_CMDLINE 8072 /* arbitrary but large enough (?) */
#define MAX_BUF_SIZE 32


/* print out the string that describes errno and exit */
void suicide(const char *action);
/* return 1 if str points to a string of digits, 0 otherwise */
int is_integer(const char *str);
/* return the maximum number of file handles for the entire system */
int max_fd();
/* return the number of total allocated file handles or -1 on error */
int allocated_fd();
/* return the maximum number of processes */
int max_pid();
/* read the cmdline of pid into buf (of size size) */
int proc_cmdline(int pid, char *buf, int size);


#endif /* UTIL_H_ */
