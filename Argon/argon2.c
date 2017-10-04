/* 
 * File:   argon2.c
 * Author: Sammy
 *
 * Created on February 27, 2017, 10:11 PM
 */


#include "argon2.h"



//#include "Blake2b.h"


//The initialization of the first two columns 

 void Fill_H_O(Argon2_arguments *args, uint8_t* H_O){
    uint8_t* H_O_enter = (uint8_t*)malloc(10*4+args->size_P_string+args->Nonce_size_S+args->size_Key+args->size_AD_X);
	uint8_t* H_O_enter_curr = H_O_enter;
    
        CAT_N_Bytes(H_O_enter_curr,&(args->p_lism),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->T_tag),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->m_cost),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->iterate),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->version),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->y),4);
	CAT_N_Bytes(H_O_enter_curr,&(args->size_P_string),4);
	CAT_N_Bytes(H_O_enter_curr,args->P_string,args->size_P_string);
	CAT_N_Bytes(H_O_enter_curr,&(args->Nonce_size_S),4);
	CAT_N_Bytes(H_O_enter_curr,args->S_Nonce,args->Nonce_size_S);
	CAT_N_Bytes(H_O_enter_curr,&(args->size_Key),4);
	CAT_N_Bytes(H_O_enter_curr,args->Key,args->size_Key);
	CAT_N_Bytes(H_O_enter_curr,&(args->size_AD_X),4);
	CAT_N_Bytes(H_O_enter_curr,args->AD_X,args->size_AD_X);

        blake2b((void*)H_O, Argon2_H_O_LENGTH, (void*)H_O_enter,  H_O_enter_curr - H_O_enter);

	free(H_O_enter);
        
     

}  



