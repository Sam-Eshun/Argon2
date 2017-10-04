/* 
 * File:   Argon2Encoding_testcase.c
 * Author: Sammy
 *
 * Created on March 23, 2017, 2:05 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include  "Argon2Encoding.h"
#include "Blake2b.h"
#include "argon2.h"
/*
 * 
 */


const uint8_t H_Ini_enter[72] = {0xB8, 0x81, 0x97, 0x91, 0xA0, 0x35, 0x96, 0x60,
                                 0xBB, 0x77, 0x09, 0xC8, 0x5F, 0xA4, 0x8F, 0x04, 
                                 0xD5, 0xD8, 0x2C, 0x05, 0xC5, 0xF2, 0x15, 0xCC, 
                                 0xDB, 0x88, 0x54, 0x91, 0x71, 0x7C, 0xF7, 0x57, 
                                 0x08, 0x2C, 0x28, 0xB9, 0x51, 0xBE, 0x38, 0x14, 
                                 0x10, 0xB5, 0xFC, 0x2E, 0xB7, 0x27, 0x40, 0x33, 
                                 0xB9, 0xFD, 0xC7, 0xAE, 0x67, 0x2B, 0xCA, 0xAC, 
                                 0x5D, 0x17, 0x90, 0x97, 0xA4, 0xAF, 0x31, 0x09, 
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


int main(void){

        uint64_t X_block[Argon2_Block_length];
        uint64_t Y_block[Argon2_Block_length];
        uint64_t Output[Argon2_Block_length];

        memset(X_block,0x12,Argon2_Block_length*sizeof(uint64_t));
        memset(Y_block,0x13,Argon2_Block_length*sizeof(uint64_t));
        
        Argon2_Comp_G(X_block, Y_block, Output);
        printf("Argon2Comp:: Compression of 0x12 and 0x13:\n");
         for(int k = 0; k<Argon2_Block_length;k++){

            printf("%016llX ",Output[k]);

        }
         printf("\n");

        uint64_t hash[4];
        uint8_t firsthash[1024];

        Argon2_Hash((uint8_t*)X_block, ARGON2_MEM_BLOCK_SIZE, 4*Argon2_H_O_LENGTH, (uint8_t*)hash);

        printf("Argon2Comp:: Hashing H' of 0x12:\n");
        for(int k = 0;k<4;k++){

            printf("%016llX ",hash[k]);

        }
         printf("\n");

        for(int k = 1;k<ARGON2_MEM_BLOCK_SIZE+1;k++)
            
        Argon2_Hash((uint8_t*)H_Ini_enter, Argon2_H_O_SEED_LENGTH, k, firsthash);

        return 0;
}
