#include <stdio.h>
#ifndef Manchester_Encoding
#define Manchester_Encoding

void manchesterEncode(const unsigned char *data, int dataSize, char *encodedData);
void manchesterDecode(const char *encodedData, int dataSize, char *decodedData);

#endif
