// Sebastian Carbonero
// © 2021

#include "hamming.h"

#include "../util/bm.h"
#include "../util/bv.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Look up table for error syndrome of Ht
static int decode_table[] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2,
    HAM_ERR, 1, 0, HAM_ERR };

// Returns fixed message
//
// msg: byte of message
// pos: bit to flip to fix message
//
static inline uint8_t fix_message(uint8_t msg, uint8_t pos) {
    return msg ^ (1 << (pos % 8));
}

// Returns reversed byte
//
// This function was inspired by the following stackoverflow post
// https://bit.ly/3tv49Oi
//
// b: byte to reverse positions of
//
unsigned char reverse(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4; // Reverses outer bits
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2; // Reverses second to out bits
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1; // reverses inner bits
    return b;
}

// Helper function for decode process
//
// Ht: Bitmatrix Ht
// code: encoded message
//
static inline uint8_t dec_helper(BitMatrix *Ht, uint8_t code) {
    BitMatrix *b_m = bm_from_data(code, 8);
    BitMatrix *result = bm_multiply(Ht, b_m); 
    uint8_t r_decoded = bm_to_data(result); 

    // Free memory
    bm_delete(&b_m);
    bm_delete(&result);
    return r_decoded;
}

// Helper function for decode process
//
// G: Bitmatrix G
// code: message to decode
//
uint8_t encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *n_m = bm_from_data(msg, 4); 
    BitMatrix *result = bm_multiply(G, n_m); 
    uint8_t r_encoded = bm_to_data(result);

    // Free memory
    bm_delete(&n_m);
    bm_delete(&result);
    return r_encoded;
}

HAM_STATUS decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    // Get error syndrome from decoded message
    uint8_t message, decoded = dec_helper(Ht, reverse(code));

    switch (decode_table[decoded]) { // Check look up table
    case (HAM_OK): *msg = (code & 0xF); return HAM_OK;
    case (HAM_ERR): return HAM_ERR;
    default:
        message = fix_message(code, 7 - decode_table[decoded]);
        *msg = (message & 0xF); // Sets message
        return HAM_CORRECT; 
    }
    return HAM_ERR;
}
