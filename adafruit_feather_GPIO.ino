#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define WRITE_PIN A2
#define READ_PIN A3
// #define DELAY_TIME 104
#define DELAY_TIME 500            // 0.5 seconds
#define MAX_DATA_bits 84          // 10 bytes * 8 bits
#define ENCODED_MAX_DATA_bits 168 // 10 bytes *2 (manchester encoding)
#define INTIAL_STATE HIGH
int mode = 1; // 0 for write com4, 1 for read com9
// 1/9600= 104.16 microseconds

void readerSetup()
{
  pinMode(READ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(READ_PIN), readStart, FALLING); // detect start bit
}
void readStart()
{
  noInterrupts();
  delayMicroseconds(104); // debounce time
  readData();
  interrupts();
}
void writerSetup()
{
  pinMode(WRITE_PIN, OUTPUT);
}
void readData()
{
  uint8_t dataReceived[ENCODED_MAX_DATA_bits];
  uint8_t decodedData[MAX_DATA_bits];
  int i = 0;
  delay(DELAY_TIME); // wait for the start bit to end
  Serial.println("\nReading Data: ...");
  while (!(dataReceived[i - 1] == 1 && dataReceived[i - 2] == 1 && dataReceived[i - 3] == 1 && dataReceived[i - 4] == 1) && i - 1 < ENCODED_MAX_DATA_bits)
  {
    dataReceived[i] = digitalRead(READ_PIN);
    Serial.print("r=");
    Serial.print(dataReceived[i]);
    Serial.print("i=");
    Serial.print(i);
    delay(DELAY_TIME);
    i++;
  }
  manchesterDecode(dataReceived, (i - 4), decodedData);
  Serial.print("\nDecoded Data: ");
  for (int j = 0; j < (i - 4) / 2; j++)
  {
    Serial.print(decodedData[j]);
  }
  // changing the data from binart representation to hex
  int hexData[MAX_DATA_bits];
  Serial.print("\nData in Hex: ");
  for (int j = 0; j < (i - 4) / 2; j++)
  {
    hexData[j] = 0;
    for (int k = 0; k < 4; k++)
    {
      hexData[j] = hexData[j] << 1;
      hexData[j] = hexData[j] + decodedData[j * 4 + k];
    }
    Serial.print(hexData[j], HEX);
  }
}
// write data, start with idle state 11111.... then start bit 0, data, stop bits 1111, idle state 11111...
void writeData(void)
{
  uint8_t dataHex[] = {0x03};
  uint8_t databits[MAX_DATA_bits];
  int dataSize = sizeof(dataHex) / sizeof(dataHex[0]);
  uint8_t encodedData[ENCODED_MAX_DATA_bits];
  Serial.print("Data in Hex: ");
  for (int j = 0; j < dataSize; j++)
  {
    Serial.print(dataHex[j], HEX);
  }
  // changing the data from hex representation to binary
  Serial.print("\nData in Binary: ");
  for (int j = 0; j < dataSize; j++)
  {
    for (int k = 0; k < 4; k++)
    {
      databits[j * 4 + k] = dataHex[j] >> 3 - k & 1;
      Serial.print(databits[j * 4 + k]);
    }
  }
  manchesterEncode(databits, dataSize * 4, encodedData);
  Serial.print("\nEncoded Data: ");
  for (int j = 0; j < dataSize * 8; j++)
  {
    Serial.print(encodedData[j]);
  }
  Serial.println();
  // idle state
  digitalWrite(WRITE_PIN, HIGH);
  delay(DELAY_TIME * 5); // 11111
  // start bit
  digitalWrite(WRITE_PIN, LOW);
  delay(DELAY_TIME);
  for (int i = 0; i < dataSize * 8; i++)
  {
    if (encodedData[i] == 1)
    {
      digitalWrite(WRITE_PIN, HIGH);
    }
    else
    {
      digitalWrite(WRITE_PIN, LOW);
    }
    delay(DELAY_TIME);
  }
  // stop bits
  digitalWrite(WRITE_PIN, HIGH);
  delay(DELAY_TIME * 4); // 1111
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  if (mode == 0)
  {
    writerSetup();
  }
  else if (mode == 1)
  {
    readerSetup();
  }
}

void loop()
{
  if (mode == 0)
  {
    writeData();
    delay(DELAY_TIME * 5);
  }
  else if (mode == 1)
  {
    // inerupt will handle the read
  }
}