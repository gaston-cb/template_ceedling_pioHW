//#include "pico/stdlib.h"
#define AMT223_12BITS 
#define AMT223_14BITS 
#define BINARY_DEC  0.021974 
typedef enum{
  COUNTER_CLOCKWISE ,
  COUNTER_ANTICLOCKWISE, 
  COUNTER_STILL ,
}sentido_t ; 

typedef struct 
{
  uint16_t raw_data_zero; 
  float angle_zero_position ; 
}zero_t ;


typedef struct{
  uint16_t  rawdata ; 
  uint16_t  value_data ; 
  zero_t    value_set_zero ; 
  sentido_t sentido ; 
  float     angle   ; 
}amt223bv_t; 




typedef unsigned int uint ; 
void initamt(uint port_clk, uint port_data) ; 
volatile void read_encoder_amt(uint16_t *raw_data) ; 
void start_read() ; 
uint16_t get_angle(amt223bv_t *amt); 
uint8_t get_stat_read_encoder_amt() ; 
void setZero_amt(); 