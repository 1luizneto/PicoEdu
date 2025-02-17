#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "math.h"
#include "inc/display.h"

//Variável global para indicar que o botão foi pressionado
volatile bool btn_pressed = false;

#if defined(SSD1306_USE_I2C)

const uint8_t I2C_SDA_PIN = 14;
const uint8_t I2C_SCL_PIN = 15;

void ssd1306_Reset(void) {
    /* for I2C - do nothing */
}

/**
 * @brief Função para escrever um comando nos registradores do display.
 *  
 */
void ssd1306_WriteCommand(uint8_t byte) {
    uint8_t buffer[2];           
    buffer[0] = 0x80;            
    buffer[1] = byte;            

    i2c_write_blocking(i2c1, SSD1306_I2C_ADDR, buffer, sizeof(buffer), false);
}

/**
 * @brief Função para enviar um comando nos registradores do display.
 *  
 */
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
    uint8_t temp_buffer[buff_size + 1]; // Buffer temporário
    temp_buffer[0] = 0x40;             // Endereço do registrador (Control byte)
    memcpy(&temp_buffer[1], buffer, buff_size); // Copia os dados para o buffer temporário

    i2c_write_blocking(i2c1, SSD1306_I2C_ADDR, temp_buffer, sizeof(temp_buffer), false);
}

#else
#error "You should define SSD1306_USE_SPI or SSD1306_USE_I2C macro"
#endif


//Buffer da tela
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

//Criando o objeto display
static SSD1306_t SSD1306;


/**
 * @brief Preenche o Screenbuffer com valores de um buffer fornecido de comprimento fixo.
 *  
 */
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len) {
    SSD1306_Error_t ret = SSD1306_ERR;
    if (len <= SSD1306_BUFFER_SIZE) {
        memcpy(SSD1306_Buffer,buf,len);
        ret = SSD1306_OK;
    }
    return ret;
}

/**
 * @brief inicializa o display oled.
 *  
 */
