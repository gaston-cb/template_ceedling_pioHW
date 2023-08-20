// Values for a zero test 
#include <stdint.h>
//#define VALUES_CERO 
#define H5_MASK 0x2000 ///0b 0010 0000 | 0000 0000   
#define H3_MASK 0x0800 ///0b 0000 1000 | 0000 0000
#define H1_MASK 0x0200 ///0b 0000 0010 | 0000 0000
#define L7_MASK 0x0080 ///0b 0000 0000 | 1000 0000
#define L5_MASK 0x0020 ///0b 0000 0000 | 0010 0000
#define L3_MASK 0x0008 ///0b 0000 0000 | 0000 1000
#define L1_MASK 0x0002 ///0b 0000 0000 | 0000 0010

/// ODD BITS                 k1 k0 h5 h4 h3 h2 h1 h0 , l7,l6,l5,l5,l3,l2,l1,l0  
#define H4_MASK 0x1000 ///0b 0001 0000 | 0000 0000   
#define H2_MASK 0x0400 ///0b 0000 0100 | 0000 0000
#define H0_MASK 0x0100 ///0b 0000 0001 | 0000 0000
#define L6_MASK 0x0040 ///0b 0000 0000 | 0100 0000
#define L4_MASK 0x0010 ///0b 0000 0000 | 0001 0000
#define L2_MASK 0x0004 ///0b 0000 0000 | 0000 0100
#define L0_MASK 0x0001 ///0b 0000 0000 | 0000 0001
#define READ_BITS_ENCODER 0x3FFF


uint16_t generate_values_amt(uint16_t position){ 
    uint16_t gen_value ; 
    uint16_t k1 =  !(uint8_t)  (     
         ((position &H5_MASK)>>13) ^    
         ((position &H3_MASK)>>11) ^  
         ((position &H1_MASK)>>9) ^  
         ((position &L7_MASK)>>7) ^  
         ((position &L5_MASK)>>5) ^  
         ((position &L3_MASK)>>3) ^  
         ((position &L1_MASK)>>1)   
    ); 
    k1 =k1<<15 ; 
    uint16_t k0  = !(uint8_t)  (         
         ((position &H4_MASK)>>12) ^    
         ((position &H2_MASK)>>10) ^  
         ((position &H0_MASK)>>8)  ^  
         ((position &L6_MASK)>>6)  ^  
         ((position &L4_MASK)>>4)  ^  
         ((position &L2_MASK)>>2)  ^  
         ((position &L0_MASK))   
    
    ); 
    k0 =k0<<14 ; 
    gen_value =  k1 | k0 | position ;   
    return gen_value ; 
}