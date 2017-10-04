#pragma once

#if !defined ARGON2_HEADER
#define ARGON2_HEADER

#include "Argon2_MAT.h"
#define Argon2Mem_Test_Leaks 0

#define Argon2_H_O_LENGTH 64



//Number of synchronization points between lanes per pass 
#define ARGON_SYNC_POINTS 4
//const uint32_t ARGON2_SYNC_POINTS = ARGON_SYNC_POINTS;






#ifndef CAT_N_Bytes

#define CAT_N_Bytes(arr,pointer,n) {memmove(arr,pointer,n); arr+=n;}

#endif

        
typedef struct _Argon2 {
    
      // Message string P  and its size = [0 .. 2^32-1]
        uint8_t* P_string;
         uint32_t size_P_string;
          // Nonce S 
        uint8_t* S_Nonce;
        uint32_t Nonce_size_S;
        // size of Nonce from  [8 .. 2^32-1]
         //uint32_t size_S_Nonce;
        // Degree of Parallelism from [1 .. 2^24-1]
        uint32_t p_lism;
         // Tag length T
         uint32_t T_tag;
        // Total cost of memory blocks
         uint32_t m_cost;
         // Number of Iterations
         uint32_t iterate;
       // Version byte, default = 0x13
        uint32_t version;
     // Key  
         // Number of steps
      //  uint32_t t_steps;
        uint8_t *Key;
        // Key size from [0 .. 2^32]
        uint32_t size_Key;
         // Associated data AD_X 
        uint8_t *AD_X;
        //Associated data size, in [0..2^32]
        uint32_t size_AD_X;
        //Type y OF Argon2 that defines the version of Argon2 : 0 = d , 1 = i , 2 = id 
        uint32_t y;
        
}Argon2_arguments;

const char* ErrorMessage(int error_code);

 void Argon2_Block2(Argon2_arguments* argument, uint8_t* tag);



 void FillSegment(Argon2_Mat* B_Matrix,ARGON2_Indexing* arguments);



//void Finalize(const Argon2_Mat *B_Matrix, Argon2_Block* FinalizeB_Matrix);

//void Finalize(const Argon2_arguments *argument, ARGON2_Indexing* FinalizeBlock);



#endif