//Function to compute the first block
 void Fill_First_Block(Argon2_Mat* B_Matrix, uint8_t* H_O, uint32_t T_tag, uint32_t c_colnum){

    uint8_t* H_Ini_enter;
    Argon2_Block Block;
    
    memset(Block.v,0,ARGON2_MEM_BLOCK_SIZE);
    
   H_Ini_enter = (uint8_t*) malloc(Argon2_H_O_SEED_LENGTH); 
	memcpy(H_Ini_enter, H_O, Argon2_H_O_LENGTH); 
	memcpy(H_Ini_enter+Argon2_H_O_LENGTH, &c_colnum, 4);
        
        for(uint32_t k=0; k< B_Matrix->p_rows; k++)        
        {
            memcpy(H_Ini_enter+(Argon2_H_O_LENGTH+4), &k, 4);
            Argon2_Hash(H_Ini_enter, Argon2_H_O_SEED_LENGTH, ARGON2_MEM_BLOCK_SIZE,Block.v);
            
            
            if(Argon2_Mat_Fill_Block(k,c_colnum,B_Matrix,&Block))
                ERROR("Argon2:: Error in filling block");
               

	}
        
        free(H_Ini_enter);
        
 
}
        //Funtion to compute all blocks in the segment 
 void FillSegment(Argon2_Mat* B_Matrix, ARGON2_Indexing* arguments){
    
    
  uint64_t i_index_ref;
  uint64_t j_ref_lane;
   Argon2_Block i_j_ref_Block;
   Argon2_Block lane_Block;
    Argon2_Block ref_Block;
   
   
   
    
   for(arguments->s_slice = 0 ; arguments->s_slice <ARGON_SYNC_POINTS ; arguments->s_slice++){
       
       for( arguments->l_lanes = 0; arguments->l_lanes < B_Matrix->p_rows; arguments->l_lanes++){
           
           if((0==arguments->s_slice) && (0 == arguments->r_pass))
              
              
               arguments->c_colnum = (ARGON_SYNC_POINTS/2);
               
           else
               arguments->c_colnum = arguments->s_slice*B_Matrix->length_Segment;
           arguments->i_index = 1;
           
           arguments->counter = 0;
           
           for(; arguments->c_colnum < (arguments->s_slice+1)*B_Matrix->length_Segment; 
                   
                   arguments->c_colnum++){
               
               
                    i_index_ref = Argon_Index(arguments,B_Matrix);
           
                            j_ref_lane = i_index_ref & 0x00000000FFFFFFFF;
               
              
            i_index_ref = i_index_ref >> (Argon2_H_O_LENGTH/2) ;
           
             if(Argon2_Mat_Release_Block(arguments->l_lanes,arguments->c_colnum - 1
                        +((arguments->c_colnum == 0) ? B_Matrix->q_columns : 0), & ref_Block, B_Matrix))
                        ERROR("Error in getting block [l_lanes,c_colnum - 1]");
             
             
                 if(Argon2_Mat_Release_Block(i_index_ref,j_ref_lane, &lane_Block, B_Matrix))
                     ERROR("Error in getting block  [i_index_ref',j_ref_lane']");
          
                 
             
             Argon2_Comp_G((uint64_t*)(ref_Block.v) ,
                          (uint64_t*)(lane_Block.v),
                         (uint64_t*)(i_j_ref_Block.v)); 
            
   
                 
             if(Argon2_Mat_Release_Block(arguments->l_lanes, arguments->c_colnum, & ref_Block,B_Matrix))
            
                 ERROR("Error in getting block  [l_lanes,c_colnum]");
              
             
             
             
             Argon2_XOR((uint64_t*)i_j_ref_Block.v,
                     (uint64_t *)ref_Block.v,
                     (uint64_t*)i_j_ref_Block.v);
                    
           
             
             if(Argon2_Mat_Fill_Block(arguments->l_lanes, arguments->c_colnum, B_Matrix, &i_j_ref_Block))
             
                 ERROR("Argon2: Problem in filling block ");
             
             
             }
             
            
             
           }
           
           
       
   
}


}

 
 //Funtion to compute the final block
 void Finalize(Argon2_Mat *B_Matrix, Argon2_Block* FinalizeB_Matrix){
   
    
    Argon2_Block Blockhash;
    
    for (uint32_t k = 0; k < B_Matrix->p_rows; ++k){
        
       if(Argon2_Mat_Release_Block(k, B_Matrix->q_columns-1, &Blockhash,B_Matrix))
                    ERROR("A2B:: Unable to get final blocks");
      
                
              
        
      Argon2_XOR((uint64_t*)(FinalizeB_Matrix->v), (uint64_t*)(Blockhash.v),(uint64_t*)(FinalizeB_Matrix->v)) ;
      
       
    }       
    
}
    


//function to perform the computations storing the output in a tag
 void Argon2_Block2(Argon2_arguments* argument, uint8_t* tag){
    
  Argon2_Mat B_Matrix;
  
  if(Argon2_Mat_Initializer(argument->m_cost,argument->p_lism, &B_Matrix))
            ERROR("Error in  parameters (m_cos,p_lism)");
       
  
  ARGON2_Indexing first_argument;
  
  ARGON2_Indexing_Initializer(& first_argument, B_Matrix.m_block, argument->iterate, argument->y);
 
  
  uint8_t H_O[Argon2_H_O_LENGTH];
  Fill_H_O(argument,H_O);
    
  Fill_First_Block(&B_Matrix,H_O,argument->T_tag,0);
  Fill_First_Block(&B_Matrix,H_O,argument->T_tag,1);
  
  for( first_argument.r_pass=0; first_argument.r_pass < first_argument.t_pass; first_argument.r_pass++){
      
       FillSegment(&B_Matrix,&first_argument);
      
  }
      
      Argon2_Block FInalBlockB;
      
      
      memset(FInalBlockB.v,0,ARGON2_MEM_BLOCK_SIZE);
      
      Finalize(&B_Matrix, &FInalBlockB);
      Argon2_Hash(FInalBlockB.v,ARGON2_MEM_BLOCK_SIZE,argument->T_tag,tag);
  
      ClearMemory(&B_Matrix);
  
}