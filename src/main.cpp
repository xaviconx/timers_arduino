//Este código está hecho para funcionar en ESP32.
//Es mejor práctica usar un arduino MEGA, Leonardo o un ESP32 debido al requerimiento
//de interrupciones que necesita este trabajo.

//Su función será correr 3 timers que inicien al mismo tiempo y mostrar el orden en que fueron
//activados en un LCD de 16x2

#include <Arduino.h>

//Pins used: (interrupt pins for Leonardo: 01237)
#define set_pin 15
#define reset_pin 16
#define input_1 17
#define input_2 18
#define input_3 19

//Variables que almacenan el tiempo:
unsigned long start_time = 0;
unsigned long interrupt_1 = 0;
unsigned long interrupt_2 = 0;
unsigned long interrupt_3 = 0;

bool set_flag = true;
bool ready_flag = false;

void trigger_1_callback();
void trigger_2_callback();
void trigger_3_callback();
void set_callback();
void reset_callback();

void setup() {
    pinMode(set_pin, INPUT_PULLUP);
    pinMode(reset_pin , INPUT_PULLUP);
    pinMode(input_1, INPUT_PULLUP);
    pinMode(input_2, INPUT_PULLUP);
    pinMode(input_3, INPUT_PULLUP);

    attachInterrupt(set_pin, set_callback, FALLING);
    attachInterrupt(reset_pin, reset_callback, FALLING);
    attachInterrupt(input_1, trigger_1_callback, FALLING);
    attachInterrupt(input_2, trigger_2_callback, FALLING);
    attachInterrupt(input_3, trigger_3_callback, FALLING);

    Serial.begin(115200);
}

void loop() {
    static unsigned long time_1 = 0;
    static unsigned long time_2 = 0;
    static unsigned long time_3 = 0;

    if((start_time == 0) && (set_flag)){
        Serial.print("Press set button to start \n");
        set_flag = false;
        ready_flag = true;
    }
    else if((interrupt_1 != 0)&&(interrupt_2 != 0)&&(interrupt_3 != 0) && ready_flag){
        time_1 = interrupt_1 - start_time;
        time_2 = interrupt_2 - start_time;
        time_3 = interrupt_3 - start_time;
        Serial.print("T1 = ");
        Serial.print(time_1);
        Serial.print("T2 = ");
        Serial.print(time_2);
        Serial.print("T3 = ");
        Serial.print(time_3);
        Serial.println();
        ready_flag = false;
    }
    delay(1000);
}

void trigger_1_callback(){
    interrupt_1 = millis();
}

void trigger_2_callback(){
    interrupt_2 = millis();
}

void trigger_3_callback(){
    interrupt_3 = millis();
}

void set_callback(){
    start_time = millis();
}

void reset_callback(){
    start_time = 0;
    interrupt_1 = 0;
    interrupt_2 = 0;
    interrupt_3 = 0;
    set_flag = true;
}