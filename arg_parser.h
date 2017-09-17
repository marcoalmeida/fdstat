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


#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_


/* Used by main to communicate with parse_opt. */
struct arguments {
    /* number of arguments we received: interval and/or count */
    int n_args;
    int args[2]; /* interval and count */
    int summary, verbose, json, cmdline;
};


void parse_args(int argc, char *argv[], struct arguments *arguments);


#endif /* ARG_PARSER_H_ */
