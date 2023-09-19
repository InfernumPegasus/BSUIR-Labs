#include <fmt/chrono.h>
#include <fmt/core.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <csignal>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "Utility.hpp"

class TCPServer {
 private:
  int serverSocketDescriptor_;

  int clientSocketDescriptor_{};
  sockaddr_in serverAddress_{};
  sockaddr_in clientAddress_{};

 public:
  explicit TCPServer(int port) {
    serverSocketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketDescriptor_ == -1) {
      handleError("Failed to create server socket");
    }

    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_addr.s_addr = INADDR_ANY;
    serverAddress_.sin_port = htons(port);

    if (bind(serverSocketDescriptor_, (struct sockaddr*)&serverAddress_,
             sizeof(serverAddress_)) < 0) {
      handleError("Failed to bind server socket");
    }

    if (listen(serverSocketDescriptor_, 3) < 0) {
      handleError("Failed to listen on server socket");
    }

    std::cout << "Server started on port " << port << std::endl;
  }

  ~TCPServer() { stopServer(); }

  void acceptConnection() {
    socklen_t clientAddressLength = sizeof(clientAddress_);
    clientSocketDescriptor_ = accept(
        serverSocketDescriptor_, (struct sockaddr*)&clientAddress_, &clientAddressLength);
    if (clientSocketDescriptor_ < 0) {
      handleError("Failed to accept client connection");
    }
  }

  [[nodiscard]] std::string receive() const {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (recv(clientSocketDescriptor_, buffer, 4096, 0) < 0) {
      handleError("Failed to receive data from client");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void send(const std::string& message) const {
    if (write(clientSocketDescriptor_, message.c_str(), message.length()) < 0) {
      handleError("Failed to send data to client");
    }
  }

  void closeConnection() const { close(clientSocketDescriptor_); }

  void stopServer() const { close(serverSocketDescriptor_); }

 private:
  static void handleError(const std::string& errorMessage) {
    std::cerr << errorMessage << std::endl;
    exit(1);
  }
};

int main() {
  TCPServer server(8080);
  std::string receivedData;
  server.acceptConnection();

  while (true) {
    receivedData = server.receive();
    auto splitData = SplitString(receivedData, ' ');

    if (splitData[0] == EXIT_COMMAND) {
      break;
    }

    if (splitData.at(0) == TIME_COMMAND) {
      auto t = std::chrono::system_clock::now();
      fmt::print("Current server time: {}\n", t);
    }

    if (splitData.size() > 1) {
      if (splitData.at(0) == ECHO_COMMAND) {
        std::cout << "Received data from client command:" << std::endl;
        for (size_t i = 1; i < splitData.size(); i++) {
          std::cout << splitData.at(i) << " ";
        }
        std::cout << std::endl;
      }
    }
  }

  server.closeConnection();
  server.stopServer();

  return 0;
}
