#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define BufferSize 10 // 10 bytes buffer
int stringIndex = 0;
const char originalBits[] = {1, 1};
int dataSize = 0;
char *stringSend;
char *stringReceived;
void setup()
{
  Serial.begin(9600);  // Initialize the Serial monitor for debugging
  Serial1.begin(9600); // Initialize Serial1 for sending data
  dataSize = sizeof(originalBits) / sizeof(originalBits[0]);
  stringSend = (char *)malloc(dataSize * 2);
  stringReceived = (char *)malloc(BufferSize);
}

void readUart(void)
{
  while ((Serial1.available()) || stringSend[stringIndex] == 13) // 13 is the ASCII value of carriage return
  {
    stringSend[stringIndex] = Serial1.read();
    stringIndex++;
  }
  Serial.print("\nManchester Encoded Data Received: ");
  Serial.println(stringSend);
  manchesterDecode(stringSend, BufferSize, stringReceived);
  Serial.print("\nManchester Decoded Data: ");
  Serial.println(stringReceived);
  stringIndex = 0;
  memset(stringSend, 0, BufferSize);     // clear buffer
  memset(stringReceived, 0, BufferSize); // clear buffer
}

void writeUart(void)
{
  manchesterEncode(originalBits, dataSize, stringSend);
  Serial.println("\nManchester Encoded Data: ");
  for (int i = 0; i < dataSize * 2; i++)
  {
    Serial.print(stringSend[i], DEC);
  }
  Serial1.write(stringSend, dataSize * 2);
  memset(stringSend, 0, dataSize * 2); // clear buffer
}

void loop()
{
  Serial.println("\nOriginal Bits: ");
  for (int i = 0; i < dataSize; i++)
  {
    Serial.print(originalBits[i], DEC);
  }
  writeUart();
  //readUart();
  delay(1000);
}