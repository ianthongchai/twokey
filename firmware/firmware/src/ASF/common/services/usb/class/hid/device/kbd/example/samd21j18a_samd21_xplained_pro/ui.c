/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include "ui.h"

void ui_init(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BUTTON_0_PIN, &config_port_pin);
	port_pin_set_config(BUTTON_1_PIN, &config_port_pin); 
	port_pin_set_config(PIN_PA00, &config_port_pin); 
}

void ui_powerdown(void)
{
	//LED_Off(LED_0_PIN);
}


void ui_wakeup_enable(void)
{
	//extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	//extint_chan_enable_callback(BUTTON_1_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void ui_wakeup_disable(void)
{
	//extint_chan_disable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	//extint_chan_disable_callback(BUTTON_1_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void ui_wakeup(void)
{
	//LED_On(LED_0_PIN);
}

void setLED(uint8_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

void ui_process(uint16_t framenumber)
{
	bool b_btn_state;
	static bool btn_last_state = false;

	b_btn_state = !port_pin_get_input_level(BUTTON_0_PIN);
	if (b_btn_state != btn_last_state) {
		btn_last_state = b_btn_state;
		
		if(b_btn_state) {
			udi_hid_kbd_down(HID_E);
			setLED(0, 8, 1, 135, 255);
		} else {
			udi_hid_kbd_up(HID_E);
			setLED(0, 0, 0, 0, 0);
		}
	}
	
	bool b_btn_state2;
	static bool btn_last_state2 = false;

	b_btn_state2 = !port_pin_get_input_level(BUTTON_1_PIN);
	if (b_btn_state2 != btn_last_state2) {
		btn_last_state2 = b_btn_state2;
		
		if(b_btn_state2) {
			udi_hid_kbd_down(HID_R);
			setLED(1, 8, 1, 135, 255);			
		} else {
			udi_hid_kbd_up(HID_R);
			setLED(1, 0, 0, 0, 0);			
		}
	}
}

void ui_kbd_led(uint8_t value)
{
	//UNUSED(value);
}
