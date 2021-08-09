/**
 * \file
 *
 * \brief Main functions for Keyboard example
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
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
#include "conf_usb.h"
#include "ui.h"

struct spi_module spi_master_instance;
struct spi_slave_inst slave;

void configure_spi_master(void);

void configure_spi_master(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = CONF_MASTER_SS_PIN;
	spi_attach_slave(&slave, &slave_dev_config);
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);

	config_spi_master.mux_setting = CONF_MASTER_MUX_SETTING;
	config_spi_master.pinmux_pad0 = CONF_MASTER_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = CONF_MASTER_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = CONF_MASTER_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = CONF_MASTER_PINMUX_PAD3;

	spi_init(&spi_master_instance, CONF_MASTER_SPI_MODULE, &config_spi_master);

	spi_enable(&spi_master_instance);
}

static volatile bool main_b_kbd_enable = true;

void setLED(uint8_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

void setLED(uint8_t index, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b) {
	//spi_select_slave(&spi_master_instance, &slave, true);
	static uint8_t wr_buffer[16] = {
		// start frame 32 bits
		0x00, 0x00, 0x00, 0x00,
		// global quarter brightness
		0b11100000,
		// blue
		0x00,
		// green
		0x00,
		// red
		0x00,
		// global quarter brightness
		0b11100000,
		// blue
		0x00,
		// green
		0x00,
		// red
		0x00,
		// end frame 32 bits
		0xff, 0xff, 0xff, 0xff
	}, rd_buffer[16];
	
	uint8_t offset = 4 + 4 * index;
	wr_buffer[offset + 0] = brightness | 0b11100000;
	wr_buffer[offset + 1] = b;
	wr_buffer[offset + 2] = g;
	wr_buffer[offset + 3] = r;
	
	while(!spi_is_ready_to_write(&spi_master_instance)) {		
	}
	spi_transceive_buffer_job(&spi_master_instance, wr_buffer,rd_buffer,16);
}

void clearLEDs(void);

void clearLEDs() {
	//spi_select_slave(&spi_master_instance, &slave, true);
	static uint8_t wr_buffer[16] = {
		// start frame 32 bits
		0x00, 0x00, 0x00, 0x00,
		// global quarter brightness
		0b11100000,
		// blue
		0x00,
		// green
		0x00,
		// red
		0x00,
		// global quarter brightness
		0b11100000,
		// blue
		0x00,
		// green
		0x00,
		// red
		0x00,
		// end frame 32 bits
		0xff, 0xff, 0xff, 0xff
	}, rd_buffer[16];

	while(!spi_is_ready_to_write(&spi_master_instance)) {
	}	
	spi_transceive_buffer_job(&spi_master_instance, wr_buffer,rd_buffer,16);
}

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();

#if !SAM0
	sysclk_init();
	board_init();
#else
	system_init();
#endif
	configure_spi_master();
	clearLEDs();
	ui_init();
	ui_powerdown();

	// Start USB stack to authorize VBus monitoring
	udc_start();

	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	while (true) {
#ifdef   USB_DEVICE_LOW_SPEED
		// No USB "Keep a live" interrupt available in low speed
		// to scan keyboard interface then use main loop
		if (main_b_kbd_enable) {
			static volatile uint16_t virtual_sof_sub = 0;
			if (sysclk_get_cpu_hz()/50000 ==
				virtual_sof_sub++) {
				virtual_sof_sub = 0;
				static uint16_t virtual_sof = 0;
				ui_process(virtual_sof++);
			}
		}
#else
		sleepmgr_enter_sleep();
#endif
	}
}

void main_suspend_action(void)
{
	ui_powerdown();
}

void main_resume_action(void)
{
	ui_wakeup();
}

void main_sof_action(void)
{
	if (!main_b_kbd_enable)
		return;
	ui_process(udd_get_frame_number());
}

void main_remotewakeup_enable(void)
{
	ui_wakeup_enable();
}

void main_remotewakeup_disable(void)
{
	ui_wakeup_disable();
}

bool main_kbd_enable(void)
{
	main_b_kbd_enable = true;
	return true;
}

void main_kbd_disable(void)
{
	main_b_kbd_enable = false;
}


/**
 * \mainpage ASF USB Device HID Keyboard
 *
 * \section intro Introduction
 * This example shows how to implement a USB Device HID Keyboard
 * on Atmel MCU with USB module.
 * The application note AVR4904 http://ww1.microchip.com/downloads/en/appnotes/doc8446.pdf
 * provides information about this implementation.
 *
 * \section startup Startup
 * The example uses the buttons or sensors available on the board
 * to simulate a standard keyboard.
 * After loading firmware, connect the board (EVKxx,Xplain,...) to the USB Host.
 * When connected to a USB host system this application provides a keyboard application
 * in the Unix/Mac/Windows operating systems.
 * This example uses the native HID driver for these operating systems.
 *
 * \copydoc UI
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and HID modules:
 *   <br>services/usb/
 *   <br>services/usb/udc/
 *   <br>services/usb/class/hid/
 *   <br>services/usb/class/hid/keyboard/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>manages UI
 *    - specific implementation for each target "./examples/product_board/":
 *       - conf_foo.h   configuration of each module
 *       - ui.c        implement of user's interface (buttons, leds)
 */
