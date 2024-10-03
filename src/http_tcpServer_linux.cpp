#include "http_tcpServer_linux.h"
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

std::string create_http_response(const std::string &content)
{
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "\r\n"
             << content;
    return response.str();
}
namespace http
{
    TcpServer::TcpServer()
    {

        TcpServer::StartServer();
    }
    TcpServer::~TcpServer()
    {
    }
    int TcpServer::StartServer()
    {
        int socketReturnValue = socket(AF_INET, SOCK_STREAM, 0);
        if (socketReturnValue < 0)
        {
            std::cout << "can't create socket" << std::endl;
        }

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(8080);
        address.sin_addr.s_addr = INADDR_ANY;
        bind(socketReturnValue, (struct sockaddr *)&address, sizeof(address));
        if (listen(socketReturnValue, 3) < 0)
        {
            std::cout << "socket field";
            exit(EXIT_FAILURE);
        }
        std::cout << "socket create it successfully" << std::endl;
        while (true)
        { // accepting new connection
            std::cout << "==========waiting req============" << std::endl;
            char buffer[1024] = {0};
            struct sockaddr_in client_addr;
            socklen_t clien_addr_len = sizeof(client_addr);
            int new_socket = accept(socketReturnValue, (struct sockaddr *)&client_addr, &clien_addr_len);
            std::cout << new_socket << std::endl;
            if (new_socket < 0)
            {
                std::cout << "accept faild" << std::endl;
                continue;
            }
            ssize_t bytes_read = read(new_socket, buffer, 1024);
            if (bytes_read < 0)
            {
                perror("read failed");
                close(new_socket);
                continue;
            }

            std::cout << "Received request:\n"
                      << buffer << std::endl;

            // Creating and sending the response
            std::string content = "<html><body><h1>Hello, World!</h1></body></html>";
            std::string response = create_http_response(content);
            std::cout << send(new_socket, response.c_str(), response.length(), 0);

            close(new_socket);
        }
        return 0;
    }
    void TcpServer::CloseServer()
    {
        close(socketReturnValue);
        exit(0);
    }

} // namespace http
