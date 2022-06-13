/*
Projeto: Rele eletrônico de monitoramento de temperatura e acionamento.
AUTOR: Gilmar 
DATA: 13/06/2022
OBJETIVO:
IMPRIMIR NO DISPLAY DISPLAY DUAS TEMPERATURAS CONVERTIDAS PELOS SENSORES LM35
CONTROLAR SAÍDAS DIGITAIS 
Etapa de desenvolvimento
Controle do display LCD 16x2
*/
// --- Bibliotecas Auxiliares       --- //
#include <LCD_I2C.h> // biblioteca da interface I2C para acionar display
// =================================================================================
// --- Instâncias                   --- //
// Estou criando um objeto da classe LCD_I2C
// para saber mais pesquise por programação orientada a objetos!!!!
LCD_I2C lcd(0x27, 16, 2); // Endereçamento do display na rede I2c o padrão é o endereço hexadecimal 27 (Default address of most PCF8574 modules, change according)
LCD_I2C lcd_2(0x26, 16, 2); // Prova de configuração do display alterado o endereço fisicamente para provar funcionamento com dois displays no mesmo barrametno de dados.
// =================================================================================
// --- Variáveis Globais ---

// =================================================================================
// --- Protótipo das Funções  organizados em outro arquivo---


// =================================================================================
// --- Configurações Iniciais ---
void setup()
{
    lcd.begin(); // usando a função da biblioteca do display para partir o display
    lcd.backlight(); // ligando as luzes do display função dentro da biblioteca
 
} // end setup
// =================================================================================
// --- Loop Infinito ---
void loop()
{
    lcd.setCursor(0, 0); // definindo as posições iniciais da msg
    lcd.print("     Hello"); // mandando um aoba para o display
    lcd.setCursor(5, 1); // definindo as posições iniciais da msg
    lcd.print("END 27!");
    delay(500);// função que aguarda o tempo de 500 ms para proceguir no código

    // fazer o display piscar só pra lembrar do natal!
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
     
}// end loop

// =================================================================================
