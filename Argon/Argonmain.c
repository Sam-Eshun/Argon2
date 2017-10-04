/* 
 * File:   main.cpp
 * Author: Sammy
 *
 * Created on December 4, 2016, 2:22 AM
 */
//#include "Argon2_MAT.h"

#include "argon2.h"







/*
 * 
 */

/*
int main(int argc, char** argv) {

    return 0;
}

 */




int main() {
    for (int k = 0; k <= 100 * Argon2Mem_Test_Leaks; k++) {

        Argon2_arguments arguments;

        // typedef struct {
        uint8_t P_string[32];
        uint8_t S_Nonce[16];
        uint8_t Key[8];
        uint8_t AD_X[12];
        uint8_t T_tag[32];

        // }Argon2_hash_arg;

        memset(P_string, 0x01, 32);
        memset(S_Nonce, 0x02, 16);
        memset(Key, 0x03, 8);
        memset(AD_X, 0x04, 12);


        arguments.P_string = P_string;
        arguments.size_P_string = 32;
        arguments.S_Nonce = S_Nonce;
        arguments.Nonce_size_S = 16;
        arguments.p_lism = 4;
        arguments.T_tag = 32;
        arguments.m_cost = 32;
        arguments.iterate = 3;
        arguments.version = 0x13;
        arguments.Key = Key;
        arguments.size_Key = 8;
        arguments.AD_X = AD_X;
        arguments.size_AD_X = 12;
        
        arguments.y = 0;
        Argon2_Block2(&arguments, T_tag);
        printf("Test of Argon2d \n");

        for (int k = 0; k < arguments.T_tag; k++)

            printf("%02X ", T_tag[k]);
        printf("\n\n");
        
        arguments.y = 1;
        Argon2_Block2(&arguments, T_tag);
        printf("Test of Argon2i \n");

        for (int k = 0; k < arguments.T_tag; k++)

            printf("%02X ", T_tag[k]);
        printf("\n\n");
        

        arguments.y = 2;
        Argon2_Block2(&arguments, T_tag);
        printf("Test of Argon2id \n");

        for (int k = 0; k < arguments.T_tag; k++)

            printf("%02X ", T_tag[k]);
        printf("\n\n");
    
    }
    return 0;

}