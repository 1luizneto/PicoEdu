#include "inc/wifi.h"

// Buffer para resposta HTTP
char http_response[8192];

//Armazena o SSID da rede WI-FI conectada
char wifi_ssid[64] = "";

// Variáveis globais adicionais (adicionar no início do arquivo)
uint32_t last_cloud_update = 0;

// Timer para requisições periódicas
static repeating_timer_t request_timer;
volatile bool request_pending = false; // Flag atômico

/**
 * @brief Cria a resposta HTTP para a página principal.
 * 
 * Esta função gera o HTML da página inicial do servidor HTTP, que contém opções
 * para interagir com diferentes componentes da placa BitDogLab.
 */
void create_http_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>PicoEdu: Aprendizado Dinâmico</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    p {"
        "      font-size: 1.2em;"
        "      color: #666;"
        "      margin-bottom: 30px;"
        "    }"
        "    .btn {"
        "      display: inline-block;"
        "      margin: 10px;"
        "      padding: 15px 25px;"
        "      font-size: 1em;"
        "      font-weight: 500;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border: none;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    .btn:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>PicoEdu: Aprendizado Dinâmico</h1>"
        "    <p>Explore as opções abaixo para iniciar seu aprendizado com a placa BitDogLab.</p>"
        "    <a class=\"btn\" href=\"/option/joystick\">Joystick</a>"
        "    <a class=\"btn\" href=\"/option/matriz\">Matriz</a>"
        "    <a class=\"btn\" href=\"/option/buzzer\">Buzzer</a>"
        "    <a class=\"btn\" href=\"/option/mic\">Mic</a>"
        "    <a class=\"btn\" href=\"/option/display\">Display</a>"
        "    <a class=\"btn\" href=\"/option/wifi\">Wifi</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}



/**
 * @brief Cria a resposta HTTP para a página do Joystick.
 * 
 * Esta função gera o HTML da página que explica o funcionamento do joystick
 * e como ele é integrado à placa BitDogLab.
 */
void create_joystick_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Joystick - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    a {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    a:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Joystick</h1>"
        "    <p> O joystick converte a posição da alavanca em sinais elétricos. No caso do modelo da BitDogLab, trata-se de um joystick analógico, no qual as posições nos eixos X e Y são convertidas em dois sinais de tensão que variam de 0 a 3,3V.</p>"
        "    <p>Quando a alavanca está na posição neutra, os valores dessas tensões são aproximadamente iguais à metade da tensão de alimentação, ou seja, Vx = Vy = VCC/2. Ao movimentar a alavanca, esses valores variam proporcionalmente à posição do joystick.</p>"
        "    <p>Os sinais analógicos gerados são lidos pelos conversores Analógico-Digitais (ADCs) do microcontrolador RP2040, que estão disponíveis nos pinos GPIO 26 e GPIO 27. Esses conversores transformam os valores analógicos em dados digitais, permitindo que o microcontrolador processe as informações.</p>"
        "    <p>Além disso, o joystick possui um botão integrado, que é acionado ao pressionar a alavanca para baixo. Esse botão está conectado ao GPIO 22 do RP2040 e deve ser configurado como entrada digital com pull-up. Em repouso, ele permanece em nível lógico alto e, ao ser pressionado, muda para nível lógico baixo.</p>"
        "    <p>Para exibir os valores lidos pelo joystick, utilizaremos o próprio terminal do VS Code como interface de saída. No terminal, serão apresentados os valores numéricos dos sinais analógicos e uma barra gráfica que se movimentará de forma proporcional à posição do joystick, facilitando a visualização do funcionamento do sensor.</p>"
        "    <a href=\"/\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}

/**
 * @brief Cria a resposta HTTP para a página da Matriz de LEDs.
 * 
 * Esta função gera o HTML da página que explica o funcionamento da matriz de LEDs
 * e como ela é controlada pelo microcontrolador RP2040.
 */
void create_matriz_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Matriz - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    h2 {"
        "      font-size: 1.8em;"
        "      color: #333;"
        "      margin: 20px 0 10px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    a {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    a:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Matriz</h1>"
        "    <p>Para controlar um LED RGB, são necessários três sinais individuais: um para cada cor (vermelho, verde e azul). Agora, imagine aplicar esse método a uma matriz com 25 LEDs RGB, organizados em 5 colunas por 5 linhas. Seriam necessários 75 sinais de controle (3 x 25), tornando inviável o uso direto de um microcontrolador convencional. Felizmente, os LEDs endereçáveis, como os WS2812, resolvem esse problema. Embora também sejam RGB, eles podem ser controlados usando apenas um único pino de dados digital. Os LEDs podem ser conectados em cadeia, onde a saída DOUT de um LED se conecta à entrada DIN do próximo. Dessa forma, um único pino do microcontrolador controla todos os LEDs, ajustando individualmente sua cor e intensidade.</p>"
        "    <h2>Desafios no Controle dos LEDs</h2>"
        "    <p>Embora essa tecnologia simplifique a conexão elétrica, o controle dos LEDs exige um timing extremamente preciso, pois o protocolo WS2812 opera com variações de tempo na ordem de nanosegundos.</p>"
        "    <h2>Uso do PIO no RP2040 para Controle dos LEDs</h2>"
        "    <p>No RP2040, podemos utilizar o PIO (Programmable Input/Output) para garantir que os sinais enviados aos LEDs sejam gerados com precisão, sem sobrecarregar o processador. O PIO funciona como uma máquina de estado programável capaz de operar de forma independente, permitindo a geração precisa dos sinais exigidos pelo WS2812, redução do consumo de processamento e execução de outras tarefas simultaneamente. A função npWrite utiliza o PIO para enviar os dados armazenados no buffer da matriz de LEDs para o hardware, transmitindo as cores previamente definidas enquanto o PIO cuida do envio correto dos sinais, garantindo a sincronização necessária. Essa abordagem torna o sistema mais eficiente, permitindo animações fluidas e controle preciso dos LEDs sem impactar o desempenho do microcontrolador.</p>"
        "    <a href=\"/\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}


/**
 * @brief Cria a resposta HTTP para a página do Buzzer.
 * 
 * Esta função gera o HTML da página que explica o funcionamento do buzzer
 * e como ele é controlado pelo microcontrolador RP2040.
 */
void create_buzzer_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Buzzer - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    h2 {"
        "      font-size: 1.8em;"
        "      color: #333;"
        "      margin: 20px 0 10px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    a {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    a:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Buzzer</h1>"
        "    <p><strong>Funcionamento de um Buzzer Passivo:</strong></p>"
        "    <p>Um buzzer passivo funciona de maneira semelhante a um alto-falante básico, utilizando uma bobina eletromagnética e uma membrana vibratória para produzir som.</p>"
        "    <p>Quando um sinal elétrico variável é aplicado à bobina, ele gera um campo magnético que interage com um ímã fixo dentro do buzzer. Essa interação faz com que a bobina e a membrana se movimentem, criando vibrações que deslocam o ar ao redor e geram ondas sonoras, as quais podem ser percebidas pelo ouvido humano.</p>"
        "    <h2>Como a Música é Produzida com um Buzzer?</h2>"
        "    <p>O som é uma onda mecânica que se propaga através de meios como o ar, água ou sólidos. Essas ondas são geradas por vibrações que movimentam as partículas do meio. Os principais parâmetros que determinam as características do som são: <strong>Frequência</strong> (medida em Hertz - Hz), que define se o som é mais agudo ou mais grave, e <strong>Amplitude</strong>, que determina a intensidade (volume) do som.</p>"
        "    <h2>Controle do Buzzer Passivo</h2>"
        "    <p>Para gerar sons com um buzzer passivo, é necessário fornecer um sinal elétrico variável, pois ele não possui um oscilador interno. A técnica mais comum para isso é a Modulação por Largura de Pulso (PWM), que permite controlar a frequência do som gerado. Dessa forma, é possível criar desde simples bipes até melodias mais complexas. Compreender os princípios matemáticos do som é essencial para utilizar o buzzer de maneira eficiente, possibilitando o controle preciso tanto da frequência quanto da amplitude do sinal.</p>"
        "    <a href=\"/\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}


/**
 * @brief Cria a resposta HTTP para a página do Microfone.
 * 
 * Esta função gera o HTML da página que explica o funcionamento do microfone
 * e como ele é controlado pelo microcontrolador RP2040.
 */
void create_microfone_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Microfone - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    h2 {"
        "      font-size: 1.8em;"
        "      color: #333;"
        "      margin: 20px 0 10px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    a {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    a:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Microfone</h1>"
        "    <p>Neste estudo, vamos aprender a ler sinais analógicos e processá-los com alta taxa de amostragem utilizando o recurso de DMA (Direct Memory Access), que permite a transferência de dados do Conversor Analógico-Digital (ADC) para a memória sem intervenção direta da CPU, otimizando o desempenho do sistema.</p>"
        "    <h2>Características do Sinal de Saída do Microfone</h2>"
        "    <p>O microfone presente na placa gera um sinal analógico cuja tensão varia conforme o som captado:<br>"
        "       <strong>Offset:</strong> Quando não há som, a saída do microfone é 1,65V, correspondente ao centro da faixa do ADC. Esse valor pode ser ajustado com um trimpot.<br>"
        "       <strong>Amplitude Máxima:</strong> O sinal pode oscilar até ±1,65V em relação ao offset, atingindo valores entre 0V e 3,3V, dependendo da intensidade do som.<br>"
        "       <strong>Faixa Total do Sinal:</strong> O sinal analógico gerado varia de 0V a 3,3V, utilizando toda a faixa dinâmica do ADC, garantindo a melhor resolução e qualidade da leitura.</p>"
        "    <h2>Características do Conversor Analógico-Digital (ADC) do RP2040</h2>"
        "    <p>O ADC do RP2040, presente no Raspberry Pi Pico, possui as seguintes especificações:<br>"
        "       <strong>Faixa de Medição (Range):</strong> Mede tensões entre 0V e VREF, onde VREF é fixado internamente em 3,3V.<br>"
        "       <strong>Resolução do ADC:</strong> A conversão é realizada com 12 bits de resolução, gerando valores entre 0 (0V) e 4095 (3,3V).<br>"
        "       <strong>Conversão para Formato Signed:</strong> Para facilitar o processamento, os valores brutos (0 a 4095) são convertidos para um intervalo centrado em 0 usando a fórmula:<br>"
        "       Valor_signed = Valor_bruto_ADC - 2048<br>"
        "       Assim, a saída do microfone (1,65V) corresponde ao valor 0 após a conversão.</p>"
        "    <a href=\"/\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}


/**
 * @brief Cria a resposta HTTP para a página do Display.
 * 
 * Esta função gera o HTML da página que explica o funcionamento do Display
 * e como ele é controlado pelo microcontrolador RP2040.
 */
void create_display_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Display - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0,0,0,0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    h2 {"
        "      font-size: 1.8em;"
        "      color: #333;"
        "      margin: 20px 0 10px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    a {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    a:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Display</h1>"
        "    <p>Aqui você aprenderá como o Display funciona.</p>"
        "    <p>O display OLED SSD1306 é um dispositivo de exibição digital que utiliza a tecnologia OLED (Organic Light-Emitting Diode) para apresentar informações visuais com alto contraste e baixo consumo de energia.</p>"
        "    <p><strong>Características e Funcionamento:</strong><br>"
        "       <strong>Tecnologia OLED:</strong> Ao contrário dos displays LCD, que dependem de uma fonte de luz de fundo, os diodos orgânicos emissores de luz geram sua própria iluminação, proporcionando imagens mais nítidas, com pretos mais profundos e maior eficiência energética.<br>"
        "       <strong>Controlador SSD1306:</strong> Este controlador integrado gerencia o funcionamento do display, convertendo os dados enviados pelo microcontrolador em sinais elétricos que acendem os pixels correspondentes. Ele possibilita a exibição de textos, gráficos e imagens em resoluções comuns de 128×64 pixels.<br>"
        "       <strong>Interfaces de Comunicação:</strong> O SSD1306 pode ser controlado por meio de interfaces I2C ou SPI, sendo que a interface I2C utiliza apenas dois pinos, facilitando a integração com diversos microcontroladores, como o Arduino e o Raspberry Pi Pico.</p>"
        "    <h2>Aplicações e Utilização</h2>"
        "    <p>Para utilizar o display OLED SSD1306, é necessário estabelecer uma comunicação entre o BitDog Lab e o display. Bibliotecas específicas facilitam o processo de programação, permitindo que você:<br>"
        "       - Envie comandos para limpar ou atualizar a tela.<br>"
        "       - Exiba textos e gráficos de forma dinâmica.<br>"
        "       - Controle a intensidade dos pixels, garantindo uma visualização ideal em diferentes condições de iluminação.</p>"
        "    <h2>Vantagens do SSD1306</h2>"
        "    <p>Entre as principais vantagens do SSD1306, destacam-se:<br>"
        "       <strong>Alto Contraste:</strong> Garante excelente legibilidade, mesmo em ambientes com muita luz.<br>"
        "       <strong>Baixo Consumo de Energia:</strong> Ideal para dispositivos portáteis e aplicações com restrição de energia.<br>"
        "       <strong>Versatilidade:</strong> Pode ser utilizado em uma ampla gama de projetos, desde sistemas embarcados simples até interfaces gráficas mais complexas.</p>"
        "    <a href=\"/\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}

/**
 * @brief Cria a resposta HTTP para a página do WIFI.
 * 
 * Esta função gera o HTML da página que explica o funcionamento do WIFI
 * e como ele é controlado pelo microcontrolador RP2040.
 */
void create_wifi_response(void) {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt\">"
        "<head>"
        "  <meta charset=\"UTF-8\">"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "  <title>Wifi - PicoEdu</title>"
        "  <link href=\"https://fonts.googleapis.com/css?family=Roboto:300,400,500&display=swap\" rel=\"stylesheet\">"
        "  <style>"
        "    body {"
        "      margin: 0;"
        "      padding: 0;"
        "      font-family: 'Roboto', sans-serif;"
        "      background: linear-gradient(135deg, #74ebd5, #ACB6E5);"
        "      min-height: 100vh;"
        "      display: flex;"
        "      align-items: center;"
        "      justify-content: center;"
        "    }"
        "    .container {"
        "      width: 90%%;"
        "      max-width: 800px;"
        "      background: #fff;"
        "      border-radius: 12px;"
        "      padding: 40px;"
        "      box-shadow: 0 8px 16px rgba(0,0,0,0.2);"
        "      text-align: center;"
        "    }"
        "    h1 {"
        "      font-size: 2.5em;"
        "      color: #333;"
        "      margin-bottom: 20px;"
        "    }"
        "    p {"
        "      font-size: 1.1em;"
        "      color: #555;"
        "      margin: 15px 0;"
        "      text-align: justify;"
        "      line-height: 1.6;"
        "    }"
        "    .text-link {"
        "      color: #1a73e8;"
        "      text-decoration: none;"
        "      font-weight: bold;"
        "    }"
        "    .text-link:hover {"
        "      text-decoration: underline;"
        "    }"
        "    .button {"
        "      display: inline-block;"
        "      margin-top: 20px;"
        "      padding: 12px 20px;"
        "      font-size: 1em;"
        "      text-decoration: none;"
        "      color: #fff;"
        "      background-color: #5c6bc0;"
        "      border-radius: 8px;"
        "      transition: background-color 0.3s ease, transform 0.3s ease;"
        "    }"
        "    .button:hover {"
        "      background-color: #3f51b5;"
        "      transform: translateY(-3px);"
        "    }"
        "  </style>"
        "</head>"
        "<body>"
        "  <div class=\"container\">"
        "    <h1>Wifi</h1>"
        "    <p>Aqui você aprenderá como o wifi funciona.</p>"
        "    <p>A Raspberry Pi Pico W possui suporte à conectividade Wi-Fi, permitindo a implementação de funcionalidades avançadas, como a criação de servidores HTTP. Utilizando a linguagem C e o SDK oficial da Raspberry Pi, é possível desenvolver aplicações que interagem diretamente com dispositivos como smartphones e computadores por meio de redes Wi-Fi. Na placa que você tem em mãos, a Pico W está conectada a uma rede Wi-Fi e configurada como um servidor HTTP básico. Esse servidor possibilita que esse site que você está vendo exista. Além disso, configurando a BitDog Lab como cliente HTTP, é possível a troca de informações entre a Pico W e outros dispositivos, viabilizando aplicações como controle remoto, monitoramento de sensores e automação. Com essa abordagem, a Pico W pode atuar como um ponto de acesso para receber comandos e exibir informações, tornando-se uma ferramenta versátil para diversos projetos conectados.</p>"
        "    <p>Nesse Link: <a class=\"text-link\" href=\"https://thingspeak.mathworks.com/channels/2838406\" target=\"_blank\">ThingSpeak</a> temos um exemplo de uma aplicação com a nuvem onde um código simples manda a temperatura para um banco de dados na nuvem, onde a partir disso diversas aplicações podem ser feitas.</p>"
        "    <a href=\"/\" class=\"button\">Voltar ao menu</a>"
        "  </div>"
        "</body>"
        "</html>\r\n");
}


/**
 * @brief Função de callback para processar requisições HTTP.
 * 
 * Esta função é chamada quando uma requisição HTTP é recebida pelo servidor.
 * Ela verifica o tipo de requisição (com base na URL) e gera a resposta apropriada
 * para cada caso, como a página do joystick, matriz de LEDs, buzzer, etc.
 * 
 * @param arg Argumento genérico (não utilizado).
 * @param tpcb Estrutura que representa a conexão TCP.
 * @param p Buffer contendo os dados da requisição HTTP.
 * @param err Código de erro (se houver).
 * @return err_t Retorna ERR_OK se a requisição foi processada com sucesso.
 */
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) 
{
    if (p == NULL) {
        // Cliente fechou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a requisição HTTP
    char *request = (char *)p->payload;

    if(strstr(request, "GET /option/joystick") != NULL)
    {
        create_joystick_response();
    } 
    else if (strstr(request, "GET /option/matriz") != NULL) {
        create_matriz_response();
    } else if (strstr(request, "GET /option/buzzer") != NULL) {
        create_buzzer_response();
    } else if (strstr(request, "GET /option/mic") != NULL) {
        create_microfone_response();
    } else if (strstr(request, "GET /option/display") != NULL) {
        create_display_response();
    } else if (strstr(request, "GET /option/wifi") != NULL) {
        create_wifi_response();
    } else {
        // Requisição padrão: mostra o menu principal
        create_http_response();
    }
     
    // Envia a resposta HTTP
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

/**
 * @brief Callback de conexão: associa o http_callback à conexão.
 * 
 * Esta função é chamada quando uma nova conexão TCP é estabelecida.
 * Ela associa a função http_callback à conexão para processar as requisições HTTP.
 * 
 * @param arg Argumento genérico (não utilizado).
 * @param newpcb Estrutura que representa a nova conexão TCP.
 * @param err Código de erro (se houver).
 * @return err_t Retorna ERR_OK se a conexão foi configurada com sucesso.
 */
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) 
{
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

/**
 * @brief Inicializa o servidor HTTP na porta 80.
 * 
 * Esta função configura o servidor TCP para escutar na porta 80 e associa
 * a função connection_callback para lidar com novas conexões.
 */
static void start_http_server(void) 
{
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    // Liga o servidor na porta 80
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
    tcp_accept(pcb, connection_callback);  // Associa o callback de conexão

    printf("Servidor HTTP rodando na porta 80...\n");
}

// Função para criar a solicitação HTTP POST com dados JSON
void create_http_request(void) {
    char json_data[256]; // Buffer para o JSON
    char http_request[1024]; // Buffer para a requisição HTTP
    
    float temperature = read_temperature_sensor();
    last_cloud_update = temperature;

    snprintf(http_request, sizeof(http_request),
             "GET /update?api_key=WBPKI1T0OOKAI89Q&field1=%.2f HTTP/1.1\r\n"
             "Host: api.thingspeak.com\r\n"
             "Connection: close\r\n\r\n", temperature);
    send_http_request(http_request, strlen(http_request));
}

char cloud_server_ip[16] = "52.22.52.109";

/**
 * @brief Envia uma requisição HTTP para um servidor remoto.
 * 
 * Esta função inicia uma conexão TCP com o servidor remoto e envia
 * a requisição HTTP criada pela função create_http_request.
 * 
 * @param data Dados da requisição HTTP.
 * @param len Tamanho dos dados da requisição.
 */
void send_http_request(const void *data, u16_t len) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        //return;
    }

    ip_addr_t server_ip;
    err_t err = dns_gethostbyname("api.thingspeak.com", &server_ip, dns_callback, (void *)data);
    if (err == ERR_OK) {
        // O endereço IP foi resolvido imediatamente
        dns_callback("api.thingspeak.com", &server_ip, (void *)data);
    } else if (err == ERR_INPROGRESS) {
        // A resolução do DNS está em andamento, o callback será chamado quando terminar
        printf("Resolução do DNS em andamento...\n");
    } else {
        printf("Erro ao iniciar a resolução do DNS\n");
        return;
    }

    if (tcp_connect(pcb, &server_ip, 80, NULL) != ERR_OK) {
        printf("Erro ao conectar ao servidor\n");
        //return;
    }

    // Associa o callback de recepção
    tcp_recv(pcb, http_client_callback);

    // Cria e envia a solicitação HTTP
    tcp_write(pcb, data, len, TCP_WRITE_FLAG_COPY);
}

/**
 * @brief Callback para processar respostas HTTP recebidas do servidor remoto.
 * 
 * Esta função é chamada quando uma resposta HTTP é recebida do servidor remoto.
 * Ela exibe a resposta no console e fecha a conexão.
 * 
 * @param arg Argumento genérico (não utilizado).
 * @param tpcb Estrutura que representa a conexão TCP.
 * @param p Buffer contendo os dados da resposta HTTP.
 * @param err Código de erro (se houver).
 * @return err_t Retorna ERR_OK se a resposta foi processada com sucesso.
 */
static err_t http_client_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Conexão fechada pelo servidor
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a resposta HTTP
    char *response = (char *)p->payload;
    
    printf("Resposta HTTP:\n%s\n", response);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

/**
 * @brief Callback para processar a resolução de DNS.
 * 
 * Esta função é chamada quando o nome de domínio é resolvido para um endereço IP.
 * Ela inicia uma conexão TCP com o servidor remoto e envia a requisição HTTP.
 * 
 * @param name Nome de domínio resolvido.
 * @param ipaddr Endereço IP resolvido.
 * @param callback_arg Dados da requisição HTTP a serem enviados.
 */
static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr == NULL) {
        printf("Erro ao resolver o nome de domínio: %s\n", name);
        return;
    }

    printf("Nome de domínio resolvido: %s -> %s\n", name, ipaddr_ntoa(ipaddr));

    // Conecte ao servidor usando o endereço IP resolvido
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    if (tcp_connect(pcb, ipaddr, 80, NULL) != ERR_OK) {
        printf("Erro ao conectar ao servidor\n");
        return;
    }

    // Associa o callback de recepção
    tcp_recv(pcb, http_client_callback);

    // Cria e envia a solicitação HTTP
    const char *data = (const char *)callback_arg;
    tcp_write(pcb, data, strlen(data), TCP_WRITE_FLAG_COPY);
}

bool request_timer_callback(repeating_timer_t *rt) {
    request_pending = true; // Marca que há uma requisição pendente
    return true; // Mantém o timer ativo
}

/**
 * @brief Inicializa o Wi-Fi e o servidor HTTP.
 * 
 * Esta função configura a conexão Wi-Fi, inicia o servidor HTTP e entra em um loop
 * principal que envia requisições periódicas para o ThingSpeak.
 */
void WIFI_Init() 
{
    stdio_init_all();  // Inicializa a saída padrão
    sleep_ms(10000);   // Tempo de espera para abrir o terminal

    char ssid[64];
    char password[64];

    printf("Digite o SSID da rede Wi-Fi: ");
    scanf("%63[^\r\n]", ssid);
    printf(" %s\n", ssid);
    printf("\nDigite a senha da rede Wi-Fi: ");
    scanf("%63s", password);
    printf("%s\n", password); 
    sleep_ms(1000);     

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        // Poderia retornar ou reiniciar, se necessário
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    // Loop para tentar a conexão enquanto ela falhar
    while (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000)) 
    {
        printf("Falha ao conectar ao Wi-Fi. Tente novamente.\n");
        // Solicita novamente os dados do usuário
        printf("SSID da rede Wi-Fi: ");
        printf(" %s\n", ssid);
        printf("\nDigite a senha da rede Wi-Fi: ");
        scanf("%63s", password);
        printf("%s\n", password); 
        sleep_ms(1000);
    }

    // Se sair do loop, a conexão foi bem-sucedida
    printf("Connected.\n");
    printf("\nIniciando servidor HTTP\n\n");
    uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    printf("Wi-Fi conectado!\n");
    strcpy(wifi_ssid, ssid);

    // Inicia o servidor HTTP
    start_http_server();

    // Inicializa o timer para requisições periódicas (10 segundos)
    add_repeating_timer_ms(
        -10000,                 // Intervalo em ms (negativo para delay relativo)
        request_timer_callback, // Função de callback
        NULL,                   // Argumento para o callback
        &request_timer          // Estrutura do timer
    );

    // Loop principal
    while (1) 
    {
        cyw43_arch_poll();  // Necessário para manter o Wi-Fi ativo

        // Verifica se há requisição pendente
        if(request_pending) 
        {
            create_http_request();
            request_pending = false; // Reseta o flag
        }

        sleep_ms(100);     
    }

    cyw43_arch_deinit();
}


/**
 * @brief Exibe o status da conexão Wi-Fi no display OLED.
 * 
 * Esta função exibe informações como endereço IP, intensidade do sinal (RSSI),
 * SSID e endereço MAC no display OLED. Ela também verifica se o botão no pino 22
 * foi pressionado para retornar ao menu principal.
 */
void WIFI_status(void)
{
    char status[64];

    while(1)
    {
        // Limpa o display (fundo branco)
        ssd1306_Fill(White);

        // Verifica se a interface de rede está ativa
        if(netif_is_up(&cyw43_state.netif[0])) {
            uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
            // Obtém o valor do RSSI (intensidade do sinal)
            int rssi; 
            cyw43_wifi_get_rssi(&cyw43_state, &rssi);

            // Linha 1: Status de conexão
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("WiFi: Conectado", Font_7x10, Black);

            // Linha 2: Endereço IP
            ssd1306_SetCursor(0, 12);
            snprintf(status, sizeof(status), "IP: %d.%d.%d.%d", 
                     ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
            ssd1306_WriteString(status, Font_7x10, Black);

            // Linha 3: Intensidade do sinal (RSSI)
            ssd1306_SetCursor(0, 24);
            snprintf(status, sizeof(status), "Sinal: %d dBm", rssi);
            ssd1306_WriteString(status, Font_7x10, Black);

            // Linha 4: SSID (supondo que a variável global "wifi_ssid" possua o nome da rede)
            ssd1306_SetCursor(0, 36);
            snprintf(status, sizeof(status), "SSID: %s", wifi_ssid);
            ssd1306_WriteString(status, Font_7x10, Black);

            // Linha 5: Endereço MAC
            ssd1306_SetCursor(0, 48);
            const u8_t *mac = cyw43_state.netif[0].hwaddr;
            snprintf(status, sizeof(status), "MAC: %02X:%02X:%02X:%02X:%02X:%02X",
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            ssd1306_WriteString(status, Font_6x8, Black);

            // Se precisar de mais status (como canal ou modo), pode adicionar outra linha:
            // ssd1306_SetCursor(0, 60);
            // ssd1306_WriteString("Modo: STA", Font_7x10, Black);
        } else {
            // Se não estiver conectado, exibe apenas o status de desconexão
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("WiFi: Desconectado", Font_7x10, Black);
        }

        // Atualiza o display para mostrar as mudanças
        ssd1306_UpdateScreen();

        // Se o botão no pino 22 for pressionado, retorna ao menu principal
        if (gpio_get(22) == 0)
        {
            DEBOUNCE;
            wifi_home();
            break;
        }

        POLLING_TIME;  // Delay para evitar sobrecarga (defina este macro conforme sua necessidade)
    }
}

/**
 * @brief Exibe o status da conexão da Nuvem no display OLED.
 * 
 * Esta função exibe informações como endereço IP do servidor. Ela também verifica se o botão no pino 22 foi pressionado para retornar ao menu principal.
 */
void webserver_status(void) 
{
    char status[64];

    while(1)
    {
        // Limpa o display (fundo branco)
        ssd1306_Fill(White);

        // Verifica se a interface de rede está ativa
        if(netif_is_up(&cyw43_state.netif[0])) {
            uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);

            // Linha 1: Status de conexão
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Nuvem: Ativa", Font_7x10, Black);

            // Linha 2: Endereço IP
            // Linha 2: Endereço do servidor
            ssd1306_SetCursor(0, 12);
            snprintf(status, sizeof(status), "Servidor: %s", cloud_server_ip);
            ssd1306_WriteString(status, Font_6x8, Black);
      
        } else {
            // Se não estiver conectado, exibe apenas o status de desconexão
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Nuvem: Desconectado", Font_7x10, Black);
        }

        // Atualiza o display para mostrar as mudanças
        ssd1306_UpdateScreen();

        // Se o botão no pino 22 for pressionado, retorna ao menu principal
        if (gpio_get(22) == 0)
        {
            DEBOUNCE;
            wifi_home();
            break;
        }

        POLLING_TIME;  // Delay para evitar sobrecarga (defina este macro conforme sua necessidade)
    }
}