void ssd1306_Init(void) 
{
    ssd1306_Reset();
    sleep_ms(100);

    
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    ssd1306_WriteCommand(SSD1306_SET_DISP); 

    ssd1306_WriteCommand(0x20); 
    ssd1306_WriteCommand(0x00); 
                               

    ssd1306_WriteCommand(0xB0); 

#ifdef SSD1306_MIRROR_VERT
    ssd1306_WriteCommand(0xC0); 
#else
    ssd1306_WriteCommand(0xC8); 
#endif

    ssd1306_WriteCommand(0x00); 
    ssd1306_WriteCommand(0x10); 

    ssd1306_WriteCommand(0x40); 

    ssd1306_SetContrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_WriteCommand(0xA0); 
#else
    ssd1306_WriteCommand(0xA1); 
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd1306_WriteCommand(0xA7); 
    ssd1306_WriteCommand(0xA6); 
#endif


    ssd1306_WriteCommand(0xA8); 
    ssd1306_WriteCommand(SSD1306_HEIGHT - 1);

    ssd1306_WriteCommand(0xA4); 
    ssd1306_WriteCommand(0xD3); 
    ssd1306_WriteCommand(0x00); 

    ssd1306_WriteCommand(0xD5); 
    ssd1306_WriteCommand(0x80); 

    ssd1306_WriteCommand(0xD9); 
    ssd1306_WriteCommand(0xF1); 

    ssd1306_WriteCommand(0xDA); 
#if (SSD1306_HEIGHT == 32)
    ssd1306_WriteCommand(0x02);
#elif (SSD1306_HEIGHT == 64)
    ssd1306_WriteCommand(0x12);
#elif (SSD1306_HEIGHT == 128)
    ssd1306_WriteCommand(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_WriteCommand(0xDB);
    ssd1306_WriteCommand(0x30); 
    ssd1306_WriteCommand(0x8D); 
    ssd1306_WriteCommand(0x14); 
    ssd1306_SetDisplayOn(1); 

    
    ssd1306_Fill(Black);
    
    
    ssd1306_UpdateScreen();
    
    
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
    
    SSD1306.Initialized = 1;
}

/**
 * @brief Função que preenche o display com uma cor.
 *  
 */
void ssd1306_Fill(SSD1306_COLOR color) {
    memset(SSD1306_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

/**
 * @brief Função atualiza os buffers do display.
 *  
 */
void ssd1306_UpdateScreen(void) {
    
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_WriteCommand(0xB0 + i); 
        ssd1306_WriteCommand(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd1306_WriteCommand(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

/**
 * @brief Função que desenha um pixel em uma posição especifica do display.
 *  
 */
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        
        return;
    }
   
 
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else { 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

/**
 * @brief Função que  escreve um char.
 *  
 */
char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
    uint32_t i, b, j;

    if (ch < 32 || ch > 126)
        return 0;

    if (SSD1306_WIDTH < (SSD1306.CurrentX + Font.width) ||
        SSD1306_HEIGHT < (SSD1306.CurrentY + Font.height))
    {

        return 0;
    }
    

    for(i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < Font.width; j++) {
            if((b << j) & 0x8000)  {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            } else {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }
    

    SSD1306.CurrentX += Font.char_width ? Font.char_width[ch - 32] : Font.width;

    return ch;
}

/**
 * @brief Função que escreve uma strig no display.
 *  
 */
char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
    while (*str) {
        if (ssd1306_WriteChar(*str, Font, color) != *str) {
       
            return *str;
        }
        str++;
    }

    return *str;
}

/**
 * @brief Função que seta a posição que as informações vão ser escritas.
 *  
 */
void ssd1306_SetCursor(uint8_t x, uint8_t y) 
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}


/**
 * @brief Função que desenha uma linha.
 *  
 */
void ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;
    
    ssd1306_DrawPixel(x2, y2, color);

    while((x1 != x2) || (y1 != y2)) {
        ssd1306_DrawPixel(x1, y1, color);
        error2 = error * 2;
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        
        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
    return;
}


/**
 * @brief Função que desenha várias linhas.
 *  
 */
void ssd1306_Polyline(const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_COLOR color) {
    uint16_t i;
    if(par_vertex == NULL) {
        return;
    }

    for(i = 1; i < par_size; i++) {
        ssd1306_Line(par_vertex[i - 1].x, par_vertex[i - 1].y, par_vertex[i].x, par_vertex[i].y, color);
    }

    return;
}

static float ssd1306_DegToRad(float par_deg) {
    return par_deg * (3.14f / 180.0f);
}

static uint16_t ssd1306_NormalizeTo0_360(uint16_t par_deg) {
    uint16_t loc_angle;
    if(par_deg <= 360) {
        loc_angle = par_deg;
    } else {
        loc_angle = par_deg % 360;
        loc_angle = (loc_angle ? loc_angle : 360);
    }
    return loc_angle;
}


void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color) {
    static const uint8_t CIRCLE_APPROXIMATION_SEGMENTS = 36;
    float approx_degree;
    uint32_t approx_segments;
    uint8_t xp1,xp2;
    uint8_t yp1,yp2;
    uint32_t count;
    uint32_t loc_sweep;
    float rad;
    
    loc_sweep = ssd1306_NormalizeTo0_360(sweep);
    
    count = (ssd1306_NormalizeTo0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_degree = loc_sweep / (float)approx_segments;
    while(count < approx_segments)
    {
        rad = ssd1306_DegToRad(count*approx_degree);
        xp1 = x + (int8_t)(sinf(rad)*radius);
        yp1 = y + (int8_t)(cosf(rad)*radius);    
        count++;
        if(count != approx_segments) {
            rad = ssd1306_DegToRad(count*approx_degree);
        } else {
            rad = ssd1306_DegToRad(loc_sweep);
        }
        xp2 = x + (int8_t)(sinf(rad)*radius);
        yp2 = y + (int8_t)(cosf(rad)*radius);    
        ssd1306_Line(xp1,yp1,xp2,yp2,color);
    }
    
    return;
}


void ssd1306_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color) {
    const uint32_t CIRCLE_APPROXIMATION_SEGMENTS = 36;
    float approx_degree;
    uint32_t approx_segments;
    uint8_t xp1;
    uint8_t xp2 = 0;
    uint8_t yp1;
    uint8_t yp2 = 0;
    uint32_t count;
    uint32_t loc_sweep;
    float rad;
    
    loc_sweep = ssd1306_NormalizeTo0_360(sweep);
    
    count = (ssd1306_NormalizeTo0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_degree = loc_sweep / (float)approx_segments;

    rad = ssd1306_DegToRad(count*approx_degree);
    uint8_t first_point_x = x + (int8_t)(sinf(rad)*radius);
    uint8_t first_point_y = y + (int8_t)(cosf(rad)*radius);   
    while (count < approx_segments) {
        rad = ssd1306_DegToRad(count*approx_degree);
        xp1 = x + (int8_t)(sinf(rad)*radius);
        yp1 = y + (int8_t)(cosf(rad)*radius);    
        count++;
        if (count != approx_segments) {
            rad = ssd1306_DegToRad(count*approx_degree);
        } else {
            rad = ssd1306_DegToRad(loc_sweep);
        }
        xp2 = x + (int8_t)(sinf(rad)*radius);
        yp2 = y + (int8_t)(cosf(rad)*radius);    
        ssd1306_Line(xp1,yp1,xp2,yp2,color);
    }
    
    // Radius line
    ssd1306_Line(x,y,first_point_x,first_point_y,color);
    ssd1306_Line(x,y,xp2,yp2,color);
    return;
}

/**
 * @brief Função que desenha circulos a partir de um algoritomo de Bresenham's.
 *  
 */
void ssd1306_DrawCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_COLOR par_color) {
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT) {
        return;
    }

    do {
        ssd1306_DrawPixel(par_x - x, par_y + y, par_color);
        ssd1306_DrawPixel(par_x + x, par_y + y, par_color);
        ssd1306_DrawPixel(par_x + x, par_y - y, par_color);
        ssd1306_DrawPixel(par_x - x, par_y - y, par_color);
        e2 = err;

        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if(-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

/**
 * @brief Função que desenha o interior de circulos a partir de um algoritomo de Bresenham's.
 *  
 */
void ssd1306_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_COLOR par_color) {
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT) {
        return;
    }

    do {
        for (uint8_t _y = (par_y + y); _y >= (par_y - y); _y--) {
            for (uint8_t _x = (par_x - x); _x >= (par_x + x); _x--) {
                ssd1306_DrawPixel(_x, _y, par_color);
            }
        }

        e2 = err;
        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if (-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

/**
 * @brief Função que desenha retangulos.
 *  
 */
void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
    ssd1306_Line(x1,y1,x2,y1,color);
    ssd1306_Line(x2,y1,x2,y2,color);
    ssd1306_Line(x2,y2,x1,y2,color);
    ssd1306_Line(x1,y2,x1,y1,color);

    return;
}

/**
 * @brief Função que desenha interior de retangulos.
 *  
 */
void ssd1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
    uint8_t x_start = ((x1<=x2) ? x1 : x2);
    uint8_t x_end   = ((x1<=x2) ? x2 : x1);
    uint8_t y_start = ((y1<=y2) ? y1 : y2);
    uint8_t y_end   = ((y1<=y2) ? y2 : y1);

    for (uint8_t y= y_start; (y<= y_end)&&(y<SSD1306_HEIGHT); y++) {
        for (uint8_t x= x_start; (x<= x_end)&&(x<SSD1306_WIDTH); x++) {
            ssd1306_DrawPixel(x, y, color);
        }
    }
    return;
}

/**
 * @brief Função que desenha inverte retangulos.
 *  
 */
SSD1306_Error_t ssd1306_InvertRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  if((x2 >= SSD1306_WIDTH) || (y2 >= SSD1306_HEIGHT)) 
  {
    return SSD1306_ERR;
  }
  if((x1 > x2) || (y1 > y2)) 
  {
    return SSD1306_ERR;
  }
  uint32_t i;
  if ((y1 / 8) != (y2 / 8)) 
  {    
    for(uint32_t x = x1; x <= x2; x++) 
    {
      i = x + (y1 / 8) * SSD1306_WIDTH;
      SSD1306_Buffer[i] ^= 0xFF << (y1 % 8);
      i += SSD1306_WIDTH;
      for (; i < x + (y2 / 8) * SSD1306_WIDTH; i += SSD1306_WIDTH) {
        SSD1306_Buffer[i] ^= 0xFF;
      }
      SSD1306_Buffer[i] ^= 0xFF >> (7 - (y2 % 8));
    }
  } 
  else 
  {
    
    const uint8_t mask = (0xFF << (y1 % 8)) & (0xFF >> (7 - (y2 % 8)));
    for (i = x1 + (y1 / 8) * SSD1306_WIDTH;
         i <= (uint32_t)x2 + (y2 / 8) * SSD1306_WIDTH; i++) {
      SSD1306_Buffer[i] ^= mask;
    }
  }
  return SSD1306_OK;
}

/**
 * @brief Função que desenha um bitmap.
 *  
 */
void ssd1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_COLOR color) {
    int16_t byteWidth = (w + 7) / 8; 
    uint8_t byte = 0;

    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }

    for (uint8_t j = 0; j < h; j++, y++) {
        for (uint8_t i = 0; i < w; i++) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }

            if (byte & 0x80) {
                ssd1306_DrawPixel(x + i, y, color);
            }
        }
    }
    return;
}

