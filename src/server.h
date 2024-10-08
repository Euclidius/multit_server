#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#include <iostream>

const int MAX_CLIENTS = 5;
const int CONNECTION_BACKLOG = 5;
const int PORT = 12345;

struct sockaddr_in new_sockaddr(int port);

int handle_connection(int server_fd);

enum Commands {
    ECHO,
    GREET,
    WHOAMI,
    HELP,
    EXIT,
    ERR,
};

struct Command {
    Commands com;
    std::vector<std::string> args;

    Command(std::string input) {
        std::stringstream f(input);

        /* std::vector<std::string> args; */
        std::string tmp_command;

        std::getline(f, tmp_command, ' ');
        if (tmp_command == std::string("echo")) {
            com = Commands::ECHO;
        } else if (tmp_command == std::string("greet")) {
            com = Commands::GREET;
        } else if (tmp_command == std::string("whoami")) {
            com = Commands::WHOAMI;
        } else if (tmp_command == std::string("help")) {
            com = Commands::HELP;
        } else if (tmp_command == std::string("exit")) {
            com = Commands::EXIT;
        } else {
            com = Commands::ERR;
        }

        while (getline(f, tmp_command, ' ')) {
            args.push_back(tmp_command);
        }
    }
};
