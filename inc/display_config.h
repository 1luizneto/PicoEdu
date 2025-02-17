#ifndef __DISPLAY_CONF_H__
#define __DISPLAY_CONF_H__

// Choose a bus
#define SSD1306_USE_I2C
//#define SSD1306_USE_SPI

// I2C Configuration
#define SSD1306_I2C_PORT        i2c1
#define SSD1306_I2C_ADDR        0x3C 

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26
#define SSD1306_INCLUDE_FONT_16x24
#define SSD1306_INCLUDE_FONT_16x15


 #define SSD1306_WIDTH           128

 #define SSD1306_HEIGHT          64

#endif 
