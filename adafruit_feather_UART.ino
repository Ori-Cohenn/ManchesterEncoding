#include "Manchester_Encoding.c"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define BufferSize 10 // 10 bytes buffer
// 1/9600= 104.16 microseconds
int stringIndex = 0;
char originalBits[] = {1, 0};
char* binaryData;
char* hexData;
int dataSize = 0;
char *stringSend;
char *stringReceived;
void setup()
{
  Serial.begin(9600);  // Initialize the Serial monitor for debugging
  //Serial1.begin(9600); // Initialize Serial1 for sending data
  dataSize = sizeof(originalBits) / sizeof(originalBits[0]);
  pinMode(2, OUTPUT); // GPIO pin 2 as output
  pinMode(3, INPUT);  // GPIO pin 3 as input
  stringSend = (char *)malloc(dataSize * 2);
  stringReceived = (char *)malloc(BufferSize);
}

// void readUart(void)
// {
//   while ((Serial1.available()) || stringSend[stringIndex] == 13) // 13 is the ASCII value of carriage return
//   {
//     stringSend[stringIndex] = Serial1.read();
//     stringIndex++;
//   }
//   Serial.print("\nManchester Encoded Data Received: ");
//   Serial.println(stringSend);
//   manchesterDecode(stringSend, BufferSize, stringReceived);
//   Serial.print("\nManchester Decoded Data: ");
//   Serial.println(stringReceived);
//   stringIndex = 0;
//   memset(stringSend, 0, BufferSize);     // clear buffer
//   memset(stringReceived, 0, BufferSize); // clear buffer
// }

// void writeUart(void)
// {
//   manchesterEncode(originalBits, dataSize, stringSend);
//   Serial.println("\nManchester Encoded Data: ");
//   for (int i = 0; i < dataSize * 2; i++)
//   {
//     Serial.print(stringSend[i]);
//   }
//   Serial1.write(stringSend, dataSize * 2);
//   memset(stringSend, 0, dataSize * 2); // clear buffer
// }

//idle state is high and the start bit is low for GPIO pins implementation
void idleState(void)
{
  digitalWrite(2, HIGH);
  delayMicroseconds(104);
}
//start condition for GPIO pins change to read low
void startCondition(void)
{
  digitalWrite(2, LOW);
  delayMicroseconds(104);
}
//stop condition for GPIO pins change to read high

//Transmitting data over GPIO pins
void transmitData(void)
{
  //if data is in hexadecimal format then convert it to binary with hexToBinary function
  if(originalBits[0] != '0' || originalBits[0] != '1')
  {
    binaryData = (char *)malloc(dataSize * 4);
    hexToBinary(originalBits, binaryData);
    for (int i = 0; i < dataSize; i++)
    {
      originalBits[i] = binaryData[i];
    }
  }
  manchesterEncode(originalBits, dataSize, stringSend);
  Serial.println("\nManchester Encoded Data: ");
  for (int i = 0; i < dataSize * 2; i++)
  {
    Serial.print(stringSend[i]);
  }
  for (int i = 0; i < dataSize * 2; i++)
  {
    if (stringSend[i] == '0')
    {
      digitalWrite(2, HIGH);
      delayMicroseconds(104);
      digitalWrite(2, LOW);
      delayMicroseconds(104);
    }
    else if (stringSend[i] == '1')
    {
      digitalWrite(2, LOW);
      delayMicroseconds(104);
      digitalWrite(2, HIGH);
      delayMicroseconds(104);
    }
  }
  memset(stringSend, 0, dataSize * 2); // clear buffer
}
//Receiving data over GPIO pins
void receiveData(void)
{
  for (int i = 0; i < dataSize * 2; i++)
  {
    if (digitalRead(3) == HIGH)
    {
      stringReceived[i] = '0';
    }
    else if (digitalRead(3) == LOW)
    {
      stringReceived[i] = '1';
    }
    delayMicroseconds(104);
  }
  binaryToHex(stringReceived, hexData);
  hexData= (char *)malloc(dataSize);
  Serial.print("\nManchester Decoded Data: ");
  Serial.println(stringReceived);
  memset(stringReceived, 0, dataSize * 2); // clear buffer
}

//hexadecimal to binary conversion
void hexToBinary(const char *hex, char *binary)
{
  int i = 0;
  while (hex[i])
  {
    switch (hex[i])
    {
    case '0':
      strcat(binary, "0000");
      break;
    case '1':
      strcat(binary, "0001");
      break;
    case '2':
      strcat(binary, "0010");
      break;
    case '3':
      strcat(binary, "0011");
      break;
    case '4':
      strcat(binary, "0100");
      break;
    case '5':
      strcat(binary, "0101");
      break;
    case '6':
      strcat(binary, "0110");
      break;
    case '7':
      strcat(binary, "0111");
      break;
    case '8':
      strcat(binary, "1000");
      break;
    case '9':
      strcat(binary, "1001");
      break;
    case 'A':
    case 'a':
      strcat(binary, "1010");
      break;
    case 'B':
    case 'b':
      strcat(binary, "1011");
      break;
    case 'C':
    case 'c':
      strcat(binary, "1100");
      break;
    case 'D':
    case 'd':
      strcat(binary, "1101");
      break;
    case 'E':
    case 'e':
      strcat(binary, "1110");
      break;
    case 'F':
    case 'f':
      strcat(binary, "1111");
      break;
    default:
      Serial.print("\nInvalid hexadecimal digit ");
    }
    i++;
  }
}

void binaryToHex(const char *binary, char *hex)
{
  int i = 0;
  while (binary[i])
  {
    switch (binary[i])
    {
    case '0000':
      strcat(hex, "0");
      break;
    case '0001':
      strcat(hex, "1");
      break;
    case '0010':
      strcat(hex, "2");
      break;
    case '0011':
      strcat(hex, "3");
      break;
    case '0100':
      strcat(hex, "4");
      break;
    case '0101':
      strcat(hex, "5");
      break;
    case '0110':
      strcat(hex, "6");
      break;
    case '0111':
      strcat(hex, "7");
      break;
    case '1000':
      strcat(hex, "8");
      break;
    case '1001':
      strcat(hex, "9");
      break;
    case '1010':
      strcat(hex, "A");
      break;
    case '1011':
      strcat(hex, "B");
      break;
    case '1100':
      strcat(hex, "C");
      break;
    case '1101':
      strcat(hex, "D");
      break;
    case '1110':
      strcat(hex, "E");
      break;
    case '1111':
      strcat(hex, "F");
      break;
    default:
      Serial.print("\nInvalid binary digit ");
    }
    i++;
  }
}

void loop()
{
  Serial.println("\nOriginal Bits: ");
  for (int i = 0; i < dataSize; i++)
  {
    Serial.print(originalBits[i], DEC);
  }
  idleState();
  transmitData();
  //receiveData();
  //writeUart();
  //readUart();
  delay(1000);
}