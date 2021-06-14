# Hamming Codes

This project is able to build two programs which either encode or decode a file. These programs use Hamming codes to encode or decode messages. The user will need to pipe in desired files to encode or decode. *To see what each file does, visit Files.*

## What you need

In order to compile this program you'll need the Makefile provided, encode.c and decode.c. You'll also need all the .h files as dependencies. 

### Running

To build the program, you will need to run these commands.

Once inside the asgn5 directory, run:

```
$ make all
```
This will build all programs including encode.c, decode.c, error.c, and entropy.c

To run encode.c:

```
$ ./encode [-h] [-i infile] [-o outfile]
```

The arguments needed to run this program are:

```
  -h             Program usage and help.
  -i infile      Input data to encode.
  -o outfile     Output of encoded data.
```

To run decode.c:

```
$ ./decode [-h] [-v] [-i infile] [-o outfile]
```

The arguments needed to run this program are:

```
  -h             Program usage and help.
  -v             Print out statistics
  -i infile      Input data to encode.
  -o outfile     Output of encoded data.
```

To run error.c:

```
SYNOPSIS
  A error-injection (noise) program.

USAGE
  ./error [-h] [-s seed] [-e error_rate]

OPTIONS
  -h               Program usage and help.
  -s seed          Specifies the random seed.
  -e error_rate    Specifies the error rate.
```

To run entropy.c:

```
SYNOPSIS
  A entropy measuring program.

USAGE
  ./entropy < [input (reads from stdin)]

OPTIONS
  -h               Program usage and help.
```

To remove the binary files run:

```
$ make clean
```

## Files

*See what each file does*

### DESIGN.pdf

This file contains my design process for this project.

### Makefile

This file contains a Makefile script to compile encode.c, decode.c, entropy.c, error.c and link all the dependencies.

### encode.c

This file is used to encode messages.

### decode.c 

This file is used to decode messages.

### error.c

This file is used to create error noise for encoding and decoding.

### entropy.c 

This file is used to measure a program.

### *.h 

Each of these files contains the functions used as dependencies for decode.c and encode.c

## Sources

* Asgn5 PDF to implement functions in structs
* Piazza for code inspiration
* Stackoverflow for inspiration 

## Authors

* **Sebastian Carbonero**
# Hamming
