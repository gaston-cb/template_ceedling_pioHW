//#include "pico/stdlib.h"
#include <stdint.h>
#define BINARY_DEC  0.021974 

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





typedef struct 
{
  uint8_t port_clk ; 
  uint8_t port_data ; 
  uint8_t port_cs ; 
  zero_t zero_position ; 
  uint16_t raw_data ;      /// refer to zero position 
  reference_t reference ; 
  float angle_position ;   /// refer to zero position 
}amt23_t;
typedef void (*calibration_fn)(amt23_t *) ; 








void initamt(uint8_t port_clk, uint8_t port_data, calibration_fn user,reference_t reference) ; 
void read_encoder_amt(amt23_t *read_encoder,read_correct_t *read) ; 
void set_cero_amt(read_correct_t *lect) ; 
//void get_info_encoder(amt23_t *data) ; 

// volatile void read_encoder_amt(uint16_t *raw_data) ; 
// void start_read() ; 
// uint16_t get_angle(amt223bv_t *amt); 
// uint8_t get_stat_read_encoder_amt() ; 
// void setZero_amt(); 