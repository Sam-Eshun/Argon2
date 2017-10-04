

#ifndef ARGON2_MATRIX_H
#define	ARGON2_MATRIX_H
#include "Argon2Encoding.h"


#define  ARGON2_MEM_BLOCK_SIZE 1024
#define Argon2_H_O_LENGTH 64
#define  Argon2_H_O_SEED_LENGTH 72
#define  Argon2_Block_length 128 
#define  Agon2_WORK_LENGTH 8  


 
typedef struct {
    uint8_t v[1024];    
} Argon2_Block;



typedef struct{
     uint8_t*  mem_matrix;
     uint32_t m_block;
     uint32_t p_rows;
     uint8_t q_columns;
    uint32_t length_Segment;
}Argon2_Mat;


typedef struct{
   
    //Argon2_Block* memory;

      // number of memory blocks
        uint64_t m_block;
       //number of pass
        uint64_t r_pass;
       //lane number
        uint64_t l_lanes;
       //total  number of passes
        uint64_t t_pass;
        //uint32_t lenght_lane;
       uint64_t i_index;
       // Number of columns
        uint64_t c_colnum;
       
       uint64_t reference_blocks;
        uint64_t current_reference_block;
       
         uint32_t length_Segment;
        
       //slice number
       uint64_t s_slice;
       uint64_t counter;
        uint64_t version;
        uint64_t partitioned_pairs[128];
        uint64_t y;
}ARGON2_Indexing;





void ARGON2_Indexing_Initializer(ARGON2_Indexing *argument,uint32_t m_block, uint32_t t_pass,  uint32_t version);

int Argon2_Mat_Initializer(uint32_t m_block, uint32_t p_rows, Argon2_Mat* B_Matrix);



 /* int Argon2_Mat_Fill_Block(const uint32_t i_index, uint64_t j_index, 
        Argon2_Mat *dst, const Argon2_Block *src); */
  
  //int Argon2_Mat_Fill_Block(const uint32_t i_index, uint64_t j_index,Argon2_Mat *dst, const Argon2_Block *src);
 int Argon2_Mat_Fill_Block( uint32_t i_index, uint32_t j_index, 
        Argon2_Mat* dst,  Argon2_Block* src);
//void FillMemoryBlocks(ARGON2_Indexing* index);

 int Argon2_Mat_Release_Block( uint32_t i_index, uint32_t j_index,  Argon2_Block* dst,Argon2_Mat* src);
 
 
 // int Argon2_Mat_Release_Block(const uint32_t i_index, uint64_t j_index,
  //         Argon2_Mat *dst, const Argon2_Block *src);
  
  
  
//void Argon2_Mat_Release_Block(const uint32_t i_index, uint64_t j_index,
         //  Argon2_Mat *dst, const Argon2_Block *src);

 
 
void ClearMemory(Argon2_Mat* B_Matrix);


uint64_t Argon_Index( ARGON2_Indexing* argument , Argon2_Mat* B_Matrix);

//uint32_t Index(const ARGON2_Indexing *argument , const Argon2_Mat* B_Matrix);


//extern void Initialize_Sbox(ARGON2_Indexing *argument);

#endif	/* ARGON2_MAT_H */

  
