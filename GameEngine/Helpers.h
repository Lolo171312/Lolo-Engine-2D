#pragma once

// Determina si el sistema es Big Endian
bool IsSystemBigEndian();

// Convierte un array de bytes en un entero según el formato Endian del sistema
int ConvertBytesToInt(const char* buffer, int byteCount);

/*
* Generates a random integer between 0 and maxInteger
*/
int RandomInt(int maxInteger);

/*
* Generates a random integer in range
*/
int RandomInt(int minInteger, int maxInteger);