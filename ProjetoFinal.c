#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"

// para o I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Pinos
#define led_RED 13   // Red=13, Blue=12, Green=11
#define led_BLUE 12  // Red=13, Blue=12, Green=11
#define led_GREEN 11 // Red=13, Blue=12, Green=11
#define botao_pinA 5 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define botao_pinB 6 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define joybutton 22 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define VRY_PIN 26   // Pino do Joystick Y
#define VRX_PIN 27   // Pino do Joystick X
#define buzzer 10    // Pino do buzzer A

// Variáveis globais
static volatile uint32_t last_time = 0;     // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t tempo_media = 0;   // Armazena o tempo para a media (em microssegundos)
static volatile uint32_t ponto_grafico = 0; // Armazena o tempo para o grafico (em microssegundos)
static volatile uint32_t current_time = 0;  // Armazena o tempo atual (em microssegundos)
bool grafico = true;                        // Se mostra ou nao o grafico
bool pressaoxbatimento = true;              // Grafico da pressao ou do batimento
bool Verde = false;                         // Variavel para Led Verde
bool Amarelo = false;                       // Variavel para Led Amarelo
bool Vermelho = false;                      // Variavel para Led Vermelho
int8_t pressoes[128];                       // Pontos grafico da pressao
int8_t batimentos[128];                     // Pontos grafico do batimento
int8_t mpressao[10];                        // Media da pressao (10 pontos por 1 segundo)
int8_t mbatimentos[10];                     // Media do batimento (10 pontos por 1 segundo)
int8_t cpressao = 0;                        // Aonde armazena o valor da pressao atual
int8_t cbatimentos = 0;                     // Aonde armazena o valor do batimento atual

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 150000) // 150 ms de debouncing
    {
        if (gpio == joybutton)
        {
        }
        if (gpio == botao_pinA)
        {
        }
        if (gpio == botao_pinB)
        {
        }
        last_time = current_time; // Atualiza o tempo do último evento
    }
}

int main()
{
    stdio_init_all();

    gpio_init(joybutton);
    gpio_set_dir(joybutton, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(joybutton);          // Habilita o pull-up interno
    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(joybutton, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(botao_pinA);
    gpio_set_dir(botao_pinA, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(botao_pinA);          // Habilita o pull-up interno
    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(botao_pinA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(botao_pinB);
    gpio_set_dir(botao_pinB, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(botao_pinB);          // Habilita o pull-up interno
    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(botao_pinB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(led_GREEN);              // Inicializa o pino do LED Verde
    gpio_set_dir(led_GREEN, GPIO_OUT); // Configura o pino como saída
    gpio_init(led_BLUE);               // Inicializa o pino do LED Azul
    gpio_set_dir(led_BLUE, GPIO_OUT);  // Configura o pino como saída
    gpio_init(led_RED);                // Inicializa o pino do LED Vermelho
    gpio_set_dir(led_RED, GPIO_OUT);   // Configura o pino como saída

    adc_init();
    adc_gpio_init(VRY_PIN);
    adc_gpio_init(VRX_PIN);

    uint32_t last_print_time = 0;

    // Para o display
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_t ssd;                                                // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    // Limpa o display.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd); // Atualiza o display

    for (int i = 0; i < 128; i++)
    { // Zerar variaveis para evitar lixo armazenado
        pressoes[i] = 0;
        batimentos[i] = 0;
    }
    while (true)
    {
        current_time = to_us_since_boot(get_absolute_time());
        adc_select_input(1);
        int16_t vrx_value = adc_read();
        adc_select_input(0);
        int16_t vry_value = adc_read();
    }
}
