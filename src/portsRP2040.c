#include "stdio.h"
#include "portsRP2040.h"

//#include "hardware/pio.h"
#include "pio_dependences/hardware_pio/include/hardware/pio.h"
#include "portsPIO.pio.h"
#include "pio_dependences/hardware_irq/include/hardware/irq.h"

static PIO pioSM = pio0 ; 
static uint state_machine_number ;  
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

typedef ENUM{
    IDLE 
    START, 
    END, 
}st_irq ; 

static st_irq irq_status =IDLE ; 



void cb_pio_irq(void){
    pioSM->irq = 0x01 ; 
    uint32_t data = pioSM->rxf[state_machine_number] ;  
    uint16_t read_encoder = (uint16_t) data ;   
    uint16_t position = read_encoder & READ_BITS_ENCODER ; 
    // uint8_t k1 = (uint8_t) ((read_encoder & 0x8000 ) >>15)  ; ///odd_bit  
    // uint8_t k0 = (uint8_t) ((read_encoder & 0x4000 ) >>14)  ; ///even_bit  
    // uint8_t check_bits_odd = ! (uint8_t)  (     
    //     ((read_encoder &H5_MASK)>>13) ^    
    //     ((read_encoder &H3_MASK)>>11) ^  
    //     ((read_encoder &H1_MASK)>>9) ^  
    //     ((read_encoder &L7_MASK)>>7) ^  
    //     ((read_encoder &L5_MASK)>>5) ^  
    //     ((read_encoder &L3_MASK)>>3) ^  
    //     ((read_encoder &L1_MASK)>>1)   
    
    // ); 
    // uint8_t check_bits_even  = !(uint8_t)  ( 
        
    //     ((read_encoder &H4_MASK)>>12) ^    
    //     ((read_encoder &H2_MASK)>>10) ^  
    //     ((read_encoder &H0_MASK)>>8)  ^  
    //     ((read_encoder &L6_MASK)>>6)  ^  
    //     ((read_encoder &L4_MASK)>>4)  ^  
    //     ((read_encoder &L2_MASK)>>2)  ^  
    //     ((read_encoder &L0_MASK))   
    
    // ); 
    // if (check_bits_even ==k0  &&  check_bits_odd == k1){
    //     read_encoder_amt(&position) ; 
    // }else {
    //     read_encoder_amt( NULL ) ; 
    // }



} 



void initHW(int port_clk,int port_data) { 
    state_machine_number = pio_claim_unused_sm(pioSM, true);
    uint offset = pio_add_program(pioSM, &AMT223BV_program);
    AMT223BV_init_hw(pioSM, state_machine_number, offset,16,port_clk,port_data);    
    pioSM->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS; // 0x...300
    irq_set_exclusive_handler(PIO0_IRQ_0,&cb_pio_irq) ;             ///NVIC CONTROLLER 
    irq_set_enabled(PIO0_IRQ_0,true) ;
}    





void startSM(){
    pioSM->irq = 0x04 ; 
}