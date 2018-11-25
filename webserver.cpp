#ifndef WEBSERVER
#define WEBSERVER

#include <iostream>
#include <arpa/inet.h> // struct sockaddr_in
#include <unistd.h> // read(), close()
#include <iostream>

#include "utils.cpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

namespace WebServer {

/**
 * Функция обрабатывает запрос клиента
 * @param _sockfd - дескриптор сокета
 * @return true
 * @onerror возвращаем отрицательное число - код ошибки
 */
int getRequest(int _sockfd) {
    // Адрес клиента
    struct sockaddr_in client_addr = {};
    socklen_t client_addr_length = 0;

    // Получаем дескриптор сокета клиента
    int _client = accept(_sockfd, (struct sockaddr *) &client_addr, &client_addr_length);
    if (_client < 0) {
        cerr << "Не удалось принять соединение клиента" << endl;
        return -1;
    }

    /// TODO: Доделать многопоточность
    /// if (fork() == 0) {
    char request[2000];

    if (recv(_client, (void *) &request, 2000, 0) < 0) {
        cerr << "Не удалось получить сообщение" << endl;
        return -1;
    }
    string response = ""; // Ответ сервера

    // Получаем запрошенный файл
    string filePath = Utils::getFilePath((char *) &request);
    cout << "GET: " << filePath << endl;

    if (!Utils::isFileExist(filePath)) {
        filePath = "./public/404.html";
        response += "HTTP/1.1 404 Not Found\n";
    }else {
        filePath = string("./public/") + filePath;
        response += "HTTP/1.1 200 OK\n";
    }
    response += "Accept-Ranges: bytes"; // TODO: Зачем нужно это
    response += "Content-Length: " + std::to_string(Utils::getContentLength(filePath)) + "\n";
    response += "Content-Type: text/html\n\n";
    response += Utils::getContent(filePath);

    send(_client, response.c_str(), response.size(), 0);
    close(_client);
    /// exit(0);
}

}

#endif //WEVSERVER