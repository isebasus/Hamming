// Sebastian Carbonero
// © 2021

#include "bm.h"
#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hi:o:"

struct stat statbuf;

// Generator Matrix G
static uint8_t encode_matrix[4][8] = { { 1, 0, 0, 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 1, 0, 1, 1 },
    { 0, 0, 1, 0, 1, 1, 0, 1 }, { 0, 0, 0, 1, 1, 1, 1, 0 } };

static uint8_t encode_mem[16] = { 0 };

// Returns the lower nibble of a byte
//
// Used from the assignment doc
//
// val: byte to get lower nibble of
//
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

// Returns the upper nibble of a byte
//
// Used from the assignment doc
//
// val: byte to get upper nibble of
//
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

// Creates generator matrix Ht
//
//*m: reference for bit matrix
// r: number of rows to fill
// c: number of columns to fill
//
static inline void generate_matrix(BitMatrix *m, uint32_t r, uint32_t c) {
    for (uint32_t i = 0; i < c; i++) {
        for (uint32_t j = 0; j < r; j++) {
            // Checks look up table decode_matrix
            if (encode_matrix[i][j] == 1) {
                bm_set_bit(m, j, i); // set bit
            }
        }
    }
}

// Reads and decodes file
//
// infile: file to decode
// outfile: file to write to
//
uint8_t read_file(FILE *infile, FILE *outfile) {
    BitMatrix *bm = bm_create(8, 4); // Create bit matrix
    generate_matrix(bm, 8, 4); // Populate generator matrix G

    // Inspired by the assignment PDF to set file permissions
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    int c;
    while (c = fgetc(infile), c != EOF) {
        uint8_t lower_nib = lower_nibble(c), upper_nib = upper_nibble(c);
        uint8_t lower = encode_mem[lower_nib];
        uint8_t upper = encode_mem[upper_nib];
        if (lower == 0) {
            lower = encode(bm, lower_nib);
        }
        if (upper == 0) {
            upper = encode(bm, upper_nib);
        }
        encode_mem[lower_nib] = lower;
        encode_mem[upper_nib] = upper;
        fputc(lower, outfile); // fputc lower nibble
        fputc(upper, outfile); // fputc upper nibble
    }
    // free memory
    bm_delete(&bm);
    fclose(infile);
    fclose(outfile);

    return 0;
}

// Prints out help guide
//
// This function is used when there are
// no user inputs
//
// Also used when there is an unknown argument
//
static inline void no_argument() { // Used to print out options
    fprintf(stderr, "SYNOPSIS\n     A Hamming(8, 4) systematic code generator."
                    "\n\nUSAGE\n     ./encode [-h] [-i infile] [-o outfile]"
                    "\n\nOPTIONS\n  -h             Program usage and help.\n"
                    "  -i infile      Input data to encode.\n"
                    "  -o outfile     Output of encoded data.\n");
    return;
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin, *outfile = stdout; // Files to read and write to
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': no_argument(); return 0; // print out help options
        case 'i':
            // Open file to read from
            if ((infile = fopen(optarg, "rb")) == NULL) { // if open fail print error
                fprintf(stderr, "Error: failed to open infile.\n");
                return 1;
            }
            break;
        case 'o':
            // Open file to write to
            if ((outfile = fopen(optarg, "wb")) == NULL) { // if write fail print error
                fprintf(stderr, "Error: Unable to write file\n");
                return 1;
            }
            break;
        case '?': no_argument(); return 1; // Unknown argument
        default: abort(); return 1;
        }
    }
    return read_file(infile, outfile);
}
