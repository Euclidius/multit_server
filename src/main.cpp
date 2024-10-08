#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

#include <fmt/core.h>

#include "server.h"
#include "logger.h"

int main(int argc, char **argv) {
    struct sockaddr_in server_addr = new_sockaddr(PORT);
    std::vector<std::thread> clients;

    Logger logger("log.txt");

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        logger.log(ERROR, "Failed to create server socket.\n");
        return 1;
    }

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        logger.log(ERROR, fmt::format("Failed to bind to port {}.", PORT));
        return 1;
    }

    if (listen(server_fd, CONNECTION_BACKLOG) != 0) {
        logger.log(ERROR, "Listen failed.\n");
        return 1;
    }

    logger.log(INFO, "Waiting for a client to connect...\n");
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients.push_back(std::thread(handle_connection, server_fd));
    }

    for (auto &i:clients) {
        i.join();
    }

    logger.log(INFO, "Exiting");
    close(server_fd);

    return 0;
}