void ssd1306_SetContrast(const uint8_t value) {
    const uint8_t kSetContrastControlRegister = 0x81;
    ssd1306_WriteCommand(kSetContrastControlRegister);
    ssd1306_WriteCommand(value);
}

void ssd1306_SetDisplayOn(const uint8_t on) {
    uint8_t value;
    if (on) {
        value = 0xAF;   // Display on
        SSD1306.DisplayOn = 1;
    } else {
        value = 0xAE;   // Display off
        SSD1306.DisplayOn = 0;
    }
    ssd1306_WriteCommand(value);
}

uint8_t ssd1306_GetDisplayOn() {
    return SSD1306.DisplayOn;
}


static inline void swap_int16(int16_t *a, int16_t *b) {
    int16_t temp = *a;
    *a = *b;
    *b = temp;
}

void drawFilledTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SSD1306_COLOR color) {
    if (y0 > y1) {
        swap_int16(&x0, &x1);
        swap_int16(&y0, &y1);
    }
    if (y1 > y2) {
        swap_int16(&x1, &x2);
        swap_int16(&y1, &y2);
    }
    if (y0 > y1) {
        swap_int16(&x0, &x1);
        swap_int16(&y0, &y1);
    }
    int16_t dx01 = x1 - x0, dy01 = y1 - y0;
    int16_t dx02 = x2 - x0, dy02 = y2 - y0;
    int16_t dx12 = x2 - x1, dy12 = y2 - y1;
    float invSlope01 = (dy01 != 0) ? (float)dx01 / dy01 : 0;
    float invSlope02 = (dy02 != 0) ? (float)dx02 / dy02 : 0;
    float invSlope12 = (dy12 != 0) ? (float)dx12 / dy12 : 0;
    float curx1 = x0, curx2 = x0;
    for (int y = y0; y <= y1; y++) {
        int startx = (int)curx1;
        int endx   = (int)curx2;
        if (startx > endx) { int tmp = startx; startx = endx; endx = tmp; }
        for (int x = startx; x <= endx; x++) {
            ssd1306_DrawPixel((uint8_t)x, (uint8_t)y, color);
        }
        curx1 += invSlope01;
        curx2 += invSlope02;
    }
    curx1 = x1;
    curx2 = x0 + invSlope02 * (y1 - y0);
    for (int y = y1; y <= y2; y++) {
        int startx = (int)curx1;
        int endx   = (int)curx2;
        if (startx > endx) { int tmp = startx; startx = endx; endx = tmp; }
        for (int x = startx; x <= endx; x++) {
            ssd1306_DrawPixel((uint8_t)x, (uint8_t)y, color);
        }
        curx1 += invSlope12;
        curx2 += invSlope02;
    }
}


