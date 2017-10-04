
//#pragma once
#ifndef ARGON2ENCODING_H
#define	ARGON2ENCODING_H


//#include "argon2.h"

#include "Blake2b.h"
//#include <string.h>
//#include <windows.h>



typedef struct Argon2_Compress{
    const uint8_t* X;
    const uint32_t* Y;
    
    const uint8_t* Output;
    const uint8_t* S;
    
}Argon2_Compres;


void Argon2_Comp_G(uint64_t* X_block, uint64_t* Y_block, uint64_t* Output);




void Argon2_Hash(uint8_t*AD_X,uint32_t size_AD_X, uint32_t T_tag,uint8_t* hash_value);


void Argon2_XOR(uint64_t *X_block, uint64_t* Y_block, uint64_t* out);




#if !defined trunc
// Truncation of the 32 lsb of a uint64_t, without changing its type
#define trunc(m) (m & 0x00000000FFFFFFFF)
#endif




#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}

#endif

#endif	/* ARGON2ENCODING_H */
