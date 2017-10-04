/* 
 * File:   Blake2b.h
 * Author: Sammy
 *
 * Created on February 23, 2017, 11:18 PM
 */
//#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef BLAKE2B_H
#define	BLAKE2B_H




#define   BLAKE2_OUTBYTES   64
#define   Blake2_Round_Num 12 
#define  BLAKE2_BLOCKBYTES 128
#define   Blake_WORK_LENGTH 8  

      
     


#define Register1 32       
#define Register2 24
#define Register3 16
#define Register4 63


  
  void blake2b ( uint8_t* hash,size_t hashlen, uint8_t* indata ,uint64_t indatalen);  
  
static inline void size32( void *dst, uint32_t w );

  
#ifdef	__cplusplus
extern "C" {
#endif

#if !defined ERROR
#define ERROR(msg) {puts((char*)msg); exit(1);}
#endif
 
  
    
static inline uint64_t ROT_64( const uint64_t e, const unsigned f )
{
  return ( e >> f ) | ( e << ( 64 - f ) );
}

 
#ifdef	__cplusplus
}
#endif

#endif	/* BLAKE2B_H */
