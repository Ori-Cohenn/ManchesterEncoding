#include <stdio.h>
#include <stdlib.h>
#include "Manchester_Encoding.h"

// Function to perform Manchester encoding
void manchesterEncode(const unsigned char *data, int dataSize, char *encodedData)
{
    int encodedIndex = 0;
    for (int i = 0; i < dataSize; i++)
    {
        if ((data[i] & 1) == 0)
        {
            encodedData[encodedIndex++] = '0'; // High to low transition for '0'
            encodedData[encodedIndex++] = '1';
        }
        else
        {
            encodedData[encodedIndex++] = '1'; // Low to high transition for '1'
            encodedData[encodedIndex++] = '0';
        }
    }
}

// Function to perform Manchester decoding
void manchesterDecode(const char *encodedData, int dataSize, char *decodedData)
{
    int decodedIndex = 0;
    for (int i = 0; i < dataSize; i += 2)
    {
        if (encodedData[i] == '0' && encodedData[i + 1] == '1')
        {
            decodedData[decodedIndex++] = 0; // Bit '0' detected
        }
        else if (encodedData[i] == '1' && encodedData[i + 1] == '0')
        {
            decodedData[decodedIndex++] = 1; // Bit '1' detected
        }
        else
        {
            printf("Error: Invalid Manchester encoding sequence.\n");
            return;
        }
    }
}