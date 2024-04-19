#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Encoding
int manchesterEncode(const uint8_t *data, int dataSize, uint8_t *encodedData)
{
    int encodedIndex = 0;
    for (int i = 0; i < dataSize; i++)
    {
        if ((data[i] & 1) == 0) // High to low transition for 0
        {
            encodedData[encodedIndex++] = 0;
            encodedData[encodedIndex++] = 1;
        }
        else if ((data[i] & 1) == 1) // Low to high transition for 1
        {
            encodedData[encodedIndex++] = 1;
            encodedData[encodedIndex++] = 0;
        }
        else
        {
            printf("Error: Invalid data.\n");
            return 1;
        }
    }
    return 0;
}

// Decoding
int manchesterDecode(uint8_t *encodedData, int dataSize, uint8_t *decodedData)
{
    int decodedIndex = 0;
    for (int i = 0; i < dataSize; i += 2)
    {
        if (encodedData[i] == 0 && encodedData[i + 1] == 1) // Bit 0 detected
        {
            decodedData[decodedIndex++] = 0;
        }
        else if (encodedData[i] == 1 && encodedData[i + 1] == 0) // Bit 1 detected
        {
            decodedData[decodedIndex++] = 1;
        }
        else
        {
            printf("Error: Invalid Manchester encoding sequence.\n");
            return 1;
        }
    }
    return 0;
}