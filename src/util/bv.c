// Sebastian Carbonero
// © 2021

#include "bv.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Gets byte length with length of bits given
//
// Used lecture 5/3
//
// x: length of bits
//
#define byte_length(x) ((x / 8) + !!(x % 8))

// Bit Vector struct
//
// Used from the assignment doc.
//
// vector: array of bytes
// length: length in bits
//
struct BitVector {
    uint32_t length; // Length in bytes.
    uint8_t *vector; // Array of bytes.
};

// Cretes bit vector
//
// length: length of bit vector
//
BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    assert(v);
    v->length = length;
    v->vector = (uint8_t *) calloc(byte_length(length), sizeof(uint8_t));
    assert(v->vector);
    return v;
}

// Deltes Bit vector
//
// **v: pointer for bit vector
//
void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
    return;
}

// Returns bit vector length
//
// *v: reference for vector struct
//
uint32_t bv_length(BitVector *v) {
    return v->length;
}

// Sets bit based on index
//
// Inspiration from Darrel Long twiddle.c
//
// *v: reference for bit vector
//  i: index of bit to set
//
void bv_set_bit(BitVector *v, uint32_t i) {
    if (v && v->vector) {
        v->vector[i / 8] |= (0x1 << (i % 8));
    }
    return;
}

// Clears bit based on index
//
// Inspiration from Darrel Long twiddle.c
//
// *v: reference for bit vector
//  i: index of bit to clear
//
void bv_clr_bit(BitVector *v, uint32_t i) {
    if (v && v->vector) {
        v->vector[i / 8] &= ~(0x1 << (i % 8));
    }
    return;
}

// XORs bit with desired bit at index i
//
// *v: reference for bit vector
// i: index of bit to XOR
// bit: desired bit value to xor with
//
void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    if (v && v->vector) {
        uint8_t b = bv_get_bit(v, bit); // get bit
        v->vector[i / 8] ^= (b << (i % 8));
    }
    return;
}

// Gets bit at index i
//
// *v: reference for bit vector
// i: index of bit to retrieve
//
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    return (v->vector[i / 8] >> (i % 8) & 0x1);
}

// Prints out bit vector
//
// Inspiration from Darrel Long twiddle.c
//
// *v: reference for bit vector
//
void bv_print(BitVector *v) {
    for (uint32_t i = 0; i < v->length; i++) {
        printf("%c", '0' + (unsigned char) bv_get_bit(v, i));
    }
    printf("\n");
    return;
}
