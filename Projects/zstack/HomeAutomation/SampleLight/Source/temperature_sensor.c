#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "MT_SYS.h"

#include "nwk_util.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_diagnostic.h"

#include "zcl_samplelight.h"
   
#include "bdb.h"
#include "bdb_interface.h"

 //GP_UPDATE
#include "gp_interface.h"
   
#include "onboard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"

#include "NLMEDE.h"


#include "hal_board_cfg.h"
#include "temperature_sensor.h"

#define MSK      (BV(0) | BV(1) | BV(2))
#define TEMPSENSOR_SKIP_ROM 		0xCC
#define TEMPSENSOR_CONVERT_T 		0x44
#define TEMPSENSOR_READ_SCRATCHPAD         0xBE


void delay_us(uint16);
void delay_ms(uint16);
void temperature_sensor_send(uint8);
uint8 temperature_sensor_read(void);
void temperature_sensor_send_byte(int8);
uint8 temperature_sensor_read_byte(void);
uint8 temperature_sensor_RST_PULSE(void);


void delay_us(uint16 microSecs) {
  while (microSecs--) {
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  }
}

void delay_ms(uint16 milliSecs) {
  while (milliSecs--) {
    delay_us(1000);
  }
}

// Sends one bit to bus
void temperature_sensor_send(uint8 bit) {
  TSENS_SBIT = 1;
  TSENS_DIR |= TSENS_BV;  // output
  TSENS_SBIT = 0;
  if (bit != 0)
    delay_us(8);
  else
    delay_us(80);
  TSENS_SBIT = 1;
  if (bit != 0)
    delay_us(80);
  else
    delay_us(2);
  //TSENS_SBIT = 1;
}

// Reads one bit from bus
uint8 temperature_sensor_read(void) {
  TSENS_SBIT = 1;
  TSENS_DIR |= TSENS_BV;  // output
  TSENS_SBIT = 0;
  delay_us(2);
  //TSENS_SBIT = 1;
  //_delay_us(15);
  TSENS_DIR &= ~TSENS_BV;  // input
  delay_us(5);
  uint8 i = TSENS_SBIT;
  delay_us(60);
  return i;
}

// Sends one byte to bus
void temperature_sensor_send_byte(int8 data) {
  uint8 i, x;
  for (i = 0; i < 8; i++) {
    x = data >> i;
    x &= 0x01;
    temperature_sensor_send(x);
  }
  //_delay_us(100);
}

// Reads one byte from bus
uint8 temperature_sensor_read_byte(void) {
  uint8 i;
  uint8 data = 0;
  for (i = 0; i < 8; i++) {
    if (temperature_sensor_read()) data |= 0x01 << i;
    //_delay_us(25);
  }
  return (data);
}

// Sends reset pulse
uint8 temperature_sensor_RST_PULSE(void) {
  TSENS_SBIT = 0;
  TSENS_DIR |= TSENS_BV;  // output
  delay_us(600);
  TSENS_DIR &= ~TSENS_BV;  // input
  delay_us(70);
  uint8 i = TSENS_SBIT;
  delay_us(200);
  TSENS_SBIT = 1;
  TSENS_DIR |= TSENS_BV;  // output
  delay_us(600);
  return i;
}

uint16 readTemperature(void) {
  float temperature = 0;
  uint8 temp1, temp2;
  if (!temperature_sensor_RST_PULSE()) {
    temperature_sensor_send_byte(TEMPSENSOR_SKIP_ROM);
    temperature_sensor_send_byte(TEMPSENSOR_CONVERT_T);
    delay_ms(750);
    temperature_sensor_RST_PULSE();
    temperature_sensor_send_byte(TEMPSENSOR_SKIP_ROM);
    temperature_sensor_send_byte(TEMPSENSOR_READ_SCRATCHPAD);
    temp1 = temperature_sensor_read_byte();
    temp2 = temperature_sensor_read_byte();
    temperature_sensor_RST_PULSE();

    if (temp1 == 0xff && temp2 == 0xff) {
      // No sensor found.
      return 0;
    }
    temperature = (uint16)temp1 | (uint16)(temp2 & MSK) << 8;
    // neg. temp
    if (temp2 & (BV(3)))
      temperature = temperature / 16.0 - 128.0;
    // pos. temp
    else
      temperature = temperature / 16.0;

    return (uint16)(temperature * 100);
  } else {
    // Fail
    return 1;
  }  
}
