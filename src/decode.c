// Sebastian Carbonero
// © 2021

#include "util/bm.h"
#include "hamming/hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hvi:o:"

struct stat statbuf;

// Generator Matrix Ht
static uint8_t decode_matrix[8][4] = {
    { 0, 1, 1, 1 },
    { 1, 0, 1, 1 },
    { 1, 1, 0, 1 },
    { 1, 1, 1, 0 },
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 },
};

// Returns the lower nibble of a byte
//
// Used from the assignment doc
//
// val: byte to get lower nibble of
//
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

// Packs byte after decoding
//
// Used from the assignment doc
//
// upper: upper nibble of byte
// lower: lower nibble of byte
//
static inline uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
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
            if (decode_matrix[i][j] == 1) {
                bm_set_bit(m, j, i); // set bit
            }
        }
    }
}

// Returns 1 if status is satisfied
//
// status: status of decode process
// type: HAM_STATUS type to compare with
// c: file status
//
static inline uint32_t check_status(HAM_STATUS status, HAM_STATUS type, int c) {
    if (status == type && c != EOF) {
        return 1;
    }
    return 0;
}

// Reads and decodes file
//
// infile: file to decode
// outfile: file to write to
// statistics: boolean for option -v
//
static inline uint8_t read_file(FILE *infile, FILE *outfile, bool statistics) {
    BitMatrix *Ht = bm_create(4, 8); // Create bit matrix
    generate_matrix(Ht, 4, 8); // Generate Ht matrix

    // Inspired by assignment PDF to set file permissions
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    int c;
    // Initialize variables
    uint32_t uncorrected = 0, corrected = 0, bytes = 0;
    uint8_t lower_nibble = 0, upper_nibble = 0;
    while (c = fgetc(infile), c != EOF) {
        HAM_STATUS first = 0, second = 0;

        // Decode first byte for lower nibble
        first = decode(Ht, (uint8_t) c, &lower_nibble);
        if (c = fgetc(infile), c != EOF) {
            // decode second byte for upper nibble
            second = decode(Ht, (uint8_t) c, &upper_nibble);

            // Check status for decode process for verbose options
            corrected += check_status(second, HAM_CORRECT, c);
            uncorrected += check_status(second, HAM_ERR, c);
        }

        // Check status for decode process for verbose options
        corrected += check_status(first, HAM_CORRECT, c);
        uncorrected += check_status(first, HAM_ERR, c);
        bytes += 2; // increment byte count
        // Pack decoded byte for fputc
        uint8_t byte = pack_byte(upper_nibble, lower_nibble);
        if (c == EOF) {
            byte = 0;
        }
        fputc(byte, outfile);
    }

    if (statistics) { // Print statistics for verbose option
        fprintf(stderr, "Total bytes processed: %d\n", bytes);
        fprintf(stderr, "Uncorrected errors: %d\n", uncorrected);
        fprintf(stderr, "Corrected errors: %d\n", corrected);
        fprintf(stderr, "Error rate: %0.6lf\n", (double) uncorrected / (double) bytes);
    }
    // Free memory
    bm_delete(&Ht);
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
    fprintf(stderr, "SYNOPSIS\n     A Hamming(8, 4) systematic code decoder."
                    "\n\nUSAGE\n     ./encode [-h] [-v] [-i infile] [-o outfile]"
                    "\n\nOPTIONS\n  -h             Program usage and help.\n"
                    "  -v infile      Print statistics of decoding to stderr.\n"
                    "  -i infile      Input data to decode.\n"
                    "  -o outfile     Output of decoded data.\n");
    return;
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin, *outfile = stdout;
    bool statistics = false; // For verbose option
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': no_argument(); return 0;
        case 'v': statistics = true; break;
        case 'i':
            if ((infile = fopen(optarg, "rb")) == NULL) { // if open fail print error
                fprintf(stderr, "Error: failed to open infile.\n");
                return 1;
            }
            break;
        case 'o':
            if ((outfile = fopen(optarg, "wb")) == NULL) { // if write fail print error
                fprintf(stderr, "Error: Unable to write file\n");
                return 1;
            }
            break;
        case '?': no_argument(); return 1; // Unknown argument
        default: abort(); return 1;
        }
    }
    return read_file(infile, outfile, statistics);
}
