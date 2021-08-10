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

uint8_t  sine_wave[256] = {
	0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
	0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
	0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
	0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
	0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
	0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
	0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
	0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
	0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
	0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
	0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
	0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
	0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
	0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
	0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
	0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
	0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
	0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
	0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
	0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
	0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
	0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
	0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
	0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
	0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
	0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
	0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
	0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
	0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
	0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

void ui_init(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BUTTON_0_PIN, &config_port_pin);
	port_pin_set_config(BUTTON_1_PIN, &config_port_pin); 
	port_pin_set_config(BUTTON_2_PIN, &config_port_pin); 
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

static uint32_t animation = 1;

static uint16_t led1 = 0, led2 = 0;

static uint8_t led1_rgb[] = { 1, 135, 255};
static uint8_t led2_rgb[] = { 1, 135, 255};

void ui_process(uint16_t framenumber)
{
	if(animation ==  0) {
		setLED(0, 0, 0, 0, 0);
		setLED(1, 0, 0, 0, 0);
	} else if (animation == 2) {
		setLED(0, sine_wave[(framenumber>>3)&0b11111111]>>3, led1_rgb[0], led1_rgb[1], led1_rgb[2]);
		setLED(1, sine_wave[(framenumber>>3)&0b11111111]>>3, led2_rgb[0], led2_rgb[1], led2_rgb[2]);
	} else if (animation == 3){
		if (led1>0){
			led1--;
		}
		setLED(0, sine_wave[128+(64-(led1>>2))]>>3, led1_rgb[0], led1_rgb[1], led1_rgb[2]);
		if (led2>0){
			led2--;
		}
		setLED(1, sine_wave[128+(64-(led2>>2))]>>3, led2_rgb[0], led2_rgb[1], led2_rgb[2]);
	}else if(animation == 1){
		setLED(0, 8, led1_rgb[0], led1_rgb[1], led1_rgb[2]);
		setLED(1, 8, led2_rgb[0], led2_rgb[1], led2_rgb[2]);
	}	
	
	bool b_btn_state;
	static bool btn_last_state = false;

	b_btn_state = !port_pin_get_input_level(BUTTON_0_PIN);
	if (b_btn_state != btn_last_state) {
		btn_last_state = b_btn_state;
		
		if(b_btn_state) {
			led1 = 64 << 2;
			udi_hid_kbd_down(HID_R);
		} else {
			udi_hid_kbd_up(HID_R);
		}
	}
	
	bool b_btn_state2;
	static bool btn_last_state2 = false;

	b_btn_state2 = !port_pin_get_input_level(BUTTON_1_PIN);
	if (b_btn_state2 != btn_last_state2) {
		btn_last_state2 = b_btn_state2;
		if(b_btn_state2) {
			led2 = 64 << 2;
			udi_hid_kbd_down(HID_E);
		} else {
			udi_hid_kbd_up(HID_E);
		}
	}

	bool b_btn_state3;
	static bool btn_last_state3 = false;

	b_btn_state3 = !port_pin_get_input_level(BUTTON_2_PIN);
	if (b_btn_state3 != btn_last_state3) {
		btn_last_state3 = b_btn_state3;
	
		if(b_btn_state3) {
			animation++;
			if (animation>3) {
				animation = 0;
			}
			setLED(0, 0, 0, 0, 0);
			setLED(1, 0, 0, 0, 0);
		}
	}
}

void ui_kbd_led(uint8_t value)
{
	//UNUSED(value);
}
