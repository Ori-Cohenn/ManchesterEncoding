#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#define size 100
char stringReceived[size];
int stringIndex = 0;
const unsigned char originalBits[] = {1, 1, 1, 1, 0};
int dataSize = 0;
char *stringSend;
void setup()
{
  Serial.begin(9600);  // Initialize the Serial monitor for debugging
  Serial1.begin(9600); // Initialize Serial1 for sending data
  dataSize = sizeof(originalBits) / sizeof(originalBits[0]);
  stringSend = (char *)malloc(dataSize * 2);
}

void readUart(void)
{
  while ((Serial1.available()) || stringReceived[stringIndex] == 13)
  {
    stringReceived[stringIndex] = Serial1.read();
    stringIndex++;
  }
  stringIndex = 0;
  Serial.println(stringReceived);
  memset(stringReceived, 0, size); // clear buffer
}

void loop()
{
  // readUart();
  Serial.println("\nOriginal Bits: ");
  for (int i = 0; i < dataSize; i++)
  {
    Serial.print(originalBits[i]);
  }
  manchesterEncode(originalBits, dataSize, stringSend);
  Serial.println("\nManchester Encoded Data: ");
  for (int i = 0; i < dataSize * 2; i++)
  {
    Serial.print(stringSend[i]);
  }
  Serial1.write(stringSend);
  memset(stringSend, 0, dataSize * 2); // clear buffer
  delay(1000);
}