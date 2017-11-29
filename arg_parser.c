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


#include <stdlib.h>
#include <argp.h>

#include "arg_parser.h"


/* put the given documentation string and bug address in the ‘--help’
 * output, as per GNU standards */
const char *argp_program_version = "fdstat 0.2";
const char *argp_program_bug_address = "<marcoafalmeida@gmail.com>";


/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* get the input argument from argp_parse, which we know is a
       pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'v':
        arguments->verbose = 1;
        break;
    case 'j':
        arguments->json = 1;
        break;
    case 'c':
        arguments->cmdline = 1;
        break;
    case 's':
        arguments->summary = 1;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            /* too many arguments */
            argp_usage (state);
        arguments->args[state->arg_num] = atoi(arg);
        arguments->n_args++;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}


void parse_args(int argc, char *argv[], struct arguments *arguments)
{
    /* program documentation */
    static char doc[] = "Report file descriptor statistics";
    /* positional arguments */
    static char args_doc[] = "INTERVAL COUNT";
    /* all options we accept */
    static struct argp_option options[] = {
        { "verbose", 'v', 0, 0, "Produce verbose output" },
        { "json", 'j', 0, 0, "Produce JSON output" },
        { "cmdline", 'c', 0, 0, "Output the command line instead of the PID" },
        { "summary", 's', 0, 0, "Display only totals (allocated and maximum)" },
        { 0 }
    };
    /* argp parser. */
    static struct argp argp = { options, parse_opt, args_doc, doc };

    /* parse arguments; every option seen by parse_opt will be
       reflected in the arguments struct */
    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
