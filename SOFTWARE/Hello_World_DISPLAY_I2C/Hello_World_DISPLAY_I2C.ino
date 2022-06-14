/*
Projeto: Rele eletrônico de monitoramento de temperatura e acionamento.
AUTOR: Gilmar 
DATA: 13/06/2022
OBJETIVO:
IMPRIMIR NO DISPLAY DISPLAY DUAS TEMPERATURAS CONVERTIDAS PELOS SENSORES LM35
CONTROLAR SAÍDAS DIGITAIS 
Etapa de desenvolvimento
ok  -   Controle do display LCD 16x2
ok  -   Controle do sensor de temperatura
ok  -   Controle de  saidas     digitais
ok  -   Controle de  entradas   digitais
imprementar logica projeto
*/
// --- Bibliotecas Auxiliares       --- //
#include <LCD_I2C.h> // biblioteca da interface I2C para acionar display
// =================================================================================
// --- Mapeamento de Hardware ---
#define pin_lm35_motor A0
#define pin_lm35_mancal A1
#define buzzer 12   // 

#define led_k1 3   //  
#define led_k2 4   //  
#define led_k3 5   //  
#define led_falha 6   //  

#define Bt1 7   //
#define Bt2 8   //    
#define interlock 9   //  
// =================================================================================
// --- Instâncias                   --- //
// Estou criando um objeto da classe LCD_I2C
// para saber mais pesquise por programação orientada a objetos!!!!
LCD_I2C lcd(0x27, 16, 2); // Endereçamento do display na rede I2c o padrão é o endereço hexadecimal 27 (Default address of most PCF8574 modules, change according)
LCD_I2C lcd_2(0x26, 16, 2); // Prova de configuração do display alterado o endereço fisicamente para provar funcionamento com dois displays no mesmo barrametno de dados.
// =================================================================================
// --- Variáveis Globais ---
float   valor_analog_lm35_motor     =   0.0,
        valor_analog_lm35_mancal    =   0.0,
        tensao_motor                =   0.0,
        tensao_mancal               =   0.0,
        temperatura_motor           =   0.0,
        temperatura_mancal          =   0.0;

static boolean  flag4               =   0,
                flag5               =   0; // variavel usada para armazenar mudança de estado dos botoes

unsigned long timeold               =   0; //variavel que armazena o tempo
int temporizador                    =   1000; // tempo em milisegundos!!!!
// =================================================================================
// --- Protótipo das Funções  ---
// Rotina para ler sensor motor retorna a leitura convertida
float ler_sensor_motor  (           );
// Rotina para ler sensor mancal retorna a leitura convertida
float ler_sensor_mancal (           );
// Rotina que le os botões
void readKey            (           ); 
//
void temporizador_      (int temp   );

