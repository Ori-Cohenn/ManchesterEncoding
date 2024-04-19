#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define WRITE_PIN A2
#define READ_PIN A3
//#define DELAY_TIME 104
#define DELAY_TIME 5000000
#define MAX_DATA_BYTES 10 // 10 bytes
#define ENCODED_MAX_DATA_BYTES 80 // 10 bytes *2 (manchester encoding)
#define INTIAL_STATE HIGH
int mode = 0; // 0 for write, 1 for read
// 1/9600= 104.16 microseconds

void writeSetup();
void writeLoop();
void readSetup();
void readStart();
void readData();

void setup()
{
  Serial.begin(9600);
  if (mode == 0)
  {
    writeSetup();
  }
  else if (mode == 1)
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
  else if (mode == 1)
  {
  //inerupt will handle the read
  }
}

void writeSetup(){
    pinMode(WRITE_PIN, OUTPUT);
    digitalWrite(WRITE_PIN, INTIAL_STATE); //intial state is idle
}

void writeLoop()
{
  uint8_t data[MAX_DATA_BYTES] = {0x03,0x01,0x00};// 10 bytes
  int dataSize = sizeof(data) / sizeof(data[0]);
  uint8_t datainbits[dataSize*8];
  Serial.print("\nData:");
  for(int i=0;i<dataSize;i++)
  {
    for(int j=0;j<8;j++)
    {
      datainbits[i*8+j]=(data[i]>>j)&1;
      Serial.print(datainbits[i*8+j]);
    }
  }
  int encodedDataSize=dataSize*8*2;
  uint8_t encodedData[encodedDataSize];
  int isEncoded=manchesterEncode(datainbits,encodedDataSize,encodedData);
  Serial.print("\nEncoded data:");
  for(int i=0;i<encodedDataSize;i++)
  {
    Serial.print(encodedData[i]);
  }
  if(isEncoded==0)
  {
    // for(int i=0;i<ENCODED_MAX_DATA_BYTES;i++)
    // {
    //   digitalWrite(WRITE_PIN,encodedData[i]);
    //   delayMicroseconds(DELAY_TIME);
    // }
  }
  else
  {
    Serial.println("Error: Invalid data.");
  }
}

void readSetup()
{
    pinMode(READ_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(READ_PIN), readStart, FALLING);
}

void readStart() // detect start condition
{
    detachInterrupt(digitalPinToInterrupt(READ_PIN)); 
    // wait for start condition to end 
    while(digitalRead(READ_PIN)==LOW);
    readData();
    attachInterrupt(digitalPinToInterrupt(READ_PIN), readStart, FALLING);
}

void readData()
{
    int currentState = digitalRead(READ_PIN); // intial state
    uint8_t *receivedData = (uint8_t *)calloc(ENCODED_MAX_DATA_BYTES, sizeof(uint8_t));
    uint8_t *decodedData = (uint8_t *)calloc(ENCODED_MAX_DATA_BYTES/2, sizeof(uint8_t));
    //memset(decodedData,0,ENCODED_MAX_DATA_BYTES/2);
    int index = 0;
    int lastState = currentState;
    Serial.println("ISR");
    Serial.println(currentState);
    // while (currentState == INTIAL_STATE) // while idle
    // {
    //     currentState = digitalRead(READ_PIN);
    // }
    
    // // Read Manchester encoded data
    unsigned long lastChangeTime = micros(); 
    while (1)
    {
        currentState = digitalRead(READ_PIN);
        if (currentState != lastState)
        {
            lastChangeTime = micros();
            lastState = currentState;
            if (currentState == HIGH)
            {
                receivedData[index++] = 1;
            }
            else
            {
                receivedData[index++] = 0;
            }
        }
        // stop condition is when the data is 1111 
        if ((receivedData[index - 4] == 1 && receivedData[index - 3] == 1 && receivedData[index - 2] == 1 && receivedData[index - 1] == 1) || index >= ENCODED_MAX_DATA_BYTES)
        {
          // remove the stop condition
            break;
        }
    }
    Serial.println("Received data:");

    int isDecoded=manchesterDecode(receivedData, index, decodedData);
    
    if(isDecoded==0)
    {
        Serial.println("Decoded data:");
        for (int i = 0; i < index / 2; i++)
        {
            Serial.print(decodedData[i]);
        }
    }
    else
    {
        Serial.println("Error: Invalid Manchester encoding sequence.");
    }
    free(receivedData);
    free(decodedData);
}

