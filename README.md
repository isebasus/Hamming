> :warning: **This Project is a modified school project** and thus no students of CSE 13s should be looking at my source code.

# 🔑 Hamming Codes [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/isebasus/Hamming/blob/master/LICENSE)
## A Hamming encode and decode implementation.

This project is able to build two programs (the encoder and decoder). Both programs use Hamming codes to encode or decode messages. The user will need to pipe in desired files to encode or decode.

## What you need

In order to compile this program you'll need the Makefile provided, encode.c and decode.c. You'll also need all the .h files as dependencies. 

### Running

To build the program, you will need to run these commands.

Once inside the hamming directory, run:

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

To remove the binary files run:

```
$ make clean
```

### 📚 Sources

* [The C Programming Language - 2nd Edition](https://drive.google.com/file/d/0BxImv9GQ06DcNjc1MTk0N2ItZmJmNC00YTM0LWFmN2UtMGQ4ZmE3Y2E1YzU4/view) - BitMatrix Inspiration
* Sample Files provided by my professor
* 🌐 The internet 
