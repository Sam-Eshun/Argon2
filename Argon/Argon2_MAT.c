/* 
 * File:   Argon2_MAT.c
 * Author: Sammy
 *
 * Created on March 5, 2017, 10:15 PM
 */
#include "Argon2_MAT.h"
#include <stdio.h>
#include <stdlib.h>


//Initializing the arguments for the argon2 indexing 
void ARGON2_Indexing_Initializer(ARGON2_Indexing *argument,uint32_t m_block, uint32_t t_pass, uint32_t version)
{
   
        (*argument).m_block = m_block;
        (*argument). t_pass= t_pass;
        (*argument).version = version;
        (*argument).r_pass = 0;          
        (*argument).l_lanes = 0;
        (*argument).c_colnum =0;
        (*argument).s_slice = 0;
        (*argument).i_index = 1;
        (*argument).counter = 0;
}

//Initializing the Matrix and set its parameters 
int Argon2_Mat_Initializer(uint32_t m_block_Ini, uint32_t p_rows, Argon2_Mat* B_Matrix)
{
    

     if (p_rows == 0 ||m_block_Ini < 8* p_rows || p_rows >0xFFFFFF )   
         ERROR("Argon2: Internal error: Illegal parameters");
     
     
    B_Matrix ->p_rows = p_rows;
    
    B_Matrix ->m_block = 4*p_rows*(m_block_Ini/(4*p_rows));
    B_Matrix ->q_columns = B_Matrix -> m_block/p_rows ;
    
    B_Matrix ->length_Segment = B_Matrix -> q_columns/4;
      
    
    
     B_Matrix ->mem_matrix = (uint8_t*)malloc(B_Matrix ->m_block*ARGON2_MEM_BLOCK_SIZE);
     
      memset(B_Matrix ->mem_matrix,0,B_Matrix ->m_block*ARGON2_MEM_BLOCK_SIZE);
      
     return 0;
    
}




// Fills the blocks in position (i,j) in the Argon2  B_Matrix with the content of the source block
int Argon2_Mat_Fill_Block( uint32_t i_index,  uint32_t j_index, 
        Argon2_Mat *dst, Argon2_Block *src){
    
    if(i_index > (dst->p_rows - 1 ) || j_index > (dst->q_columns - 1))
    
       return 1;
    
    
    memcpy ((dst->mem_matrix)+(ARGON2_MEM_BLOCK_SIZE)*(j_index + dst ->
                    q_columns*i_index),src ->v,ARGON2_MEM_BLOCK_SIZE);
 
                return 0;
                
                
} 
  


// Getting the block ERROR;  in position (i,j) in the Argon2  B_Matrix, storing the content in the dst block

int Argon2_Mat_Release_Block( uint32_t i_index, uint32_t j_index,  Argon2_Block *dst,Argon2_Mat *src){
    
    if(i_index > (src->p_rows - 1) || j_index > (src->q_columns - 1))
        ERROR("Argon2: Internal error: Passing of wrong parameters");
           
    
             memcpy(dst->v,(src->mem_matrix)+ARGON2_MEM_BLOCK_SIZE*(j_index+src->
                     q_columns*i_index),ARGON2_MEM_BLOCK_SIZE);
             
        return 0;
  
}

  //Freeing the memory
void ClearMemory(Argon2_Mat* B_Matrix){

free(B_Matrix->mem_matrix);
}




//Getting J_1 and J_2 as specified in Argon2d
uint64_t Argon2d_description(Argon2_Mat* B_Matrix, uint32_t i_index, uint32_t j_index){

           Argon2_Block buff;
            uint32_t J_1[2];
          j_index += B_Matrix ->q_columns *(j_index == 0);
           
           if(Argon2_Mat_Release_Block(i_index,j_index-1,&buff,B_Matrix))
             
              ERROR("Invalid indeces, reading random bytes from RAM") ;
        memcpy(J_1,buff.v,8);

        return *((uint64_t*)J_1);
            
            
           
}

// Each application of the 2-round compression function G in the counter
  // mode gives 128 64-bit values J_1 || J_2  The first input is the all
  // zero block 


