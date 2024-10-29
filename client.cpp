#include <iostream>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void run_client(int client_id) {
    struct addrinfo hints, *servinfo;
    int sockfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("127.0.0.1", "3000", &hints, &servinfo) != 0) {
        std::cerr << "Client " << client_id << ": Error in getaddrinfo" << std::endl;
        return;
    }

    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1) {
        std::cerr << "Client " << client_id << ": Socket creation failed" << std::endl;
        freeaddrinfo(servinfo);
        return;
    }

    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        std::cerr << "Client " << client_id << ": Connection failed" << std::endl;
        close(sockfd);
        freeaddrinfo(servinfo);
        return;
    }

    freeaddrinfo(servinfo);

    const char *request = "GET /Downloads/1337_transcripts.pdf HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request, strlen(request), 0) == -1) {
        std::cerr << "Client " << client_id << ": Sending request failed" << std::endl;
        close(sockfd);
        return;
    }

    char buffer[4096];
    int bytes_received;
    std::cout << "Client " << client_id << " received:\n";
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        // std::cout << buffer;
    }

    if (bytes_received == -1) {
        std::cerr << "Client " << client_id << ": Receiving response failed" << std::endl;
    }

    close(sockfd);
    std::cout << "\nClient " << client_id << " finished.\n";
}

int main() {
    const int num_clients = 1000;
    int currentMax = 100;  // Set the number of clients to spawn
    std::vector<std::thread> clients;

    int count = 0;
    while (count++ < num_clients) {
        for (int i = 0; i < currentMax; i++) {
            clients.emplace_back(run_client, i + 1 + count * currentMax);
        }

        // Join all threads
        for (auto &client : clients) {
            client.join();
        }
        clients.clear();
    }

    return 0;
}
