
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




// void setUp(void) { 
//     initHW_Ignore() ; 

// } 


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
    float angle = (float)(read_position)*(360.0/(0x3FFF+1)) ; 
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
    char msg[10] ; 
    TEST_ASSERT_EQUAL(sizeof(angle), sizeof(encoder_data.zero_position.angle_zero)) ; 
    for (i = 0 ; i<0x3FFF+1;i++){
        angle = (float)(i)*(360.0/(0x3FFFu+1u)) ; 
        return_data_pio_sm(generate_values_amt(i)) ;
        set_cero_amt(&lect); 
        TEST_ASSERT_EQUAL(lect, OK) ; 
        TEST_ASSERT_EQUAL(i, encoder_data.zero_position.raw_data) ;  
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE (angle, encoder_data.zero_position.angle_zero,msg) ; 
        resetTest() ;
    }
}

/*******************************TESTING DE POSICION Y SISTEMAS DE REFERENCIA ***************/

// void test_lectura_encoders_fail(void){ 
// }

/// @brief se prueba el calculo de la posición referenciada al sistema clockwise 
/// @param   
void test_lectura_encoders_reference_anticlockwise_primer_cuadrante(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t zero_raw_data = 1502 ; 
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    // primer cuadrante 
    // 33º -> 1502 DIG_VALUE
    return_data_pio_sm(generate_values_amt(zero_raw_data)) ; 
    set_cero_amt(&lect); 
    /// valor mayor a 33º 
    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt((uint16_t)2000)) ; 
    angle =(float)(2000)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,zero_raw_data) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+2000-zero_raw_data)%16384) ;
    //printf("position is: %f\r\n",angle-angle_zero) ;  
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    //angulo menor a 1502 
    return_data_pio_sm(generate_values_amt((uint16_t)1501)) ; 
    angle =(float)(1500)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
      
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,zero_raw_data) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+1501-zero_raw_data)%16384) ;
    TEST_ASSERT_EQUAL(data_read_fn.angle_position, encoder_data.angle_position) ;      
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 
}

void test_lectura_encoders_reference_anticlockwise_segundo_cuadrante(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 6826 ; 
    uint16_t mayor_zero = 7000; 
    uint16_t menor_zero = 6200; 
    
    float angle ; 
    float angle_zero ; 

    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    // segundo cuadrante 
    // 6826 ->  150º aproach 
    return_data_pio_sm(generate_values_amt(raw_zero)) ; 
    set_cero_amt(&lect); 
    /// valor mayor a 150º  
    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+mayor_zero-raw_zero)%16384  ) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    //printf("position is: %f\r\n",angle-angle_zero) ;  
    //angulo menor a 150º  
    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+menor_zero-raw_zero)%16384  ) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,encoder_data.angle_position) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

 }

void test_lectura_encoders_reference_anticlockwise_tercer_cuadrante(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 10465 ; 
    uint16_t mayor_zero = 11000; 
    uint16_t menor_zero = 10200; 
    
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    // tercer cuadrante 
    // 10465 ->  230º aproach 
    return_data_pio_sm(generate_values_amt(raw_zero)) ; 
    set_cero_amt(&lect); 
    /// valor mayor a 230  
    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+mayor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

    //printf("position is: %f\r\n",angle-angle_zero) ;  
    //angulo menor a 230º  
    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+menor_zero-raw_zero)%16384);
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position, encoder_data.angle_position) ; 
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 
     
}

void test_lectura_encoders_reference_anticlockwise_cuarto_cuadrante(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ;
    uint16_t raw_zero  = 15018 ; 
    uint16_t mayor_zero = 15400; 
    uint16_t menor_zero = 14900;  
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    // cuarto cuadrante 
    // 15018 ->  330º aproach 
    return_data_pio_sm(generate_values_amt(raw_zero)) ; 
    set_cero_amt(&lect); 
    /// valor mayor a 330º  
    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+mayor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

    //printf("position is: %f\r\n",angle-angle_zero) ;  
    //angulo menor a 330º  
    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+menor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position, encoder_data.angle_position) ;      
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

}

