
//#include <math.h>
//#include <stdio.h>
#include "Argon2Encoding.h"

#include "argon2.h"
//#include "Argon2_MAT.h"
//#include "Blake2b.h"
//#include <string.h>



// Utility function to Xor the two arrays
void Argon2_XOR(uint64_t* X_block, uint64_t* Y_block, uint64_t* outre){

    for(int k =0; k< 128; ++k)
        outre[k] =X_block[k]^ Y_block[k];
      


}

// the core function for the permutation P based on the Blake2b function

void Round_G(uint64_t* a, uint64_t* b, uint64_t* c, uint64_t* d){

	*a = *a + *b + 2*trunc(*a)*trunc(*b);
	*d = ROT_64(*d ^ *a, 32);
	*c = *c + *d + 2*trunc(*c)*trunc(*d);
	*b = ROT_64(*b ^ *c, 24);
	*a = *a + *b + 2*trunc(*a)*trunc(*b);
	*d = ROT_64(*d ^ *a, 16);
	*c = *c + *d + 2*trunc(*c)*trunc(*d);
	*b = ROT_64(*b ^ *c, 63);

}

//Permutation P  based on the round function of BLAKE2b


void BLAKE2_Permutation(uint64_t* V){
    //column step
	Round_G( V + 0, V + 4, V +  8, V + 12);  
	Round_G( V + 1, V + 5, V +  9, V + 13);
	Round_G( V + 2, V + 6, V + 10, V + 14);
	Round_G( V + 3, V + 7, V + 11, V + 15);
        //Diagonal step
	Round_G( V + 0, V + 5, V + 10, V + 15);
	Round_G( V + 1, V + 6, V + 11, V + 12);
	Round_G( V + 2, V + 7, V +  8, V + 13);
	Round_G( V + 3, V + 4, V +  9, V + 14);

}

 


/*
 * the main compression functions of Argon2 which takes two input 
 * thus two arrays of 1024 bytes and compresses them into one array
 * of 1024 bytes.
 */
void Argon2_Comp_G(uint64_t* X_block, uint64_t* Y_block, uint64_t* Output){

    uint64_t block_R[128];           

    Argon2_XOR(X_block, Y_block, block_R);

    uint64_t block_Q[128];                        
    memcpy(block_Q,block_R,sizeof(block_R));
        
    for (int k = 0; k < 8; ++k)
        BLAKE2_Permutation(block_Q+16*k);
    
    uint64_t columns[16];
    for   (int k = 0; k < 8 ; ++k){
        
        for (int r = 0; r < 8; ++r)
        { 
             columns[2*r] = block_Q[16*r+2*k];
        
             columns[2*r+1] = block_Q[16*r+1+2*k];
            
        }
        
        BLAKE2_Permutation(columns);
        
        for (int r=0; r<8; ++r){
    		block_Q[16*r+2*k] = columns[2*r];       
    		block_Q[16*r+1+2*k] = columns[2*r+1];
               
    	}
        
    }
        
        	Argon2_XOR(block_Q,block_R,Output); 
        
    }
     

// Variable-length hash function based on Blake2b
       
void Argon2_Hash(uint8_t*AD_X,uint32_t size_AD_X, uint32_t T_tag,uint8_t* hash_value){
    
    
    uint8_t* CONCAT_AD_X_or_T_tag;
    
        CONCAT_AD_X_or_T_tag = (uint8_t*) malloc(size_AD_X+ARGON_SYNC_POINTS);

        memcpy(CONCAT_AD_X_or_T_tag, &T_tag, ARGON_SYNC_POINTS);
        
        memcpy(CONCAT_AD_X_or_T_tag+ARGON_SYNC_POINTS, AD_X, size_AD_X);


        if(T_tag <= Argon2_H_O_LENGTH)
            
                 blake2b(hash_value,T_tag,CONCAT_AD_X_or_T_tag,size_AD_X+ARGON_SYNC_POINTS);

        else
        {
            
           
           uint32_t r = (T_tag/(Argon2_H_O_LENGTH/2))-2;
           
            uint8_t V_1[Argon2_H_O_LENGTH];
            
              blake2b(V_1,Argon2_H_O_LENGTH,CONCAT_AD_X_or_T_tag, size_AD_X+ARGON_SYNC_POINTS);  
               memcpy(hash_value, V_1, (Argon2_H_O_LENGTH/2));
               
                for (int k = 1; k < r; ++k){

                    blake2b(V_1,Argon2_H_O_LENGTH,V_1,Argon2_H_O_LENGTH);                     
                    memcpy(hash_value+k*(Argon2_H_O_LENGTH/2), V_1, (Argon2_H_O_LENGTH/2));   
               
}

               
               blake2b(V_1, T_tag-((Argon2_H_O_LENGTH/2)*r), V_1,Argon2_H_O_LENGTH);                   
                
                    memcpy(hash_value+((Argon2_H_O_LENGTH/2)*r),V_1, T_tag-((Argon2_H_O_LENGTH/2)*r));                

        }   
        
        
        free(CONCAT_AD_X_or_T_tag);
               
 

}
