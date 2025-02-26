# Aluno: Wilton Lacerda Silva Júnior
## Grupo 6 Subgrupo 5
## Matrícula: TIC370100193
# Video explicativo: https://youtu.be/CUQtGfITsVc
# Atividade 1 Projeto Final
# Monitoramento de Sinais Vitais para Ambientes Hospitalares
O objetivo do projeto é dar maior segurança para enfermos em hospitais, com monitoramento constante de seus sinais vitais e chamadas de especialistas para intervenção caso esses sinais estejam anormais.
## Funcionalidades

- **Joystick:**  
  - O Joystick funciona como dois potenciômetros, podendo assim ler os volores do X e Y dele com ADC.
  - Ele será usado para simular os valroes da pressão e batimentos cardíacos.
- **2 Leds:**
  - Os leds serão retornos visuais do estado do enfermo.
- **Display OLED**
  - O display mostrara os valores e os graficos dos valores em relação ao tempo.
- **2 Botões**
  - Os botões serão usados para alterar o modo de gráfico, qual gráfico está sendo mostrado, e acionar o modo de emergência.
- **Buzzer**
  - O buzzer será o retorno sonoro para situações de emergências.

# Requisitos
## Hardware:

- Raspberry Pi Pico W.
- 2 potenciômetros nas portas 26 e 27.
- 3 LEDS nas portas 11 e 13.
- 2 botões nas portas 5 e 6.
- 1 display ssd1306 com o sda na porta 14 e o scl na porta 15.
- 1 buzzer na porta 10

## Software:

- Ambiente de desenvolvimento VS Code com extensão Pico SDK.

# Instruções de uso
## Configure o ambiente:
- Certifique-se de que o Pico SDK está instalado e configurado no VS Code.
- Compile o código utilizando a extensão do Pico SDK.
## Teste:
- Utilize a placa BitDogLab para o teste. Caso não tenha, conecte os hardwares informados acima nos pinos correspondentes.
