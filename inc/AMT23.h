//#include "pico/stdlib.h"
#include <stdint.h>
#define BINARY_DEC  0.021974 
typedef void (*calibration_fn)(uint16_t) ; 

typedef enum {
  OK, 
  ERROR, 
}read_correct_t ; 

typedef enum {
  CLOCKWISE, 
  ANTICLOCKWISE, 
}reference_t ; 



typedef struct{
  uint16_t raw_data; 
  float angle_zero ; 
}zero_t ; 


typedef struct{
  uint16_t raw_data ; 
  float angle_north ; 
}north_t ; 


typedef enum {
  CCW,   // counter-clockwise  -- al reves agujas reloj
  CW,    // clockwise -- agujas del reloj
  STILL, 
}turn_sense_t;




typedef struct 
{
  uint8_t port_clk ; 
  uint8_t port_data ; 
  uint8_t port_cs ; 
  north_t nort_position ;  /// refer to absolute position  
  zero_t zero_position ; 
  uint16_t raw_data ;      /// refer to zero position 
  reference_t reference ; 
  turn_sense_t turning; 
  float angle_position ;   /// refer to zero position 
}amt23_t;







// inicializacion 
// setear cero 
// lectura 
// setear referencia 



void initamt(uint8_t port_clk, uint8_t port_data, calibration_fn user,reference_t reference) ; 
void read_encoder_amt(amt23_t *read_encoder,read_correct_t *read) ; 
void set_cero_amt(read_correct_t *lect) ; 


// volatile void read_encoder_amt(uint16_t *raw_data) ; 
// void start_read() ; 
// uint16_t get_angle(amt223bv_t *amt); 
// uint8_t get_stat_read_encoder_amt() ; 
// void setZero_amt(); 