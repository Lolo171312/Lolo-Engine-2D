#include "Helpers.h"

// Determina si el sistema es Big Endian
bool IsSystemBigEndian()
{
    int testValue = 1;
    // Si el primer byte de `testValue` es 0 es Big Endian si no, es Little Endian
    return !((reinterpret_cast<char*>(&testValue))[0]);
}

// Convierte un array de bytes en un entero según el formato Endian del sistema
int ConvertBytesToInt(const char* buffer, int byteCount)
{
    int result = 0;

    if (!IsSystemBigEndian())
    {
        //Los bytes del buffer se copian en el orden original
        for (int i = 0; i < byteCount; i++)
        {
            (reinterpret_cast<char*>(&result))[i] = buffer[i];
        }
    }
    else
    {
        //Los bytes se copian en orden inverso para que el resultado sea correcto
        for (int i = 0; i < byteCount; i++)
        {
            (reinterpret_cast<char*>(&result))[3 - i] = buffer[i];
        }
    }
    return result;
}
