#include <Arduino.h>
#include <stdint.h>

#define RED_LED_PIN D5
#define GREEN_LED_PIN D6
#define BLUE_LED_PIN D7

void initLEDs()
{
    pinMode(RED_LED_PIN,OUTPUT);
    pinMode(GREEN_LED_PIN,OUTPUT);
    pinMode(BLUE_LED_PIN,OUTPUT);
    digitalWrite(RED_LED_PIN,HIGH);
    digitalWrite(GREEN_LED_PIN,HIGH);
    digitalWrite(BLUE_LED_PIN,HIGH);
}

void setLightsColor(uint8_t r, uint8_t g, uint8_t b)
{
    analogWrite(RED_LED_PIN,255-r);
    analogWrite(GREEN_LED_PIN,255-g);
    analogWrite(BLUE_LED_PIN,255-b);

}