//Função de callback para a interrupção do botão
void button_callback(uint gpio, uint32_t events) {
    btn_pressed = true;
}

void delay_with_button_check(uint ms) {
    // Quebra o delay total em pequenos intervalos para verificar o botão
    const uint interval = 10; // 10 ms
    for(uint i = 0; i < ms; i += interval) {
        sleep_ms(interval);
        if(btn_pressed) {
            return; // Sai mais cedo se o botão for pressionado
        }
    }
}

void teste_display(void) 
{
    // Inicializa o pino do botão
    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);
    // Configura a interrupção para detectar borda de descida (quando o botão é pressionado)
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true, button_callback);

    btn_pressed = false;
    
    // Loop infinito para testar os recursos do display
    while(1) {
        if(btn_pressed) {  // Verificação rápida a cada início de iteração
            sleep_ms(200); // Debounce
            display_home();
            break;
        }

        // --------- Teste 1: Preenche a tela com Black e White ---------
        // Preencher com Black
        ssd1306_Fill(Black);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // Preencher com White
        ssd1306_Fill(White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 3: Desenha uma linha diagonal ---------
        ssd1306_Fill(Black);
        ssd1306_Line(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 4: Desenha um retângulo (contorno) ---------
        ssd1306_Fill(Black);
        ssd1306_DrawRectangle(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 5: Desenha um retângulo preenchido ---------
        ssd1306_Fill(Black);
        ssd1306_FillRectangle(10, 10, SSD1306_WIDTH - 10, SSD1306_HEIGHT - 10, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 6: Desenha um círculo (contorno) ---------
        ssd1306_Fill(Black);
        ssd1306_DrawCircle(SSD1306_WIDTH / 2, SSD1306_HEIGHT / 2, 20, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 7: Desenha um círculo preenchido ---------
        ssd1306_Fill(Black);
        ssd1306_FillCircle(SSD1306_WIDTH / 2, SSD1306_HEIGHT / 2, 20, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 8: Desenha um arco (180°) ---------
        ssd1306_Fill(Black);
        ssd1306_DrawArc(SSD1306_WIDTH / 2, SSD1306_HEIGHT / 2, 20, 0, 180, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 9: Desenha um arco com linhas de raio (270°) ---------
        ssd1306_Fill(Black);
        ssd1306_DrawArcWithRadiusLine(SSD1306_WIDTH / 2, SSD1306_HEIGHT / 2, 20, 0, 270, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 10: Desenha um triângulo preenchido ---------
        ssd1306_Fill(Black);
        // Desenha um triângulo com a base na parte inferior
        drawFilledTriangle(0, SSD1306_HEIGHT - 1, SSD1306_WIDTH / 2, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, White);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        // --------- Teste 11: Inverte um retângulo na tela ---------
        ssd1306_Fill(White);
        ssd1306_DrawRectangle(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, Black);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
        
        ssd1306_InvertRectangle(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1);
        ssd1306_UpdateScreen();
        delay_with_button_check(500);
        if(btn_pressed) {
            sleep_ms(200);
            display_home();
            break;
        }
    }
}

/**
 * Exibe uma animação de uma bola quicando na tela.
 * A animação roda até que o botão seja pressionado.
 */
void animation_display(void) 
{
    // Propriedades da bola
    int x = SSD1306_WIDTH / 2;
    int y = SSD1306_HEIGHT / 2;
    int dx = 1;
    int dy = 1;
    const int radius = 4;
    
    // Controle de tempo
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    uint32_t last_update = start_time;

    while (true) 
    {
        // Verifica se 2 segundos se passaram
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if((now - start_time) >= 3000) {
            break;
        }

        // Verificação do botão
        if (!gpio_get(JOYSTICK_BUTTON)) 
        {
            sleep_ms(200);
            break;
        }

        // Atualiza posição a cada 30ms
        if((now - last_update) > 30) 
        {
            x += dx;
            y += dy;

            // Colisão com bordas
            if((x - radius <= 0) || (x + radius >= SSD1306_WIDTH)) dx *= -1;
            if((y - radius <= 0) || (y + radius >= SSD1306_HEIGHT)) dy *= -1;

            last_update = now;
        }

        // Desenha o frame
        ssd1306_Fill(White);
        ssd1306_DrawRectangle(0, 0, SSD1306_WIDTH-1, SSD1306_HEIGHT-1, Black);
        ssd1306_FillCircle(x, y, radius, Black);
        ssd1306_UpdateScreen();
    }
    
    // Volta ao menu após 2 segundos ou botão pressionado
    display_home();
}