// =================================================================================
// --- Configurações Iniciais ---
void setup()
{
    lcd.begin(); // usando a função da biblioteca do display para partir o display
    lcd.backlight(); // ligando as luzes do display função dentro da biblioteca
    // comunicação serial para debug do código
    Serial.begin(9600); // Inicia a comunicação com

    pinMode(pin_lm35_motor,     INPUT           ); // Define o sensor como uma entrada de sinal
    pinMode(pin_lm35_mancal,    INPUT           ); // Define o sensor como uma entrada de sinal
    // entradas digitais
        pinMode(Bt1,            INPUT_PULLUP    );
        pinMode(Bt2,            INPUT_PULLUP    );
        pinMode(interlock,      INPUT_PULLUP    );
    // saidas digitais
        pinMode(led_k1,         OUTPUT          );
        pinMode(led_k2,         OUTPUT          );
        pinMode(led_k3,         OUTPUT          );
        pinMode(led_falha,      OUTPUT          );
 
 
    // armazena primeira leitura sensor motor
    valor_analog_lm35_motor  = float(analogRead(pin_lm35_motor));
    // temperatura sensor mancal
    valor_analog_lm35_mancal = float(analogRead(pin_lm35_mancal));
    tensao_motor  = (valor_analog_lm35_motor * 5)  / 1023;
    tensao_mancal = (valor_analog_lm35_mancal * 5) / 1023;
    temperatura_motor  = tensao_motor  / 0.010; // dividimos a tensão por 0.010 que representa os 10 mV
    temperatura_mancal = tensao_mancal / 0.010;
    // debug inicial
    Serial.print    ("A temperatura motor eh "  );
    Serial.println  (ler_sensor_motor ()        );
    Serial.print    ("A temperatura mancal eh " );
    Serial.println  (ler_sensor_mancal ()       );
    // fim debug
    delay           (1000                       );
    // teste inicial de conexões eletricas
    // debug eletrico
    digitalWrite(led_k1,        HIGH);
    digitalWrite(led_k2,        HIGH);
    digitalWrite(led_k3,        HIGH);
    digitalWrite(led_falha,     HIGH);
    delay       (1000               );
    digitalWrite(led_k1,        LOW );
    digitalWrite(led_k2,        LOW );
    digitalWrite(led_k3,        LOW );
    digitalWrite(led_falha,     LOW );
    // fim debug

    // debug display
    lcd.setCursor   (0, 0           ); // definindo as posições iniciais da msg
    lcd.print       ("     Hello"   ); // mandando um aoba para o display
    lcd.setCursor   (5, 1           ); // definindo as posições iniciais da msg
    lcd.print       ("END 27!"      );
    delay           (500            );// função que aguarda o tempo de 500 ms para proceguir no código
    for (int i = 0; i < 5; ++i){
        lcd.backlight(); // liga
        delay(50);
        lcd.noBacklight(); // nao liga!!!
        delay(50);
        lcd_2.backlight();
        delay(50);
        lcd_2.noBacklight();
        delay(50);
    }// end for
    lcd.clear(); // limpando o texto do display
    delay(500);
    // fim debug display

 
} // end setup
// =================================================================================
// --- Loop Infinito ---
void loop()
{
 
 
}// end loop

// =================================================================================
float ler_sensor_motor (){
    valor_analog_lm35_motor = float(analogRead(pin_lm35_motor));
    tensao_motor = (valor_analog_lm35_motor * 5) / 1023;
    temperatura_motor = tensao_motor / 0.010;
    return  temperatura_motor;
}// end ler_sensor_motor

float ler_sensor_mancal (){
    valor_analog_lm35_mancal = float(analogRead(pin_lm35_mancal));
    tensao_mancal = (valor_analog_lm35_mancal * 5) / 1023;
    temperatura_mancal = tensao_mancal / 0.010;
    return  temperatura_mancal;
}// end ler_sensor_motor

void readKey() {
        static boolean flag1 = 0, flag2 = 0, flag3 = 0; // teste local

        if (digitalRead(bt1)) {
            if (flag4 == 1) {
                Serial.println("partiu bt1!  "); // debug via serial
                delay(25);
                // colocar função aqui
                cont = 0;
            }
            flag1 = 0x01;
            flag4 = 0x00; // teste memoria de outros testes
            delay(10);
        }
        if (!digitalRead(bt1) && flag1) {
            flag1 = 0x00;
            flag4 = 0x01;
            Serial.println("RESET bt1");
            delay(10);
        }
        // logica para o outro botao
        if (digitalRead(bt2)) {
            if (flag5 == 1) {
                Serial.println("partiu bt2!  "); 
                delay(25);
                // colocar funcao aqui
                cont = 0;
            }
            flag2 = 0x01;
            flag5 = 0x00; // teste memoria de outros testes
            delay(10);
        }
        if (!digitalRead(bt2) && flag2) {
            flag2 = 0x00;
            flag5 = 0x01;
            Serial.println("RESET bt2");
            delay(10);
        }
} // readKey

void temporizador_(int temp){
    if (millis() - timeold >= temp){
        Serial.print    ("O intervado maior que  "  );
        Serial.println  (temp);
        // colocar função aqui
        timeold = millis();
    }// end if
}//endtemporizador

     