void test_equal_zero_anticlockwise(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ;
    uint16_t raw_zero  = 0x00 ; 
    uint16_t mayor_zero = 25; 
    uint16_t menor_zero = 14900;  
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    return_data_pio_sm(generate_values_amt(raw_zero)) ; 
    set_cero_amt(&lect); 


    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+mayor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 

    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+menor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 
}
/***********************************PROCEDIMIENTOS DE TEST PARA CLOCKWISE **************************/



void test_lectura_encoders_reference_clockwise_primer_cuadrante(void){ 

    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 1502 ; 
    uint16_t mayor_zero = 2000; 
    uint16_t menor_zero = 1300; 
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 
    return_data_pio_sm(generate_values_amt(raw_zero)) ;
    set_cero_amt(&lect); 
    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;

    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-mayor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,360.0+angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
   
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-menor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 
}

void test_lectura_encoders_reference_clockwise_segundo_cuadrante(void){
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 6826 ; 
    uint16_t mayor_zero = 7000; 
    uint16_t menor_zero = 6200; 
    float angle,angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 

    return_data_pio_sm(generate_values_amt(raw_zero)) ;
    set_cero_amt(&lect); 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    angle_zero =(float)(raw_zero)*(360.0/(0x3FFF+1)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 

    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;    
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-mayor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,360.0+angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-menor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 
} 


void test_lectura_encoders_reference_clockwise_tercer_cuadrante(void){
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 10465 ; 
    uint16_t mayor_zero = 11000; 
    uint16_t menor_zero = 10200; 
    float angle,angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 

    return_data_pio_sm(generate_values_amt(raw_zero)) ;
    set_cero_amt(&lect); 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    angle_zero =(float)(raw_zero)*(360.0/(0x3FFF+1)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 

    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;    
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-mayor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,360.0+angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-menor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 
} 


void test_lectura_encoders_reference_clockwise_cuarto_cuadrante(void){
    read_correct_t lect ; 
    amt23_t data_read_fn ; 
    uint16_t raw_zero  = 15018 ; 
    uint16_t mayor_zero = 15400; 
    uint16_t menor_zero = 14900; 
    float angle,angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,CLOCKWISE) ; 

    return_data_pio_sm(generate_values_amt(raw_zero)) ;
    set_cero_amt(&lect); 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    angle_zero =(float)(raw_zero)*(360.0/(0x3FFF+1)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 

    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;    
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-mayor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,360.0+angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ; 
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+raw_zero-menor_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle_zero-angle) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position ); 
} 


void test_equal_zero_clockwise(void){ 
    read_correct_t lect ; 
    amt23_t data_read_fn ;
    uint16_t raw_zero  = 0x00 ; 
    uint16_t mayor_zero = 25; 
    uint16_t menor_zero = 14900;  
    float angle ; 
    float angle_zero ; 
    initHW_Ignore() ; 
    initamt(PORT_CLK,PORT_DATA,user_test,ANTICLOCKWISE) ; 
    return_data_pio_sm(generate_values_amt(raw_zero)) ; 
    set_cero_amt(&lect); 


    angle_zero = encoder_data.zero_position.angle_zero; 
    return_data_pio_sm(generate_values_amt(mayor_zero)) ; 
    angle =(float)(mayor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 
    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+mayor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 

    return_data_pio_sm(generate_values_amt(menor_zero)) ; 
    angle =(float)(menor_zero)*(360.0/(0x3FFF+1)) ; 
    read_encoder_amt(&data_read_fn, &lect) ; 

    TEST_ASSERT_EQUAL(OK,lect) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.raw_data,raw_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.zero_position.angle_zero,angle_zero) ;
    TEST_ASSERT_EQUAL(data_read_fn.raw_data,(16384+menor_zero-raw_zero)%16384) ;
    TEST_ASSERT_EQUAL_FLOAT(data_read_fn.angle_position,angle-angle_zero) ;
    printf("line: %d - position Zero: %f - angle_read: %f, transform %f\r\n",__LINE__,angle_zero,angle,data_read_fn.angle_position); 
}

