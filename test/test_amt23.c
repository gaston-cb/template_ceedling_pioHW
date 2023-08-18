#include "unity.h" 
#include "AMT23.h"
#include "mock_portsRP2040.h"
//#include "mock_portsRP2040.h"



void setUp(void) { 
} 



void test_init_encoder(void){ 
    initamt(8,9) ; 
    TEST_ASSERT_EQUAL(8,8) ; 
}