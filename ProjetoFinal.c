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
static volatile uint32_t last_time = 0;    // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t tempo_media = 0;  // Armazena o tempo para a media (em microssegundos)
static volatile uint32_t current_time = 0; // Armazena o tempo atual (em microssegundos)
bool alarme;
bool grafico = false;          // Se mostra ou nao o grafico
bool pressaoxbatimento = true; // Grafico da pressao ou do batimento
bool Verde = false;            // Variavel para Led Verde
bool Amarelo = false;          // Variavel para Led Amarelo
bool Vermelho = false;         // Variavel para Led Vermelho
int16_t contapressao = 0;
int16_t contabatimentos = 0;
int8_t apressao;
int8_t abatimentos;
int8_t pressoes[128];   // Pontos grafico da pressao
int8_t batimentos[128]; // Pontos grafico do batimento
int8_t mpressao[10];    // Media da pressao (10 pontos por 1 segundo)
int8_t mbatimentos[10]; // Media do batimento (10 pontos por 1 segundo)
int8_t cgrafico = 0;    // Aonde armazena o valor da pressao e batimento atual
int8_t cmedia = 0;      // Aonde armazena o valor da media

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
            if (grafico)
            {
                pressaoxbatimento = !pressaoxbatimento;
            }
            else
            {
                alarme = !alarme;
                if (alarme)
                {
                    gpio_put(buzzer, true);
                }
                else
                {
                    gpio_put(buzzer, false);
                }
            }
        }
        if (gpio == botao_pinB)
        {
            grafico = !grafico;
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
    tempo_media = to_us_since_boot(get_absolute_time());
    while (true)
    {
        current_time = to_us_since_boot(get_absolute_time());
        adc_select_input(1);
        int16_t vrx_value = adc_read();
        adc_select_input(0);
        int16_t vry_value = adc_read();

        apressao = vrx_value / 17;
        abatimentos = vry_value / 25;

        if (current_time - tempo_media > 100000)
        {
            tempo_media = to_us_since_boot(get_absolute_time());
            mpressao[cmedia] = apressao;
            mbatimentos[cmedia] = abatimentos;
            cmedia++;
            if (cmedia == 10)
            {
                cmedia = 0;
                for (int i = 0; i < 10; i++)
                {
                    contapressao = contapressao + mpressao[i];
                    contabatimentos = contabatimentos + mbatimentos[i];
                }
                pressoes[cgrafico] = contapressao / 10;
                batimentos[cgrafico] = contabatimentos / 10;
                contapressao = 0;
                contabatimentos = 0;
                cgrafico++;
                if (cgrafico == 128)
                {
                    cgrafico = 0;
                }
            }
        }
        if (grafico)
        { // mostrar graficos
            if (pressaoxbatimento)
            { // pressao
                for (int i = 0; i < 128; i++)
                {
                    contapressao = pressoes[i] / 2;
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "Pressao" , 100, 120);
                    ssd1306_vline(&ssd, i, 0, contapressao, 1);
                    contapressao = 0;
                }
            }
            else
            { // batimento
                for (int i = 0; i < 128; i++)
                {
                    contabatimentos = pressoes[i] / 1.64;
                    ssd1306_fill(&ssd, false);
                    ssd1306_draw_string(&ssd, "Batimentos" , 88, 120);
                    ssd1306_vline(&ssd, i, 0, contabatimentos, 1);
                    contabatimentos = 0;
                }
            }
        }
        else
        { // mostrar valores atuais
            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd, 3, 3, 122, 58, 1, 0); // Desenha um retângulo
        }
    }
}
