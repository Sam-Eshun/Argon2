

//#ifndef _Blake2_

#include "Blake2b.h"



static const uint64_t blake2_IV[8]= {


  0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
    0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
    0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
    0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
};


static const size_t blake2_SIGMA[12][16] ={

  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
   { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
   { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
   { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
   { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
   { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
   { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
   { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
   { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
   { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
   { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
   { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
};



//Blake2b mixing function
void Argon2_Blake_Compress_G(uint64_t* v, int a, int b, int c, int d, uint64_t x, uint64_t y){
    v[a] = v[a] + v[b] + x;
    v[d] = ROT_64((v[d]^v[a]) , Register1);
    v[c] = v[c] + v[d];
    v[b] = ROT_64((v[b]^v[c]) , Register2);
    v[a] = v[a] + v[b] + y;
    v[d] = ROT_64((v[d]^v[a]) , Register3);
    v[c] = v[c] + v[d];
    v[b] = ROT_64((v[b]^v[c]) , Register4);
    
}


//the main compression function

void  blake2b_Init_parama(uint64_t* out, uint64_t* h, uint64_t* u, int S ){
    
     uint64_t v[2*Blake_WORK_LENGTH]; 
    memcpy(v,out,BLAKE2_OUTBYTES);
    memcpy(v+Blake_WORK_LENGTH,blake2_IV,BLAKE2_OUTBYTES);
    
     v[12] ^= u[0];
     
     v[13] ^= u[1];
     
     
    if(S)
            v[14] = ~v[14];
     
     
     for(int k = 0; k < Blake2_Round_Num;k++){

        Argon2_Blake_Compress_G( v, 0, 4,  8, 12, h[blake2_SIGMA[k][ 0]], h[blake2_SIGMA[k][ 1]] );
        Argon2_Blake_Compress_G( v, 1, 5,  9, 13, h[blake2_SIGMA[k][ 2]], h[blake2_SIGMA[k][ 3]] );
        Argon2_Blake_Compress_G( v, 2, 6, 10, 14, h[blake2_SIGMA[k][ 4]], h[blake2_SIGMA[k][ 5]] );
        Argon2_Blake_Compress_G( v, 3, 7, 11, 15, h[blake2_SIGMA[k][ 6]], h[blake2_SIGMA[k][ 7]] );
        Argon2_Blake_Compress_G( v, 0, 5, 10, 15, h[blake2_SIGMA[k][ 8]], h[blake2_SIGMA[k][ 9]] );
        Argon2_Blake_Compress_G( v, 1, 6, 11, 12, h[blake2_SIGMA[k][10]], h[blake2_SIGMA[k][11]] );
        Argon2_Blake_Compress_G( v, 2, 7,  8, 13, h[blake2_SIGMA[k][12]], h[blake2_SIGMA[k][13]] );
        Argon2_Blake_Compress_G( v, 3, 4,  9, 14, h[blake2_SIGMA[k][14]], h[blake2_SIGMA[k][15]] );

    }
     
      for(int k = 0; k < Blake_WORK_LENGTH; k++)
          
          out[k] = out[k] ^ v[k] ^ v[k+Blake_WORK_LENGTH];
          
       
     
}


/*
 * Blake2b hash function. Takes an uint8_t data[indatalen] and outputs an uint8_t digest[hashlen] without any key
 */

 void blake2b ( uint8_t* hash,size_t hashlen, uint8_t* indata ,uint64_t indatalen)
 {
 
   
   
     size_t Blake2update = (indatalen/BLAKE2_BLOCKBYTES) + (indatalen%BLAKE2_BLOCKBYTES!=0); 
     
     uint64_t out[Blake_WORK_LENGTH];
      uint64_t t[2];
      uint8_t in_buffer[BLAKE2_BLOCKBYTES];
      t[0]=0 ;
      t[1]=0;
      
      memcpy(out,blake2_IV,  BLAKE2_OUTBYTES);
       out[0] = out[0] ^ 0x01010000 ^ hashlen;  // 0xF6950 0x01010000
       
       for (size_t toproduce=0; toproduce < Blake2update-1; toproduce++){
           
           memcpy(in_buffer,indata+BLAKE2_BLOCKBYTES*toproduce,BLAKE2_BLOCKBYTES);
                   
              t[0]+=BLAKE2_BLOCKBYTES;                                                        
            if(t[0]<BLAKE2_BLOCKBYTES)                                                      
                    t[1]++;        
			blake2b_Init_parama(out,(uint64_t*)in_buffer,t,0);                                           
       }  
        
       memcpy(in_buffer,indata+(indatalen/BLAKE2_BLOCKBYTES)*BLAKE2_BLOCKBYTES,indatalen%BLAKE2_BLOCKBYTES);
      
       memset(in_buffer+(indatalen%BLAKE2_BLOCKBYTES),0,BLAKE2_BLOCKBYTES-indatalen%BLAKE2_BLOCKBYTES);
       
       t[0]+=(indatalen % BLAKE2_BLOCKBYTES == 0) ? BLAKE2_BLOCKBYTES :indatalen%BLAKE2_BLOCKBYTES;       
       
        if(t[0]<(indatalen%BLAKE2_BLOCKBYTES)) 
		t[1]++;
        
        blake2b_Init_parama(out,(uint64_t*)in_buffer,t,1); 
        
        memcpy(hash,out,hashlen); 
       
 }