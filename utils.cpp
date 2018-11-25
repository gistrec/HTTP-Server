#include <iterator> // istream_iterator
#include <cstring> // strchr
#include <fstream>
#include <string>

using std::string;


namespace Utils {

/**
 * Функция нужна для получения пути файла из HTTP запроса
 * @param httpHeader - указатель на строку HTTP запроса
 * @return путь до файла
 */
string getFilePath(char *httpHeader) {
    char *pos = strchr(httpHeader + 5, ' ');
    string result(httpHeader + 5, pos - httpHeader - 5);
    return result;
}

/**
 * Функция нужна для получения типа данных
 * @param filePath - путь до файла
 * @return тип данных (например, text/html)
 */
string getContentType(const string &filePath) {
    string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
    if (fileExtension == "html") return string("text/html");
    if (fileExtension == "iso") return string("image/x-icon");
}

/**
 * Функция нужна для получения размера данных
 * @param filePath - путь до файла
 * @return размер файла
 */
size_t getContentLength(const string &filePath) {
        std::ifstream file(filePath, std::ios::in);

        std::streampos fsize = file.tellg(); // Указатель на начало файла
        file.seekg(0, std::ios::end);        // Перемещаем указатель на конец файла

        fsize = file.tellg() - fsize; // Разность указателей
        file.close();

        return (size_t) fsize;
}

/**
 * Функция нужна для получения содержимого файла
 * @param filePath - путь до файла
 * @return содержимое файла
 */
string getContent(const string &filePath) {
    string fileContent;
    std::ifstream file(filePath);

    std::istream_iterator<char> start(file), end;
        fileContent.insert(fileContent.begin(), start, end);

    return fileContent;
}

};
