// Sebastian Carbonero
// © 2021

#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Bit Matrix Struct
//
// Used from the assignment doc.
//
// rows: rows of matrix
// columns: columns of matrix
// vector: Bit Vector struct
//
struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

// Cretes bit matrix
//
// rows: length of bit matrix
// cols: columns of bit matrix
//
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    assert(m); // Checks for malloc failure
    m->rows = rows;
    m->cols = cols;
    m->vector = bv_create(rows * cols); // Creates bit vector as matrix
    assert(m->vector); // Checks for calloc failure
    return m;
}

// Helper function to check range of r, c
//
// *m: reference for bit matrix
// r: bit matrix rows
// c: bit matrix columns
//
static inline bool in_range(BitMatrix *m, uint32_t r, uint32_t c) {
    return (((int32_t) r >= 0 && r < m->rows) && ((int32_t) c >= 0 && c < m->cols));
}

// Deltes Bit Matrix
//
// **m: pointer for bit matrix
//
void bm_delete(BitMatrix **m) {
    if (*m && (*m)->vector) {
        bv_delete(&(*m)->vector);
        free((*m)->vector);
        free(*m);
        *m = NULL;
    }
    return;
}

// Returns size of Bit Matrix rows
//
// *m: reference for bit matrix
//
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

// Returns size of Bit Matrix columns
//
// *m: reference for bit matrix
//
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

// Sets bit based on row and column posistion
//
// (r * m.cols + c) is the index
//
// *m: reference for bit matrix
// r: row index
// c: column index
//
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    if (in_range(m, r, c)) {
        bv_set_bit(m->vector, (r * m->cols + c));
    }
}

// Clears bit based on row and column index
//
// (r * m.cols + c) is the index
//
// *m: reference for bit matrix
// r: row index
// c: column index
//
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    if (in_range(m, r, c)) {
        bv_clr_bit(m->vector, (r * m->cols + c));
    }
    return;
}

// Gets bit based on row and column index
//
// // (r * m.cols + c) is the index
//
// *m: reference for bit matrix
// r: row index
// c: column index
//
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    assert(in_range(m, r, c));
    return bv_get_bit(m->vector, (r * m->cols + c));
}

// Turns byte into bit matrix
//
// byte: byte to create matrix with
// length: byte length
//
BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    if (length <= 8) {
        BitMatrix *bm = bm_create(length, 1);
        for (uint32_t i = 0; i < length; i++) {
            uint8_t b = (byte >> i) & 1; // Gets ith bit
            if (b == 1) { // if bit is 1, set bit
                bm_set_bit(bm, i, 0);
            }
        }
        return bm;
    }
    return NULL;
}

// Turns bit matrix into byte
//
// *m: reference for bit matrix
//
uint8_t bm_to_data(BitMatrix *m) {
    uint8_t byte = 0;
    // Gets first column of bit matrix
    for (uint32_t i = 0; i < 8; i++) {
        // Sets each bit to byte
        byte |= bv_get_bit(m->vector, (i)) << i;
    }
    return byte;
}

// Matrix multiplies two bit matrices
//
// *A: reference for bit matrix A
// *B: reference for bit matrix B
//
BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    if (A->cols == B->rows) {
        // Create bit matrix of A rows and B columns
        BitMatrix *r = bm_create(A->rows, B->cols);
        for (uint32_t i = 0; i < A->rows; i++) {
            for (uint32_t j = 0; j < B->cols; j++) {
                uint32_t s = 0;
                // Follows rule of C(i,k) = A(j,k) * B(k, j)
                for (uint32_t k = 0; k < A->cols; k++) {
                    // Add to sum of matrix multiplication
                    s += (bm_get_bit(A, i, k) * bm_get_bit(B, k, j));
                }
                if ((s % 2) == 1) {
                    bm_set_bit(r, i, j); 
                }
            }
        }
        return r;
    }
    return NULL;
}

// Prints out Bit Matrix
//
// *m: reference for bit matrix
//
void bm_print(BitMatrix *m) {
    for (uint32_t i = 0; i < m->cols; i++) {
        for (uint32_t j = 0; j < m->rows; j++) {
            printf("%d", bm_get_bit(m, j, i));
        }
        printf("\n");
    }
    return;
}
