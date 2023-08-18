#include <stdio.h>
#include <string.h>
//#include "pico/stdlib.h"
#include "stdint.h"
#include "stdlib.h"
#include "AMT23.h"
#include "portsRP2040.h"
/// res14b 0.00006
#define MAX14BITS 16383 




static void set_start_read_encoder_amt() ; 
static void set_end_read_encoder_amt() ; 

static uint16_t convert_angle_to_zero(uint16_t d2c);
static uint16_t rawdata_last  = 0 ; 
static amt223bv_t encoder ; 
static uint8_t end_read_encoder_amt  = 0; 



void initamt(uint port_clk, uint port_data){
  encoder.rawdata = 0 ;  
  initHW(port_clk,port_data)  ; 
}


/// @brief inicia la fsm para realizar la lectura del 
void start_read(){  
  set_start_read_encoder_amt() ; 
  startSM() ; 
}


/**
 * @brief Set the Zero object
 * 
 */
void setZero_amt(){ 
  encoder.value_set_zero = encoder.rawdata  ;  ///set a zero position to a measurement angle 
  rawdata_last = convert_angle_to_zero(encoder.rawdata) ;
  encoder.rawdata = rawdata_last ; 
}

uint16_t get_direction(){ 
  
  return encoder.sentido ; 

} 

uint16_t get_angle(amt223bv_t *amt)
{ 
  ///FIXME: add a converter function  
  if ( encoder.rawdata > rawdata_last ){ 
    encoder.sentido = COUNTER_ANTICLOCKWISE ; 
  }else if( encoder.rawdata < rawdata_last  ){
    encoder.sentido = COUNTER_CLOCKWISE ; 
  }else if (rawdata_last == encoder.rawdata){
    encoder.sentido = COUNTER_STILL ;
  }
  encoder.angle = (encoder.rawdata)*(360.0/MAX14BITS) ; 
  memcpy(amt,&encoder,sizeof(encoder)) ; 
  return encoder.rawdata ;  
}

volatile void read_encoder_amt(uint16_t *raw_data){ 
  set_end_read_encoder_amt() ; 
  if (raw_data !=NULL){ 
    rawdata_last    = encoder.rawdata ; 
    encoder.rawdata = convert_angle_to_zero(*raw_data)    ; 
  }
}




static uint16_t convert_angle_to_zero(uint16_t d2c){ 
  return (d2c - encoder.value_set_zero +  MAX14BITS)%MAX14BITS ;  
}

static void set_start_read_encoder_amt(){ 
  end_read_encoder_amt = 1 ; 
}

static void set_end_read_encoder_amt(){ 
  end_read_encoder_amt = 0 ; 
}

uint8_t get_stat_read_encoder_amt(){ 
  return end_read_encoder_amt ; 
}