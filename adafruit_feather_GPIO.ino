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

volatile int startFlag = 0;

void printHex(uint8_t dataHex[], int dataSize)
{
  for (int i = 0; i < dataSize; i++)
  {
    Serial.print(dataHex[i], HEX);
  }
}

void binaryToHex(uint8_t binaryArr[], int size, char hexArr[])
{
  for (int i = 0; i < size; i += 4)
  {
    int nibble = 0;
    for (int j = 0; j < 4; j++)
    {
      nibble = (nibble << 1) | binaryArr[i + j];
    }
    if (nibble < 10)
    {
      hexArr[i / 4] = nibble + '0';
    }
    else
    {
      hexArr[i / 4] = nibble - 10 + 'A';
    }
  }
  hexArr[size / 4] = '\0'; // Null terminate the string
}

void readStart()
{
  startFlag = 1;
}

void readerSetup()
{
  pinMode(READ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(READ_PIN), readStart, FALLING);
}
void writerSetup()
{
  pinMode(WRITE_PIN, OUTPUT);
  digitalWrite(WRITE_PIN, HIGH);
}
void readData()
{
  uint8_t dataReceived[ENCODED_MAX_DATA_bits];
  uint8_t decodedData[MAX_DATA_bits];
  int bufferIndex = 0;
  char hexData[MAX_DATA_bits];
  int consecutiveOnesCount = 0;
  delay(DELAY_TIME); // Waiting for start bit to pass
  Serial.println("\n\nReading Data: ...");
  while (consecutiveOnesCount < 4 && bufferIndex - 1 < ENCODED_MAX_DATA_bits)
  {
    dataReceived[bufferIndex] = digitalRead(READ_PIN);
    consecutiveOnesCount = dataReceived[bufferIndex] == 1 ? consecutiveOnesCount + 1 : 0;
    Serial.print(dataReceived[bufferIndex]);
    delay(DELAY_TIME);
    bufferIndex++;
  }
  if (consecutiveOnesCount >= 4)
  {
    manchesterDecode(dataReceived, (bufferIndex - 4), decodedData);
    Serial.print("\nDecoded Data: ");
    for (int j = 0; j < (bufferIndex - 4) / 2; j++)
    {
      Serial.print(decodedData[j], DEC);
    }
    // changing the data from binary representation to hex
    Serial.print("\nData in Hex: ");
    binaryToHex(decodedData, (bufferIndex - 4) / 2, hexData);
    Serial.print(hexData);
    memset(dataReceived, 0, sizeof(dataReceived));
    memset(decodedData, 0, sizeof(decodedData));
    memset(hexData, 0, sizeof(hexData));
  }
}
// write data, start with idle state 11111.... then start bit 0, data, stop bits 1111, idle state 11111...
void writeData(void)
{
  // uint8_t dataHex[] = {0xDE,0xED,0xBE,0xEF,0x01,0x90};
  uint8_t dataHex[] = {0x01, 0x03};
  uint8_t databits[MAX_DATA_bits];
  int dataSize = sizeof(dataHex) / sizeof(dataHex[0]);
  uint8_t encodedData[ENCODED_MAX_DATA_bits];
  Serial.print("\nData in Hex: ");
  // Move to function
  printHex(dataHex, dataSize);
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
  delay(DELAY_TIME * 6); // 111111
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
    while (!Serial.available())
      ;
    if (Serial.read() == 'w')
      writeData();
  }
  else if (mode == 1)
  {
    if (startFlag == 1)
    {
      detachInterrupt(digitalPinToInterrupt(READ_PIN));
      startFlag = 0;
      readData();
      attachInterrupt(digitalPinToInterrupt(READ_PIN), readStart, FALLING);
    }
  }
}