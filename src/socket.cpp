#ifndef SOCKET
#define SOCKET

#include <arpa/inet.h> // struct sockaddr_in
#include <iostream>

using std::cerr;
using std::endl;

namespace Socket {

/**
 * Функция нужна для создания сокета
 * @param port - порт веб сервера
 * @return дескриптор сокета
 * @onerror возвращаем отрицательное число - код ошибки
 */
int createSocket(int port = 8080) {
    //region @var _sockfd - Создание сокета [AF_INET, SOCK_DGRAM, IPPROTO_TCP]
    int _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_sockfd < 0) {
        cerr << "Не удалось создать сокет" << endl;
        return -1;
    }
    //endregion

    //region @var addr - Создание адреса веб сервера  [port: port, ip: INADDR_ANY]
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    //endregion

    //region Установка сокету флага SO_REUSEADDR
    int result = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &addr, sizeof(addr));
    if (result < 0) {
        cerr << "Не удалось установить сокету флаг SO_REUSEADDR" << endl;
        return -1;
    }
    //endregion

    //region Бинд сокета
    result = bind(_sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (result == -1) {
        cerr << "Не удалось назначить адрес сокету" << endl;
        return -1;
    }
    //endregion

    //region Переводим сокет в режим ожидания соединения
    if (listen(_sockfd, 5) < 0) {
        cerr << "Cannot listem socket" << endl;
        return -1;
    }
    //endregion

    return _sockfd;
}

}

#endif //SOCKET