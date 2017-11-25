# fdstat
**fdstat** reports the number of files descriptors allocated by each process.

The information is collected from the `/proc` filesystem, therefore an
open file may be a regular file, a directory, a block special file, a
character special file, an executing text reference, a library, a
stream or a network file (Internet socket, NFS file or UNIX domain
socket).

Each report provides information gathered at the time of execution.


# Usage

Usage: `fdstat [OPTION...] [INTERVAL [COUNT]]`

Options:
```
  -c, --cmdline              Output the command line instead of the PID

  -j, --json                 Produce JSON output

  -s, --summary              Display only totals (allocated and maximum)

  -v, --verbose              Produce verbose output

  -?, --help                 Give this help list

      --usage                Give a short usage message

  -V, --version              Print program version
```

The `INTERVAL` parameter specifies the amount of time in seconds between each  report.
The  `COUNT` parameter can be specified in conjunction with the `INTERVAL` parameter. If
the `COUNT` parameter is specified, the  value  of `COUNT`  determines  the  number  of
reports  generated at `INTERVAL` seconds apart. If the `INTERVAL` parameter is specified
without the `COUNT` parameter, the **fdstat** command generates reports continuously.

Report bugs to <marcoafalmeida@gmail.com>.
