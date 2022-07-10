/*
Projeto: Rele eletrônico de monitoramento de temperatura e acionamento.
AUTOR: Gilmar 
DATA: 13/06/2022
OBJETIVO:
IMPRIMIR NO DISPLAY DISPLAY DUAS TEMPERATURAS CONVERTIDAS PELOS SENSORES LM35
CONTROLAR SAÍDAS DIGITAIS E RECEBER COMANDOS VIA ENTRADAS DIGITAIS
Etapa de desenvolvimento
ok  -   Controle do display LCD 16x2
ok  -   Controle do sensor de temperatura
ok  -   Controle de  saidas     digitais
ok  -   Controle de  entradas   digitais
ok  -   Rotina de partida
ok  -   Imprementar logica projeto
Juntar todas as lógicas e testar no proteus
*/
// --- Bibliotecas Auxiliares       --- //
#include <LCD_I2C.h> // biblioteca da interface I2C para acionar display
// =================================================================================
// --- Mapeamento de Hardware ---
#define pin_lm35_motor A0   //
#define pin_lm35_mancal A3  //
#define buzzer 7           // 
#define led_k1 2            //  
#define led_k2 3            //  
#define led_k3 4            //  
#define led_falha 6         //  
#define bt1 8               //
#define bt2 9               //    
#define interlock 5         //  
// =================================================================================
// --- Instâncias                   --- //
// Estou criando um objeto da classe LCD_I2C
// para saber mais pesquise por programação orientada a objetos!!!!
LCD_I2C lcd(0x27, 16, 2);   //  Endereçamento do display na rede I2c o padrão é o endereço hexadecimal 27 
                            //  (Default address of most PCF8574 modules, change according)
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

unsigned long   timeold             =   0; //variavel que armazena o tempo
int             temporizador        =   1000, // tempo em milisegundos!!!!
                cont                =  0, // variavel temporaria
                temp                =  1000,
                TEMP_RA             = 23000;
                int teste_temp = 0;
// =================================================================================
// --- Protótipo das Funções  ---
// Rotina para ler sensor motor retorna a leitura convertida
float ler_sensor_motor  (                                       );
// Rotina para ler sensor mancal retorna a leitura convertida
float ler_sensor_mancal (                                       );
// Rotina que le os botões
void readKey            (                                       ); 
// Rotina de temporizador que não para a leitura dos botoes
void temporizador_      (int temp                               );
// Rotina de partida do equipamento
void partiu             (                                       ); 
// Rotina display temperatura
void mostrar_temp       (                                       );
// Rotina rele eletronico
void rele_eletronico    (int partir,int parar,int temporizador  );
// =================================================================================
// --- Configurações Iniciais ---
void setup()
{
    lcd.begin(); // usando a função da biblioteca do display para partir o display
    lcd.backlight(); // ligando as luzes do display função dentro da biblioteca
    // comunicação serial para debug do código
    Serial.begin(9600); // Inicia a comunicação com
    // entradas digitais
        pinMode(pin_lm35_motor  , INPUT         ); // Define o sensor como uma entrada de sinal
        pinMode(pin_lm35_mancal , INPUT         ); // Define o sensor como uma entrada de sinal
        pinMode(bt1             , INPUT_PULLUP  );
        pinMode(bt2             , INPUT_PULLUP  );
        pinMode(interlock       , INPUT_PULLUP  );
    // saidas digitais
        pinMode(led_k1          , OUTPUT        );
        pinMode(led_k2          , OUTPUT        );
        pinMode(led_k3          , OUTPUT        );
        pinMode(led_falha       , OUTPUT        );
        pinMode(buzzer          , OUTPUT        );
    partiu ();
} // end setup
// =================================================================================
// --- Loop Infinito ---
void loop(){
    readKey();
    delay(10);

        if (millis() - timeold >= temp){
            Serial.print    ("O intervado maior que  "  );
            Serial.println  (temp);
            mostrar_temp ();
            timeold = millis();
        }// end if
}// end loop
// ===================  Desenvolvimento de funções   ===============================
float ler_sensor_motor (){//a0
    float v_lm35_motor  =0.00;
    float t_motor       =0.0;
    float temp_motor    =0.0;
    v_lm35_motor = analogRead(pin_lm35_motor);
    t_motor = (v_lm35_motor/1024) * 5000;
    temp_motor = t_motor / 100;
    return  temp_motor;


}// end ler_sensor_motor
// =================================================================================

