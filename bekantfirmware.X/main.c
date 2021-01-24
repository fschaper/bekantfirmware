#include <xc.h> // XC8 General Include File

#include <stdint.h>
#include <stdbool.h>

#include "system.h"        // System funct/params, like osc/peripheral config
#include "user.h"          // User funct/params, such as InitApp

#include "btn/btn.h"
#include "lin/lin_d.h"
#include "bekant/bui.h"

#include <pic.h>

void main(void) {
    /* Configure the oscillator for the device */
    ConfigureOscillator();

// This code seems to help get it out of a "stuck" state
// When the echo from TX back to RX is not working
//    TRISC = 0x00; // All outputs
//
//    while (true) {
//    //for (uint8_t j = 0; j < 200; j++) {
//        LATC = 0xff;
//        for (uint8_t i = 0; i <= 0x10; i++) {
//            CLRWDT();
//            for (uint8_t k = 0; k < 0x10; k++) {
//                
//            }
//        }
//
//        LATC = 0x00;
//        for (uint8_t i = 0; i <= 0x10; i++) {
//            CLRWDT();
//            for (uint8_t k = 0; k < 0x10; k++) {
//                
//            }
//        }
//    }


    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    TXSTAbits.TXEN = 1; // Transmit enabled

    ANSELB = 0b00000000;
    TRISB = 0b00000011;
    INPUT_t last_input = INPUT_IDLE;
    
    TRISC = 0b10000000; // C7: input (serial RX)

    lin_init_hw();

    // TEST CODE TO DEBUG IN SIMULATOR
    //lin_id = 0x11;
    //lin_tx_frame();
    
    //lin_id = 0x12;
    //lin_data_count = 3;
    //lin_data = (uint8_t[]){0, 0, 0};
    //lin_tx_frame();
    
    while (1) {
        ButtonState_t button_state = (ButtonState_t)PORTBbits;
        
        if (btn_debounce(button_state)) {
            INPUT_t input = btn_gesture(button_state);
            
            if (input != last_input) {
                last_input = input;

                bui_input(input);
            }
        }
        
        CLRWDT();
        while (!PIR1bits.TMR2IF);
        PIR1bits.TMR2IF = false;
    }
}