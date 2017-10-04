#include <stdio.h>
#include "Argon2_MAT.h"
#include  "Argon2Encoding.h"

int main(){

Argon2_Mat Matrix;
Argon2_Mat_Initializer(8, 1, &Matrix);
Argon2_Block Block;
memset(&(Block.v),0xFF,ARGON2_MEM_BLOCK_SIZE);
             for(uint32_t k = 0;k<8;k++)
                Argon2_Mat_Fill_Block(0,k,&Matrix,&Block);
                
                memset(&(Block.v),0,ARGON2_MEM_BLOCK_SIZE);
                
                for(uint32_t k = 0; k < 8; k++){
                    Argon2_Mat_Release_Block(0,k,&Block,&Matrix);
                    for(int k = 0;k<16;k++)
                        printf("%01X",Block.v[k]);
                    printf("\n");
                }
                
                ARGON2_Indexing arguments;
                arguments.r_pass = 2;
                arguments.l_lanes = 0;
                arguments.c_colnum = 2048;
                arguments.m_block = 8*ARGON2_MEM_BLOCK_SIZE;
                arguments.s_slice = 2;
                arguments.t_pass = 12;
                arguments.version = 1;
                arguments.i_index = 0;
                arguments.counter = 0;
                
                
                
               printf("Argon2i  indexes [Argon2i]: %016llX\n", Argon_Index(&arguments,&Matrix));
               arguments.r_pass = 2;
               arguments.l_lanes = 0;
               arguments.c_colnum = 6;
               arguments.m_block = 8;
               arguments.s_slice = 0;
               arguments.t_pass = 12;
               arguments.version = 0;
               arguments.i_index = 0;
               
               arguments.counter = 0;
               
               
               
               printf("Argon2d  indexes [Argon2d]: %016llX\n", Argon_Index(&arguments,&Matrix));
               return  0;
              
}