float ler_sensor_mancal (){//a3
     float v_lm35_mancal  =0.00;
    float t_mancal       =0.0;
    float temp_mancal    =0.0;
    v_lm35_mancal = analogRead(pin_lm35_mancal);
    t_mancal = (v_lm35_mancal/1024) * 5000;
    temp_mancal = t_mancal / 100;
    return  temp_mancal;
}// end ler_sensor_motor
// =================================================================================
void readKey() {
    static boolean flag1 = 0, flag2 = 0, flag3 = 0; // teste local

        if (digitalRead(bt1) && !digitalRead(interlock) && teste_temp == 0  ) {
            if (flag4 == 1 ) {
                Serial.println("partiu bt1!  "); // debug via serial
                delay(25);
                int partir=1, parar=0;
                if (cont == 0) rele_eletronico    (partir, parar,TEMP_RA); 
                cont = 1;
            }
            flag1 = 0x01;
            flag4 = 0x00; // teste memoria de outros testes
            delay(10);
        }// end if
        if (!digitalRead(bt1) && flag1) {
            flag1 = 0x00;
            flag4 = 0x01;
            Serial.println("RESET bt1");
            delay(10);
        }// end if
        // logica para o outro botao
        if (digitalRead(bt2)) {
            if (flag5 == 1) {
                Serial.println("partiu bt2!  "); 
                delay(25);
                int partir=0, parar=1;
                rele_eletronico    (partir, parar,TEMP_RA);
                cont = 0;
            }// end if
            flag2 = 0x01;
            flag5 = 0x00; // teste memoria de outros testes
            delay(10);
        }// end if
        if (!digitalRead(bt2) && flag2) {
            flag2 = 0x00;
            flag5 = 0x01;
            Serial.println("RESET bt2");
            delay(10);
        }// end if
        if (digitalRead(interlock) || teste_temp == 1)
        {
            int partir=0, parar=1;
            digitalWrite(led_falha,        HIGH    );
            rele_eletronico    (partir, parar,TEMP_RA);
            cont = 0;
        }else{
            digitalWrite(led_falha,        LOW     );
        }// end if
} // readKey
// =================================================================================
void temporizador_(int temp){
    if (millis() - timeold >= temp){
        Serial.print    ("O intervado maior que  "  );
        Serial.println  (temp);
        
        // colocar função aqui
        timeold = millis();
    }// end if
}//endtemporizador
// =================================================================================
void partiu             (           ){
    // ROTINA BIP DUPLO
    digitalWrite(buzzer,        HIGH);
    delay(150);
    digitalWrite(buzzer,        LOW );
    delay(50);
    digitalWrite(buzzer,        HIGH);
    delay(150);
    digitalWrite(buzzer,        LOW );
    // FIM ROROTINA BIP DUPLO
    lcd.backlight   (                   ); // liga
    delay           (50                 );
    lcd.setCursor   (0, 0               ); // definindo as posições iniciais da msg
    lcd.print       ("UNIUBE QUIRINOPO" ); // mandando um aoba para o display
                    //1234567890123456
    lcd.setCursor   (0, 1               ); // definindo as posições iniciais da msg
    lcd.print       ("PROJETO ENG 01"   );
    delay           (2500               );// função que aguarda o tempo de 500 ms para proceguir no código
    lcd.clear       (                   );
    lcd.noBacklight (                   );
    delay(50);
    //
    lcd.backlight   (                   ); // liga
    delay           (50                 );
    lcd.setCursor   (0, 0               ); // definindo as posições iniciais da msg
    lcd.print       ("GILMAR P ARAUJO" ); // mandando um aoba para o display
                    //1234567890123456
    lcd.setCursor   (0, 1               ); // definindo as posições iniciais da msg
    lcd.print       ("RA:1125833     "  );
    delay           (2500               );// função que aguarda o tempo de 500 ms para proceguir no código
    lcd.clear       (                   );
    lcd.noBacklight (                   );
    delay(50);
    //
    lcd.backlight   (                   ); // liga
    delay           (50                 );
    lcd.setCursor   (0, 0               ); // definindo as posições iniciais da msg
    lcd.print       ("RELE ELETRONICO" ); // mandando um aoba para o display
                    //1234567890123456
    lcd.setCursor   (0, 1               ); // definindo as posições iniciais da msg
    lcd.print       ("Seja bem vindo  " );
    delay           (2500               );// função que aguarda o tempo de 500 ms para proceguir no código
    lcd.clear       (                   );
    lcd.noBacklight (                   );
    delay(50);
    lcd.backlight   (                   ); // liga
    digitalWrite(buzzer,        HIGH);
    delay(150);
    digitalWrite(buzzer,        LOW );
}// end partiu
// =================================================================================
void mostrar_temp (){
    // LINHA DE DE CIMA DO DISPLAY
    lcd.backlight   (                   );
    lcd.clear       (                   );
    delay(50);
    lcd.setCursor   (0, 0               ); 
    lcd.print       ("INTERLOCK:"       );
    lcd.setCursor   (10, 0              );
    if (digitalRead(interlock)) lcd.print ("ATIVO!");
    if (!digitalRead(interlock)) lcd.print ("LIVRE!");
    if (!digitalRead(interlock)) lcd.print ("LIVRE!");
    float tmotor = ler_sensor_motor();
    float tmancal = ler_sensor_mancal();
    //Serial.print("tmotor:");
    //Serial.print(tmotor);
    //Serial.print("tmancal:");
    //Serial.print(tmancal);
     //Serial.println("");
    if (tmotor >= 27 || tmancal >= 27)
    {
        teste_temp = 1;
        lcd.setCursor   (0, 0               ); 
        lcd.print       ("ALARME:"       );
        lcd.setCursor   (7, 0              );
        lcd.print ("TEMPERATURA");
          
    }else{
        teste_temp = 0;
    }
    // FIM LINHA DE DE CIMA DO DISPLAY
        // LINHA DE BAIXO DO DISPLAY
        lcd.setCursor   (0, 1               ); 
        lcd.print       ("MT"               );
        lcd.setCursor   (2, 1               ); 
        lcd.print       (ler_sensor_motor() );
        lcd.setCursor   (6, 1               ); 
        lcd.print       ("C"                );
        lcd.setCursor   (9, 1               );
        lcd.print       ("MC"               );
        lcd.setCursor   (11, 1              );
        lcd.print       (ler_sensor_mancal());
        lcd.setCursor   (15, 1              ); 
        lcd.print       ("C"                );
        delay           (100                );
        delay(50);
        // FIM LINHA DE BAIXO DO DISPLAY
}// end mostrar_temp 
// =================================================================================
void rele_eletronico    ( int partir, int parar, int TEMP_RA ){
    if (partir == 1 && parar == 0 )
        {
            digitalWrite(led_k1,        HIGH    );
            digitalWrite(led_k2,        HIGH     );
            digitalWrite(led_k3,        LOW     );
            //delay(1000);
            timeold = millis();
            while ( millis() - timeold <=  TEMP_RA) 
            {
                if (digitalRead(interlock) || digitalRead(bt2))
                {
                    digitalWrite(led_k1,        LOW    );
                    digitalWrite(led_k2,        LOW    );
                    digitalWrite(led_k3,        LOW    );
                    break;
                }// end if
            }// end while

            timeold = millis();
                if (!digitalRead(interlock) || !digitalRead(bt2))
                    {
                        digitalWrite(led_k2,        LOW     );
                        delay       (500                    );
                        if (digitalRead(interlock) || digitalRead(bt2))
                            {
                                digitalWrite(led_k1,        LOW    );
                                digitalWrite(led_k2,        LOW    );
                                digitalWrite(led_k3,        LOW    );
                            }else{
                                digitalWrite(led_k3,        HIGH    );
                            } // end if 
                    }// end if
        }// END IF

    if (partir == 0 && parar == 1)
        {  
            digitalWrite(led_k1,        LOW    );
            digitalWrite(led_k2,        LOW    );
            digitalWrite(led_k3,        LOW    ); 
        }// end if 
////hhhghjgjg
/* 
if(tempera motor for menor que setpoint) faça
if(tempera mancal for menor que setpoint) faça

*/

}// end rele_eletronico
// =================================================================================
/*  // armazena primeira leitura sensor motor
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
    // fim debug display    */
