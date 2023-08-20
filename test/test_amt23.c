
#include "unity.h" 

#include "AMT23.h"
#include "mock_portsRP2040.h"
#include "values_test.h"
#define MAX16bits 0xFFFFu
// values for test_init_amt() ; 
#define PORT_CLK 8 
#define PORT_DATA 9 
#define PORT_CS PORT_CLK+1  

// value for a test pio_sm 
#define VALUE_READ 0xABCD 


//#include "mock_portsRP2040.h"
extern amt23_t encoder_data ; 
calibration_fn user_test ; 
void return_data_pio_sm(uint16_t value_return){ 
    startSM_Ignore() ; 
    get_data_ExpectAndReturn(value_return) ; 
}




//void setUp(void) { 
//    initHW_Ignore() ; 
//
//} 


/// test de inicialización de valores 

void test_init_encoder(void){ 
    user_test = NULL ; 
    initHW_Ignore() ;
    initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 
    TEST_ASSERT_EQUAL(PORT_CLK,encoder_data.port_clk) ;
    TEST_ASSERT_EQUAL(PORT_CS,encoder_data.port_cs) ;
    TEST_ASSERT_EQUAL(PORT_DATA,encoder_data.port_data) ;
    TEST_ASSERT_EQUAL(CLOCKWISE,encoder_data.reference) ;

    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    TEST_ASSERT_EQUAL(PORT_CLK,encoder_data.port_clk) ;
    TEST_ASSERT_EQUAL(PORT_CS,encoder_data.port_cs) ;
    TEST_ASSERT_EQUAL(PORT_DATA,encoder_data.port_data) ;
    TEST_ASSERT_EQUAL(ANTICLOCKWISE,encoder_data.reference) ;


}


/***************TESTING DE CERO ****************************************/

void test_zero_correct(void){ 
    read_correct_t lect ; 
    read_correct_t oK = OK  ; 
    read_correct_t error = ERROR  ; 
    uint16_t data_correct  = 0x61AB ; 
    uint16_t read_position = 0x21AB ; 
    float angle = (float)(read_position)*(360.0/0x3FFF) ; 
    return_data_pio_sm(data_correct) ;
    set_cero_amt(&lect); 
    TEST_ASSERT_EQUAL(lect, OK) ; 
    TEST_ASSERT_EQUAL(read_position, encoder_data.zero_position.raw_data) ;  
    TEST_ASSERT_EQUAL_FLOAT (angle, encoder_data.zero_position.angle_zero) ; 
    resetTest() ;
    data_correct = 0xE1AB ; //change parity bit 
    return_data_pio_sm(data_correct) ;
    set_cero_amt(&lect); 
    TEST_ASSERT_EQUAL(error,lect) ; 
}


void test_zero_encoder(void){ 
    read_correct_t lect ; 
    uint16_t value_encoder ; 
    uint16_t i  ; 
    float angle ; 
    for (i = 0 ; i<0x3FFF+1;i++){
        angle = (float)(i)*(360.0/0x3FFF) ; 
        return_data_pio_sm(generate_values_amt(i)) ;
        set_cero_amt(&lect); 
        TEST_ASSERT_EQUAL(lect, OK) ; 
        TEST_ASSERT_EQUAL(i, encoder_data.zero_position.raw_data) ;  
        TEST_ASSERT_EQUAL_FLOAT (angle, encoder_data.zero_position.angle_zero) ; 
        resetTest() ;
    }
}

/*******************************TESTING DE POSICION Y SISTEMAS DE REFERENCIA ***************/

// void test_lectura_encoders_fail(void){ 




// }

// void test_lectura_encoders_reference_clockwise(void){ 
//     read_correct_t lect ; 
//     amt23_t data_read_fn ; 
//     float angle ; 
//     float angle_zero ; 
//     uint16_t i,j ; 
//     //initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 
//     char message[80] ; 
//     for (i = 0 ; i<0x3FFF;i++){
//         return_data_pio_sm(generate_values_amt(i)) ;
//         set_cero_amt(&lect); 
//         angle_zero = encoder_data.zero_position.angle_zero; 
//         for (j=0;j<16384;j++){ 
//             return_data_pio_sm(generate_values_amt(i)) ;
//             read_encoder_amt(&data_read_fn,&lect) ; 
            
//             sprintf(message,"angle_zero =%f - position_abs = %d - delta_Hex = %d -delta = %f -i=%d j=%d"
//                     ,angle_zero,
//                     j,
//                     ((0x3FFF)+(j-encoder_data.zero_position.raw_data))%(0x3FFF),
//                     encoder_data.angle_position,i,j) ; 
                    

//             //TEST_ASSERT_EQUAL(OK,lect) ; 
//             //TEST_ASSERT_EQUAL_MESSAGE(((0x3FFF)+(j-encoder_data.zero_position.raw_data))%(0x3FFF), encoder_data.raw_data, message) ; 
//             printf("i=%d, j=%d\r\n",i,j) ; 

//             resetTest() ;          

//         }
//     }//for zero0 -> calcular delta_i 


//     //for zero1 -> calcular delta_i 
    




// }


//void test_lectura_encoders_reference_anticlockwise(void){ }
