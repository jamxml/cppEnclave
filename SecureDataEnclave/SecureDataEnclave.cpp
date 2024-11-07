#include "SecureDataEnclave_t.h" // Заголовочный файл, сгенерированный из EDL
#include <cstring> // Подключаем библиотеку для использования memcpy
#include <sgx_tseal.h> // Основные функции Intel SGX


// Определяем собственную функцию min
inline size_t min(size_t a, size_t b) {
    return (a < b) ? a : b;
}

const int ARRAY_SIZE = 5;
const char* secureData[ARRAY_SIZE] = { "Ivan", "misha", "grisha", "petr", "pobeda" };

void getPD(char* outbuffer, size_t len, const int index) {
    if (index >= 0 && index < ARRAY_SIZE) {
        memcpy(outbuffer, secureData[index], min(len - 1, strlen(secureData[index])));
        outbuffer[min(len - 1, strlen(secureData[index]))] = '\0'; // Завершаем строку нулевым символом
    }
    else {
        const char* error_msg = "Error: Invalid index!";
        memcpy(outbuffer, error_msg, min(len - 1, strlen(error_msg)));
        outbuffer[min(len - 1, strlen(error_msg))] = '\0';
    }
}
int getArraySize() {
    return ARRAY_SIZE;
}