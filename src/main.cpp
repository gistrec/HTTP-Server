#include <iostream> // cin, cout
#include <fstream>
#include <string>

#include "socket.cpp"
#include "webserver.cpp"

int main() {
    // Создаем сокет
    int _sockfd = Socket::createSocket();

    // Обрабатываем запрос
    while (true) WebServer::getRequest(_sockfd);

    return 0;
}

