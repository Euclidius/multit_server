#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>

#include <fmt/core.h>

#include "server.h"
#include "logger.h"

Logger logger("log.txt");

struct sockaddr_in new_sockaddr(int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    return server_addr;

}

void disconnect(int client_fd, std::string client_id) {
    logger.log(INFO, fmt::format("Client {} disconnected.", client_id));

    close(client_fd);
}

int handle_connection(int server_fd) {
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    ssize_t receive_status;
    std::string input;
    char buf[1024];

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len);
    std::string client_id = inet_ntoa(client_addr.sin_addr);

    logger.log(INFO, fmt::format("Client {} connected.", client_id));

    for (;;) {
        memset((void *) buf, 0, sizeof(buf));
        receive_status = recv(client_fd, buf, 1024, 0);
        *std::remove(buf, buf+strlen(buf), '\n') = '\0';
        input = buf;

        Command command = Command(buf);

        switch (command.com) {
            case EXIT:
                disconnect(client_fd, client_id);
                return 0;
            case ECHO:
                {
                    std::string responce = "Not enough arguments\n";
                    if (command.args.size() == 0) {
                        send(client_fd, responce.c_str(), responce.length(), 0);
                    } else {
                        send(client_fd, command.args[0].c_str(), command.args[0].length(), 0);
                        send(client_fd, "\n", 1, 0);
                    }
                    break;
                }
            case GREET:
                break;
            case HELP:
                break;
            case WHOAMI:
                break;
            case ERR:
                break;
        }

        logger.log(INFO, fmt::format("Client {} sent {}", client_id, buf));
        if (receive_status == -1) {
            logger.log(ERROR, "Data accepting failed.\n");
            return 1;
        } else if (receive_status == 0) {
            disconnect(client_fd, client_id);
            return 0;
        }
    }



    return 0;
}
