#include <iostream>
#include <sgx_urts.h>
#include "SecureDataEnclave_u.h"
#include <windows.h>

// Вспомогательные функции для загрузки анклава
sgx_enclave_id_t create_enclave(const char* enclave_path) {
    sgx_enclave_id_t enclave_id;
    sgx_launch_token_t token = { 0 };
    int updated = 0;

    // Преобразуем путь к анклаву из const char* в LPCWSTR
    int path_len = strlen(enclave_path) + 1;
    wchar_t* wide_path = new wchar_t[path_len];
    MultiByteToWideChar(CP_ACP, 0, enclave_path, -1, wide_path, path_len);

    // Создаем анклав
    sgx_status_t status = sgx_create_enclave(wide_path, SGX_DEBUG_FLAG, &token, &updated, &enclave_id, NULL);
    delete[] wide_path; // Освобождаем память
    if (status != SGX_SUCCESS) {
        std::cerr << "Ошибка при создании анклава!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return enclave_id;
    
}

void destroy_enclave(sgx_enclave_id_t enclave_id) {
    sgx_status_t status = sgx_destroy_enclave(enclave_id);
    if (status != SGX_SUCCESS) {
        std::cerr << "Ошибка при уничтожении анклава!" << std::endl;
    }
}

int main() {
    sgx_enclave_id_t eid;
    sgx_status_t ret = SGX_SUCCESS;
    char outbuffer[256];
    size_t len = sizeof(outbuffer);
    int index;
    
    
    // Укажите путь к вашему анклаву
    const char* enclave_path = "SecureDataEnclave.signed.dll"; // Путь к скомпилированному анклаву

    // Создание анклава
    eid = create_enclave(enclave_path);

    int array_size;
    ret = getArraySize(eid, &array_size);

    // Бесконечный цикл для запроса данных
    while (true) {
        std::cout << "vvedite index massiva (0-" << array_size - 1 << ") ili -1 dlya vihoda: ";
        std::cin >> index;

        // Проверка команды выхода
        if (index == -1) {
            std::cout << "end." << std::endl;
            break;
        }

        // Вызов функции getPD из анклава
        ret = getPD(eid, outbuffer, len, index);
        
        // Вывод полученных данных
        std::cout << "Polychennie dannie: " << outbuffer << std::endl;
    }

    // Закрытие анклава
    destroy_enclave(eid);

    return 0;
}