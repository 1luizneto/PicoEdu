#include "inc/menu.h"
#include <string.h>
#include <stdio.h>

// Variáveis de seleção para o menu do joystick
// Cada variável representa a opção atualmente selecionada para cada demo.
static uint8_t posicao_selecao_joystick = JOYSTICK_RGB;
static uint8_t posicao_selecao_matriz   = JOYSTICK_RGB;
static uint8_t posicao_selecao_buzzer   = JOYSTICK_RGB;
static uint8_t posicao_selecao_mic      = JOYSTICK_RGB;
static uint8_t posicao_selecao_display  = JOYSTICK_RGB;
static uint8_t posicao_selecao_wifi     = JOYSTICK_RGB;

/*
 * Função: joystick_update_menu
 * ----------------------------
 * Atualiza a tela do display OLED para exibir o menu do joystick.
 * O menu mostra as opções "JoyStick POS", "JoyStick RGB" e "Menu Principal"
 * com um destaque (bitmap) posicionado de acordo com a seleção atual.
 */
static void joystick_update_menu(void) {
    uint8_t pos_y;
    // Determina a posição vertical do destaque com base na seleção atual
    switch(posicao_selecao_joystick) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    // Limpa o display e desenha as opções do menu
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("JoyStick POS", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("JoyStick RGB", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    // Desenha o destaque utilizando um bitmap na posição definida
    ssd1306_DrawBitmap(0, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: joystick_home
 * ----------------------
 * Exibe o menu do joystick e permite a navegação entre as opções usando o ADC.
 * A função lê o valor do ADC para determinar se o usuário deseja mover a seleção
 * para cima ou para baixo, e executa a ação correspondente ao pressionar do botão.
 */
void joystick_home(void) {
    // Seleciona o canal ADC para a leitura do joystick
    adc_select_input(0);
    bool can_move = true;
    joystick_update_menu();

    while(1) {
        uint32_t adc_val_y = adc_read();
        uint8_t prev = posicao_selecao_joystick;
        
        // Ajusta a seleção conforme o valor do ADC (movimento para cima ou para baixo)
        if(adc_val_y > 3800 && can_move) {
            posicao_selecao_joystick = (posicao_selecao_joystick == JOYSTICK_POS) ? MENU : posicao_selecao_joystick - 1;
            can_move = false;
        } else if(adc_val_y < 500 && can_move) {
            posicao_selecao_joystick = (posicao_selecao_joystick == MENU) ? JOYSTICK_POS : posicao_selecao_joystick + 1;
            can_move = false;
        } else if(adc_val_y >= 1500 && adc_val_y <= 2500) {
            can_move = true;
        }
        
        // Se a seleção mudar, atualiza o menu
        if(prev != posicao_selecao_joystick)
            joystick_update_menu();
        
        // Se o botão for pressionado, executa a ação correspondente à opção selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_joystick == MENU) {
                // Retorna ao menu principal
                home(1);
                adc_select_input(1);
                return;
            } else if(posicao_selecao_joystick == JOYSTICK_POS) {
                // Executa a função joystick_pos() para a opção "JoyStick POS"
                joystick_pos();
            } else if(posicao_selecao_joystick == JOYSTICK_RGB) {
                // Executa a função joystick_rgb() para a opção "JoyStick RGB"
                joystick_rgb();
            }
        }
        POLLING_TIME; 
    }
}

/*
 * Função: joystick_screen
 * ------------------------
 * Inicializa e exibe o menu do joystick.
 */
void joystick_screen(void) {
    joystick_update_menu();
    joystick_home();
}

/*-----------------------------------------------------------
 * Função do menu principal (home)
 *----------------------------------------------------------*/

/*
 * Função: updateHomeScreen
 * -------------------------
 * Atualiza a tela do menu principal com base na opção atual.
 * Mostra o título "MENU" e a opção selecionada centralizada em um retângulo.
 */
static void updateHomeScreen(uint8_t option) {
    char* messages[TOTAL_OPTIONS] = {
        "Demo Joystick",
        "Demo Matriz",
        "Demo Buzzer",
        "Demo Mic",
        "Demo Display",
        "Demo Wifi"
    };
    char* optionText = messages[option - 1];
    ssd1306_Fill(White);
    // Desenha o cabeçalho do menu
    ssd1306_FillRectangle(2, 2, 125, 12, Black);
    ssd1306_SetCursor(48, 3);
    ssd1306_WriteString("MENU", Font_7x10, White);
    // Desenha o retângulo que contém a opção atual
    ssd1306_DrawRectangle(3, 14, 125, 62, Black);
    
    // Centraliza o texto da opção atual
    uint8_t textWidth = strlen(optionText) * 7;
    uint8_t availableWidth = 123;
    uint8_t x = 2 + (availableWidth - textWidth) / 2;
    uint8_t y = 14 + (48 - 10) / 2;
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(optionText, Font_7x10, Black);
    
    // Desenha setas (triângulos) nas laterais
    drawFilledTriangle(4, 38, 12, 30, 12, 46, Black);
    drawFilledTriangle(123, 38, 115, 30, 115, 46, Black);
    ssd1306_UpdateScreen();
}

/*-----------------------------------------------------------
 * Funções básicas para cada opção do menu principal
 *----------------------------------------------------------*/

/*
 * Função: demo_joystick
 * ----------------------
 * Inicia o demo do joystick exibindo o menu correspondente.
 */
void demo_joystick(void) {
    joystick_screen();    
}

/*
 * Função: demo_matriz
 * --------------------
 * Inicia o demo da matriz, exibindo o menu e executando as funções associadas.
 */
void demo_matriz(void) {
    matriz_screen();    
}

/*
 * Função: matriz_update_menu
 * ---------------------------
 * Atualiza a tela do menu da matriz, exibindo as opções "Matriz RGB", "Matriz Jogo"
 * e "Menu Principal", com destaque conforme a seleção.
 */
static void matriz_update_menu(void) {
    uint8_t pos_y;
    switch(posicao_selecao_matriz) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("Matriz RGB", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("Matriz Jogo", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    ssd1306_DrawBitmap(1, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: matriz_home
 * --------------------
 * Gerencia o menu da matriz, permitindo a navegação via ADC e executa a função
 * correspondente à opção selecionada.
 */
void matriz_home(void) {
    adc_select_input(0);
    bool can_move = true;
    matriz_update_menu();
    
    while(1) {
        uint32_t adc_val_y = adc_read();
        uint8_t prev = posicao_selecao_matriz;
        
        // Atualiza a seleção conforme o valor do ADC
        if(adc_val_y > 3800 && can_move) {
            posicao_selecao_matriz = (posicao_selecao_matriz == MATRIZ_1) ? MENU : posicao_selecao_matriz - 1;
            can_move = false;
        } else if(adc_val_y < 500 && can_move) {
            posicao_selecao_matriz = (posicao_selecao_matriz == MENU) ? MATRIZ_1 : posicao_selecao_matriz + 1;
            can_move = false;
        } else if(adc_val_y >= 1500 && adc_val_y <= 2500) {
            can_move = true;
        }
        
        if(prev != posicao_selecao_matriz)
            matriz_update_menu();
        
        // Se o botão for pressionado, executa a ação da opção selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_matriz == MENU) {
                adc_select_input(1);
                home(2);
            } else if(posicao_selecao_matriz == MATRIZ_1) {
                // Executa o efeito da Matriz RGB
                Matriz_RGB();
            } else if(posicao_selecao_matriz == MATRIZ_2) {
                // Executa a função para o jogo na matriz
                matriz_jogo();
            }
        }
        POLLING_TIME; 
    }
}

/*
 * Função: matriz_screen
 * ----------------------
 * Inicializa e exibe o menu da matriz.
 */
void matriz_screen(void) {
    matriz_update_menu();
    matriz_home();
}

/*
 * Função: demo_buzzer
 * --------------------
 * Inicia o demo do buzzer, chamando a tela correspondente.
 */
void demo_buzzer(void) {
    buzzer_screen();
}

/*
 * Função: buzzer_update_menu
 * ---------------------------
 * Atualiza a tela do menu do buzzer, exibindo as opções "Asa Branca", "Mario Tema"
 * e "Menu Principal", com destaque de acordo com a seleção atual.
 */
static void buzzer_update_menu(void) {
    uint8_t pos_y;
    switch(posicao_selecao_buzzer) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("Asa Branca", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("Mario Tema", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    ssd1306_DrawBitmap(1, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: buzzer_home
 * --------------------
 * Gerencia o menu do buzzer, permitindo navegação e execução da ação
 * correspondente à opção selecionada.
 */
void buzzer_home(void) {
    adc_select_input(0);
    bool can_move = true;
    buzzer_update_menu();
    while(1) {
        uint32_t adc_val = adc_read();
        uint8_t prev = posicao_selecao_buzzer;
        // Ajusta a seleção conforme o valor do ADC
        if(adc_val > 3800 && can_move) {
            posicao_selecao_buzzer = (posicao_selecao_buzzer == JOYSTICK_POS) ? MENU : posicao_selecao_buzzer - 1;
            can_move = false;
        } else if(adc_val < 500 && can_move) {
            posicao_selecao_buzzer = (posicao_selecao_buzzer == MENU) ? JOYSTICK_POS : posicao_selecao_buzzer + 1;
            can_move = false;
        } else if(adc_val >= 1500 && adc_val <= 2500) {
            can_move = true;
        }
        if(prev != posicao_selecao_buzzer)
            buzzer_update_menu();

        // Se o botão for pressionado, executa a ação selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_buzzer == MENU) {
                home(3);
                adc_select_input(1);
                return;
            } else if(posicao_selecao_buzzer == BUZZER_1) {
                // Executa a música "Asa Branca"
                asaBranca_play();
            } else if(posicao_selecao_buzzer == BUZZER_2) {
                // Executa a música "Mario Tema"
                mario_play();
            }
        }
        POLLING_TIME;
    }
}

/*
 * Função: buzzer_screen
 * ----------------------
 * Inicializa e exibe o menu do buzzer.
 */
void buzzer_screen(void) {
    buzzer_update_menu();
    buzzer_home();
}

/*
 * Função: demo_mic
 * -----------------
 * Inicia o demo do microfone, chamando a tela correspondente.
 */
void demo_mic(void) {
    mic_screen();
}

/*
 * Função: mic_update_menu
 * ------------------------
 * Atualiza a tela do menu do microfone, exibindo as opções "Teste Mic",
 * "Mic Matriz" e "Menu Principal", com destaque na opção selecionada.
 */
static void mic_update_menu(void) {
    uint8_t pos_y;
    switch(posicao_selecao_mic) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("Teste Mic", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("Mic Matriz", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    ssd1306_DrawBitmap(1, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: mic_home
 * -----------------
 * Gerencia o menu do microfone, permitindo a navegação e a execução
 * da função selecionada (teste do mic ou visualização na matriz).
 */
void mic_home(void) {
    adc_select_input(0);
    bool can_move = true;
    mic_update_menu();
    while(1) {
        uint32_t adc_val = adc_read();
        uint8_t prev = posicao_selecao_mic;
        // Atualiza a seleção com base na leitura do ADC
        if(adc_val > 3800 && can_move) {
            posicao_selecao_mic = (posicao_selecao_mic == JOYSTICK_POS) ? MENU : posicao_selecao_mic - 1;
            can_move = false;
        } else if(adc_val < 500 && can_move) {
            posicao_selecao_mic = (posicao_selecao_mic == MENU) ? JOYSTICK_POS : posicao_selecao_mic + 1;
            can_move = false;
        } else if(adc_val >= 1500 && adc_val <= 2500) {
            can_move = true;
        }
        if(prev != posicao_selecao_mic)
            mic_update_menu();
        
        // Se o botão for pressionado, executa a ação selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_mic == MENU) {
                home(4);
                adc_select_input(1);
                return;
            } else if(posicao_selecao_mic == MIC_1) {
                // Executa o teste do microfone
                mic_test();
            } else if(posicao_selecao_mic == MIC_2) {
                // Executa a visualização do microfone na matriz de LEDs
                mic_matriz();
            }
        }
        POLLING_TIME;
    }
}

/*
 * Função: mic_screen
 * -------------------
 * Inicializa e exibe o menu do microfone.
 */
void mic_screen(void) {
    mic_update_menu();
    mic_home();
}

/*
 * Função: demo_display
 * --------------------
 * Inicia o demo do display, chamando a tela correspondente.
 */
void demo_display(void) {
    display_screen();
}

/*
 * Função: display_update_menu
 * ----------------------------
 * Atualiza a tela do menu do display, exibindo as opções "Teste Display",
 * "Animation Display" e "Menu Principal", com o destaque posicionado conforme a seleção.
 */
static void display_update_menu(void) {
    uint8_t pos_y;
    switch(posicao_selecao_display) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("Teste Display", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("Animation Display", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    ssd1306_DrawBitmap(1, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: display_home
 * ---------------------
 * Gerencia o menu do display, permitindo a navegação entre as opções
 * e executando a função correspondente à opção selecionada.
 */
void display_home(void) {
    adc_select_input(0);
    bool can_move = true;
    display_update_menu();
    while(1) {
        uint32_t adc_val = adc_read();
        uint8_t prev = posicao_selecao_display;
        // Atualiza a seleção conforme o valor do ADC
        if(adc_val > 3800 && can_move) {
            posicao_selecao_display = (posicao_selecao_display == JOYSTICK_POS) ? MENU : posicao_selecao_display - 1;
            can_move = false;
        } else if(adc_val < 500 && can_move) {
            posicao_selecao_display = (posicao_selecao_display == MENU) ? JOYSTICK_POS : posicao_selecao_display + 1;
            can_move = false;
        } else if(adc_val >= 1500 && adc_val <= 2500) {
            can_move = true;
        }
        if(prev != posicao_selecao_display)
            display_update_menu();

        // Se o botão for pressionado, executa a ação da opção selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_display == MENU) {
                home(5);
                adc_select_input(1);
                return;
            } else if(posicao_selecao_display == DISPLAY_1) {
                // Executa o teste do display
                teste_display();
            } else if(posicao_selecao_display == DISPLAY_2) {
                // Executa a animação no display
                animation_display();
            }
        }
        POLLING_TIME; 
    }
}

/*
 * Função: display_screen
 * -----------------------
 * Inicializa e exibe o menu do display.
 */
void display_screen(void) {
    display_update_menu();
    display_home();
}

/*
 * Função: demo_wifi
 * -----------------
 * Inicia o demo do wifi, chamando a tela correspondente.
 */
void demo_wifi(void) {
    wifi_screen();
}

/*
 * Função: wifi_update_menu
 * -------------------------
 * Atualiza a tela do menu do wifi, exibindo as opções "Wifi Status",
 * "WebServer Status" e "Menu Principal", com destaque conforme a seleção.
 */
static void wifi_update_menu(void) {
    uint8_t pos_y;
    switch(posicao_selecao_wifi) {
        case JOYSTICK_POS:
            pos_y = 1;
            break;
        case JOYSTICK_RGB:
            pos_y = 23;
            break;
        case MENU:
            pos_y = 45;
            break;
        default:
            pos_y = 23;
    }
    ssd1306_Fill(White);
    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("Wifi Status", Font_7x10, Black);
    ssd1306_SetCursor(5, 27);
    ssd1306_WriteString("WebServer Status", Font_7x10, Black);
    ssd1306_SetCursor(5, 49);
    ssd1306_WriteString("Menu Principal", Font_7x10, Black);
    ssd1306_DrawBitmap(1, pos_y, bitmap_item_sel_outline, 128, 19, Black);
    ssd1306_UpdateScreen();
}

/*
 * Função: wifi_home
 * ------------------
 * Gerencia o menu do wifi, permitindo a navegação via ADC e executando a ação
 * correspondente à opção selecionada.
 */
void wifi_home(void) {
    adc_select_input(0);
    bool can_move = true;
    wifi_update_menu();
    while(1) {
        uint32_t adc_val = adc_read();
        uint8_t prev = posicao_selecao_wifi;
        // Ajusta a seleção conforme o valor do ADC
        if(adc_val > 3800 && can_move) {
            posicao_selecao_wifi = (posicao_selecao_wifi == JOYSTICK_POS) ? MENU : posicao_selecao_wifi - 1;
            can_move = false;
        } else if(adc_val < 500 && can_move) {
            posicao_selecao_wifi = (posicao_selecao_wifi == MENU) ? JOYSTICK_POS : posicao_selecao_wifi + 1;
            can_move = false;
        } else if(adc_val >= 1500 && adc_val <= 2500) {
            can_move = true;
        }
        if(prev != posicao_selecao_wifi)
            wifi_update_menu();

        // Se o botão for pressionado, executa a ação da opção selecionada
        if(gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            if(posicao_selecao_wifi == MENU) {
                home(6);
                adc_select_input(1);
                return;
            } else if(posicao_selecao_wifi == WIFI_1) {
                // Exibe o status do wifi
                WIFI_status();
            } else if(posicao_selecao_wifi == WIFI_2) {
                // Exibe o status do webserver
                webserver_status();
            }
        }
        POLLING_TIME; 
    }
}

/*
 * Função: wifi_screen
 * --------------------
 * Inicializa e exibe o menu do wifi.
 */
void wifi_screen(void) {
    wifi_update_menu();
    wifi_home();
}


/*
 * Função: home
 * ------------
 * Exibe o menu principal e permite navegar entre as 6 opções de demos.
 * O usuário pode navegar pelas opções usando o valor lido do ADC e pressionar
 * o botão para selecionar uma opção, que chama a função correspondente.
 */
void home(uint8_t option) {
    updateHomeScreen(option);
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(1);
    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);
    uint8_t currentOption = option;
    uint16_t adc_val_x;
    
    while(1) 
    {
        adc_val_x = adc_read();
        // Verifica o movimento para a esquerda (valor baixo) para selecionar a opção anterior
        if (adc_val_x < JOYSTICK_LEFT_THRESHOLD) {
            if (currentOption == 1)
                currentOption = TOTAL_OPTIONS;
            else
                currentOption--;
            updateHomeScreen(currentOption);
            while (adc_read() < JOYSTICK_LEFT_THRESHOLD)
                sleep_ms(10);
            DEBOUNCE;
        }
        // Verifica o movimento para a direita (valor alto) para selecionar a próxima opção
        else if (adc_val_x > JOYSTICK_RIGHT_THRESHOLD) {
            currentOption++;
            if (currentOption > TOTAL_OPTIONS)
                currentOption = 1;
            updateHomeScreen(currentOption);
            while (adc_read() > JOYSTICK_RIGHT_THRESHOLD)
                sleep_ms(10);
            DEBOUNCE;
        }
        
        // Ao pressionar o botão, chama a função correspondente à opção selecionada
        if (gpio_get(JOYSTICK_BUTTON) == 0) {
            DEBOUNCE;
            switch(currentOption) {
                case 1:
                    demo_joystick();
                    break;
                case 2:
                    demo_matriz();
                    break;
                case 3:
                    demo_buzzer();
                    break;
                case 4:
                    demo_mic();
                    break;
                case 5:
                    demo_display();
                    break;
                case 6:
                    demo_wifi();
                    break;
                default:
                    break;
            }
            // Após retornar do demo, atualiza novamente a tela do menu principal
            updateHomeScreen(currentOption);
        }
        sleep_ms(50);
    }
}

// Variável global para armazenar o canal atual do ADC
static uint8_t current_adc_channel = 1;

/*
 * Função: set_adc_channel
 * ------------------------
 * Wrapper para selecionar o canal do ADC e atualizar a variável global.
 */
void set_adc_channel(uint8_t channel) {
    current_adc_channel = channel;
    adc_select_input(channel);
}

/*
 * Função: read_temperature_sensor
 * --------------------------------
 * Realiza a leitura do sensor de temperatura.
 * Salva o canal atual do ADC, seleciona o canal do sensor de temperatura,
 * aguarda a estabilização, lê o valor, desabilita o sensor e restaura o canal original.
 * Realiza o cálculo para converter o valor lido em temperatura (°C).
 */
float read_temperature_sensor(void) {
    // Salva o canal atual
    uint8_t saved_channel = current_adc_channel;
    
    // Seleciona o canal do sensor de temperatura e aguarda estabilização
    set_adc_channel(4);
    sleep_ms(10);
    adc_set_temp_sensor_enabled(true);
    uint32_t raw_value = adc_read();
    adc_set_temp_sensor_enabled(false);
    
    // Restaura o canal original
    set_adc_channel(saved_channel);
    
    // Converte o valor bruto em tensão e, posteriormente, em temperatura (°C)
    float conversion = raw_value * 3.3f / (1 << 12);
    float temperature = 27.0f - ((conversion - 0.706f) / 0.001721f);
    
    return temperature;
}
