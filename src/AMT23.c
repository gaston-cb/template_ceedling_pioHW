#include <stdio.h>
#include <string.h>

#include "AMT23.h"
#include "portsRP2040.h"

#define MAX14BITS 16383u // 2**14 
#ifndef TEST
  #define TYPE_DATA static 
#else
  #define TYPE_DATA   
#endif 

#define NUMBER_BITS 15 

/// ODD BITS    k1 k0 h5 h4 h3 h2 h1 h0 , l7,l6,l5,l5,l3,l2,l1,l0  
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



static read_correct_t checkBytes(uint16_t check_value) ; 
static void transform_coordinates(uint16_t read_angle) ; 
TYPE_DATA amt23_t encoder_data; 
static calibration_fn user_function ; 
static uint8_t fixed_point[2] ; 




void initamt(uint8_t port_clk, uint8_t port_data,calibration_fn user,reference_t reference){
  user_function = user ; 
  encoder_data.port_clk = port_clk ; 
  encoder_data.port_data = port_data ; 
  encoder_data.port_cs = port_clk+1  ; 
  encoder_data.reference = reference ; 
  initHW(port_clk,port_data)  ; 
}


void read_encoder_amt(amt23_t *read_encoder,read_correct_t *read){ 
  uint16_t data_encoder; 
  startSM() ; 
  data_encoder = get_data(); //blocking funcion 
  
  if (checkBytes(data_encoder)== ERROR){ 
    *read = ERROR ; 
    return ; 
  }

  transform_coordinates(data_encoder& READ_BITS_ENCODER) ; 
  memcpy(read_encoder,&encoder_data, sizeof(encoder_data)) ; 
  if (user_function ==NULL){
    return ; 
  }
  user_function(read_encoder) ; 

}


void set_cero_amt(read_correct_t *lectura){ 
  uint16_t data_encoder ; 
  startSM() ; 
  data_encoder = get_data() ; 
  if (checkBytes(data_encoder)==ERROR){ 
    *lectura = ERROR ; 
    return ; 
  }else { 
    *lectura = OK; 
  } 

  encoder_data.zero_position.raw_data   =  data_encoder & READ_BITS_ENCODER ; 
  encoder_data.zero_position.angle_zero =  (float)encoder_data.zero_position.raw_data *(360.0/(0x3FFFu+1u)) ; 
}



static read_correct_t checkBytes(uint16_t check_value){
    read_correct_t report_check = ERROR ; //RETURN VARIABLE 
    uint8_t k1 = (uint8_t) ((check_value & 0x8000 ) >>15)  ; ///odd_bit  
    uint8_t k0 = (uint8_t) ((check_value & 0x4000 ) >>14)  ; ///even_bit  
    uint8_t check_bits_odd = ! (uint8_t)  (     
        ((check_value &H5_MASK)>>13) ^    
        ((check_value &H3_MASK)>>11) ^  
        ((check_value &H1_MASK)>>9) ^  
        ((check_value &L7_MASK)>>7) ^  
        ((check_value &L5_MASK)>>5) ^  
        ((check_value &L3_MASK)>>3) ^  
        ((check_value &L1_MASK)>>1)   
    
    ); 
    uint8_t check_bits_even  = !(uint8_t)  ( 
        
        ((check_value &H4_MASK)>>12) ^    
        ((check_value &H2_MASK)>>10) ^  
        ((check_value &H0_MASK)>>8)  ^  
        ((check_value &L6_MASK)>>6)  ^  
        ((check_value &L4_MASK)>>4)  ^  
        ((check_value &L2_MASK)>>2)  ^  
        ((check_value &L0_MASK))   
    
    );  

    if (k1 == check_bits_odd && k0 == check_bits_even){
      report_check = OK ; 
    }
    return report_check ; 
}







void transform_coordinates(uint16_t read_angle){ 
  

  if (encoder_data.reference == CLOCKWISE){
    encoder_data.raw_data = (16384 + encoder_data.zero_position.raw_data - read_angle)%(16384) ; 
    encoder_data.angle_position    = (float) encoder_data.raw_data *(360.0/(16384)) ; 
  }else if (encoder_data.reference == ANTICLOCKWISE){ 
    encoder_data.raw_data = ( (16384 + read_angle) - encoder_data.zero_position.raw_data)%(16384) ; 
    encoder_data.angle_position    = (float) encoder_data.raw_data *(360.0/(16384)) ;  
  }





}