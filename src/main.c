/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An
  * @version V1.0
  * @date    Oct 10, 2022
  * @brief   ECE 362 Lab 5 template
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdint.h>

void initb();
void initc();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);
void buttons(void);
void keypad(void);

void mysleep(void) {
    for(int n = 0; n < 1000; n++);
}

int main(void) {
    // Uncomment when most things are working
    autotest();
    
    initb();
    initc();

    // uncomment one of the loops, below, when ready
    // while(1) {
    //   buttons();
    // }

    // while(1) {
    //   keypad();
    // }

    for(;;);
}

/**
 * @brief Init GPIO port B
 *        Pin 0: input
 *        Pin 4: input
 *        Pin 8-11: output
 *
 */
void initb() {
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;  //enable the clock for B Ports
    GPIOB -> MODER &= ~0x00ff0303;         //keep ports you don't want to change at 0 then set ports you want to change to 11
    GPIOB -> MODER |=  0x00550000;         //set 8-11 to output and 0 and 4 to input
}

/**
 * @brief Init GPIO port C
 *        Pin 0-3: inputs with internal pull down resistors
 *        Pin 4-7: outputs
 *
 */
void initc() {
    RCC   -> AHBENR|= RCC_AHBENR_GPIOCEN;
    GPIOC -> MODER &= ~0x0000ffff;
    GPIOC -> MODER |=  0x00005500;
    GPIOC -> PUPDR &= ~0x000000ff;
    GPIOC -> PUPDR |=  0x000000aa;
}

/**
 * @brief Set GPIO port B pin to some value
 *
 * @param pin_num: Pin number in GPIO B
 * @param val    : Pin value, if 0 then the
 *                 pin is set low, else set high
 */
void setn(int32_t pin_num, int32_t val) {
    if(val == 0) {
        GPIOB -> BSRR = 1 << (pin_num + 16);
    }
    else {
        GPIOB -> BSRR = 1 << pin_num;
    }
}

/**
 * @brief Read GPIO port B pin values
 *
 * @param pin_num   : Pin number in GPIO B to be read
 * @return int32_t  : 1: the pin is high; 0: the pin is low
 */
int32_t readpin(int32_t pin_num) {
    uint16_t portb = GPIOB->IDR;    // Takes in the whole 16 bit value
    return (portb >> pin_num) & 1;  //shifts the port you want to the beginning then & with 1 to leave only that pin
                                    // Example   0x0001000  then shift   0x00000001 & 1     would delete any 1 that might come after
}

/**
 * @brief Control LEDs with buttons
 *        Use PB0 value for PB8
 *        Use PB4 value for PB9
 *
 */
void buttons(void) {
    int32_t pin0 = readpin(0);
    setn(8, pin0);
    int32_t pin4 = readpin(4);
    setn(9, pin4);
}

/**
 * @brief Control LEDs with keypad
 *
 */
void keypad(void) {
    for(int i = 1; i<5; i++) {
        GPIOC -> ODR = 0x1 << (i + 3);
        mysleep();
        uint16_t portc = (GPIOC -> IDR) & 0xf;
        if(i==1) {
            setn(8, portc & 1);
        }
        else if(i==2){
            setn(9, portc & 2);
        }
        else if(i==3){
            setn(10, portc & 4);
        }
        else if(i==4){
            setn(11, portc & 8);
        }
        //setn(12 - i, portc & (1 << (i-1)));
    }

}