void Argon2i_description(ARGON2_Indexing* arguments){
        uint64_t Full_zero_block_X[128];
        uint64_t input_block[128];
        memset(Full_zero_block_X,0,128*sizeof(uint64_t));
        // the second input is constructed as follows  ( r || l || s || m' || t || x || i || 0 ),
        input_block[0] = arguments->r_pass;                                      
        input_block[1] = arguments->l_lanes;                                      
        input_block[2] = arguments->s_slice;                                      
        input_block[3] = arguments->m_block;                                     
        input_block[4] = arguments->t_pass; 
        input_block[5] = arguments->version;
        input_block[6] = arguments->i_index;
        input_block[7] = arguments->y;
       memset(input_block+7, 0, 121*sizeof(uint64_t)); 
       
       Argon2_Comp_G(Full_zero_block_X,  input_block, arguments->partitioned_pairs);                        
       Argon2_Comp_G(Full_zero_block_X, arguments->partitioned_pairs, arguments->partitioned_pairs);

        arguments->i_index++;                                               
        arguments->counter = 0;  
        
}


//Mapping J_1 and J_2 to reference block index based on rules in the specification 
uint64_t Argon2_Mapping(ARGON2_Indexing* arguments, Argon2_Mat* B_Matrix, uint64_t J_1){
    
    struct {
        uint32_t l_lanes;
        uint64_t index;
        uint64_t reference_blocks;
        uint64_t current_reference_block;
        
        uint32_t pair[2];
       
    }reference_block;
      
       reference_block.pair[0] = (uint32_t)J_1;
      reference_block.pair[1] = (uint32_t)(J_1>> 32);
    
     reference_block.l_lanes = reference_block.pair[1] % B_Matrix->p_rows;
     
         
         if(arguments->r_pass == 0){
             if(arguments->s_slice==0)
                  reference_block.reference_blocks = arguments->c_colnum - 1;
             
          else{
                   if(reference_block.l_lanes == arguments->l_lanes) 
                    
                   reference_block.reference_blocks = arguments->
                           s_slice*B_Matrix->length_Segment  +  
                             (arguments->c_colnum % B_Matrix->length_Segment) - 1; 
            
               else                                                   
                        reference_block.reference_blocks = arguments->s_slice*
                                   B_Matrix->length_Segment 
                                - ((arguments->c_colnum % B_Matrix->length_Segment) == 0); 
          }
            
             
           reference_block.current_reference_block = 0 ;
           
     
               }else{
        if(reference_block.l_lanes == arguments->l_lanes)                                               
                       reference_block.reference_blocks = 3*B_Matrix->length_Segment +             
                        (arguments->c_colnum % B_Matrix->length_Segment) - 1; 
        
     
        else
            
              reference_block.reference_blocks = (3*B_Matrix->length_Segment -            
                        ((arguments->c_colnum % B_Matrix->length_Segment) == 0));                   
         
             reference_block.current_reference_block   = (arguments->s_slice+1 % 4)*
                     B_Matrix->length_Segment; 
        }
     
     
        reference_block.index = (reference_block.pair[0]);                                                       
       reference_block.index = (reference_block.index*reference_block.index) >> 32;                                         
       reference_block.index = (reference_block.reference_blocks*reference_block.index) >> 32;                             
       reference_block.index = reference_block.reference_blocks - 1 - reference_block.index;                               
        reference_block.index+= reference_block.current_reference_block;                                     
      reference_block.index = reference_block.index % B_Matrix->q_columns;                                                  
        reference_block.index^= ((uint64_t)(reference_block.l_lanes) << 32);                                            

        return (reference_block.index);
     
     
}

uint64_t Argon_Index( ARGON2_Indexing *argument , Argon2_Mat* B_Matrix){
    
   // uint32_t Argon_Index(const ARGON2_Indexing *argument , const Argon2_Mat* B_Matrix){
    
    if(argument->version==1 ||
         ((argument->version==2) && (argument->r_pass==0) && (argument->s_slice < 2))){
        if(argument->counter==128||argument->counter==0)
            
            Argon2i_description(argument);
            
        argument->counter++;
        return Argon2_Mapping(argument,B_Matrix, argument->partitioned_pairs[argument->counter-1]);
        
    }
    
    else
        return 
                Argon2_Mapping(argument,B_Matrix, Argon2d_description
                (B_Matrix,argument->l_lanes, argument->c_colnum));
    
    
    
}