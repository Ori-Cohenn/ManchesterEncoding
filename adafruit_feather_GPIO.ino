#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define WRITE_PIN A2
#define READ_PIN A3
#define DELAY_TIME 104
#define MAX_DATA_SIZE 40 // 10 bytes
#define ENCODED_MAX_DATA_SIZE 80 // 10 bytes *2 (manchester encoding)
#define INTIAL_STATE HIGH
int mode = 1; // 0 for write, 1 for read
// 1/9600= 104.16 microseconds


void writeSetup(){
    pinMode(WRITE_PIN, OUTPUT);
    digitalWrite(WRITE_PIN, INTIAL_STATE); //intial state is idle
}

void writeLoop()
{
    digitalWrite(WRITE_PIN, HIGH);
    delayMicroseconds(DELAY_TIME);
    digitalWrite(WRITE_PIN, LOW);
}

// 1/9600= 104.16 microseconds
void readSetup()
{
    pinMode(READ_PIN, INPUT);
}

void readLoop()
{
    int currentState = digitalRead(READ_PIN); // intial state
    char *receivedData = (char *)malloc(ENCODED_MAX_DATA_SIZE);
    char *decodedData = (char *)malloc(ENCODED_MAX_DATA_SIZE/2);
    memset(decodedData,0,ENCODED_MAX_DATA_SIZE/2);
    int index = 0;
    int lastState = currentState;
    Serial.println(currentState);
    while (currentState == INTIAL_STATE) // idle state waiting for start condition
    {
        currentState = digitalRead(READ_PIN);
        Serial.println(currentState);
    }
    
    // Read Manchester encoded data
    
    unsigned long lastChangeTime = micros();
    while (index < ENCODED_MAX_DATA_SIZE && ((micros() - lastChangeTime < 4 * DELAY_TIME)))
    {
        currentState = digitalRead(READ_PIN);
        Serial.println(currentState);
        if (currentState != lastState)
        {
            receivedData[index++] = currentState == HIGH ? '1' : '0';
            lastChangeTime = micros();
            lastState = currentState;
        }
    }
    int isDecoded=manchesterDecode(receivedData, index, decodedData);
    if(isDecoded==0)
    {
        Serial.println("Decoded data:");
        Serial.println(decodedData);
    }
    else
    {
        Serial.println("Error: Invalid Manchester encoding sequence.");
    }
    free(receivedData);
    free(decodedData);
}

void setup()
{
  Serial.begin(9600);
  if (mode == 0)
  {
    writeSetup();
  }
  if (mode == 1)
  {
    readSetup();
  }
}

void loop()
{
  if (mode == 0)
  {
    writeLoop();
  }
  if (mode == 1)
  {
    readLoop();
  }
  delay(1000);
}