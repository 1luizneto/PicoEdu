#ifndef WIFI_H
#define WIFI_H

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "inc/menu.h"

//#define WIFI_SSID ""  // Substitua pelo nome da sua rede Wi-Fi
//#define WIFI_PASS "" // Substitua pela senha da sua rede Wi-Fi

// Variáveis globais
extern char button1_message[50];
extern char button2_message[50];
extern char http_response[8192];

// Protótipos das funções
void create_http_response(void);
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
static void start_http_server(void);
void create_http_request(void);
void send_http_request(const void *data, u16_t len);
static err_t http_client_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
void monitor_buttons(void);
void WIFI_Init(void);
void WIFI_status(void);
void webserver_status(void);

#endif 