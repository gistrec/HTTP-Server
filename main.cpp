#include <iostream> // cin, cout

#include <string.h> // strcpy()
#include <unistd.h> // read(), close()
#include <fcntl.h>  // fcntl()
#include <arpa/inet.h> // struct sockaddr_in

using std::cout;
using std::cerr;
using std::endl;

int main() {
    //region @var _sockfd - Create socket [AF_INET, SOCK_DGRAM, IPPROTO_TCP]
    int _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_sockfd < 0) {
        cerr << "Cannot create socket" << endl;
        return -1;
    }
    //endregion

    //region @var addr - Server Address [port: 8080, ip: INADDR_ANY]
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    //endregion

    //region Socket set option SO_REUSEADDR
    int result = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &addr, sizeof(addr));
    if (result < 0) {
        cerr << "Cant set SO_REUSEADDR" << endl;
        return -1;
    }
    //endregion

    //region Bind socket
    result = bind(_sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (result == -1) {
        cerr << "Can't bind address" << endl;
        return -1;
    }
    //endregion

    //region Listen socket
    if (listen(_sockfd, 5) < 0) {
        cerr << "Cannot listem socket" << endl;
    }
    //endregion

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t client_addr_length = 0;

        // Accept new client
        if (int _client = accept(_sockfd, (struct sockaddr *)&client_addr, &client_addr_length)){
            bool connected = true;

            if (_client < 0) {
                cerr << "Connection accept failed" << endl;
                connected = false;
            }
            cout << "Подключен новый клиент" << endl;

            if(fork() == 0){
                char buf[2000];
                ssize_t buf_length = 0;

                // Пока клиент подключен, читаем сообщения
                while (connected) {
                    if (recv(_client, (void *)&buf, 2000, 0) <= 0) {
                        cerr << "Не удалось получить сообщение" << endl;
                        connected = false;
                        break;
                    }

                    cout << buf << endl;
                    memset(buf, '\0', 2000);
                    strcpy(buf, "HTTP/1.1 200 OK\n");
                    strcpy(buf + 16, "Server: CNAI Demo Web Server\n");
                    strcpy(buf + 45, "Content-Length: 0\n");
                    strcpy(buf + 63, "Content-Type: text/html"); // 63 + 23 = 86
                    cout << buf << endl;
                    send(_client, buf, 100, 0);
                    close(_client);
                }
                // ууу, новый поток
                exit(0);
            }
            close(_client);

        }

    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}