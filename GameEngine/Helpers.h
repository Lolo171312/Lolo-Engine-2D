#pragma once

// Determina si el sistema es Big Endian
bool IsSystemBigEndian();

// Convierte un array de bytes en un entero según el formato Endian del sistema
int ConvertBytesToInt(const char* buffer, int byteCount);