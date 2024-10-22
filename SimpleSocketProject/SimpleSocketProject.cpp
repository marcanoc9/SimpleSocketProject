#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <dirent.h>

void handleClient(int clientSocket, const std::string& rootDirectory) {
    // Implementation of handleClient function

    // ...
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <root_directory>" << std::endl;
        return 1;
    }

    // Get the port number and root directory from command line arguments
    int port = std::stoi(argv[1]);
    std::string rootDirectory = argv[2];

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Bind the socket to the specified port number
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error listening");
        return 1;
    }

    while (true) {
        // Accept a client connection
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Error accepting client connection");
            return 1;
        }

        // Fork to create a child process
        pid_t childProcess = fork();

        if (childProcess == 0) {
            // In the child process
            // Call handleClient to handle communication with the client
            handleClient(clientSocket, rootDirectory);

            // Close the client socket
            close(clientSocket);

            // Exit the child process
            return 0;
        }

        // In the parent process
        // Close the client socket and continue listening
        close(clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}