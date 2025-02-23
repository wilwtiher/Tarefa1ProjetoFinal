#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
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

// Variáveis globais
static volatile uint32_t last_time = 0;     // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t tempo_media = 0;   // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t ponto_grafico = 0; // Armazena o tempo do último evento (em microssegundos)
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
    uint32_t current_time = to_us_since_boot(get_absolute_time());
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

// Funcao para iniciar pino PWM
uint pwm_init_gpio(uint gpio, uint wrap)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
    return slice_num;
}

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
