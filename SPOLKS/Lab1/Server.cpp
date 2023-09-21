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
      HandleError("Failed to create server socket");
    }

    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_addr.s_addr = INADDR_ANY;
    serverAddress_.sin_port = htons(port);

    if (bind(serverSocketDescriptor_, (struct sockaddr*)&serverAddress_,
             sizeof(serverAddress_)) < 0) {
      HandleError("Failed to bind server socket");
    }

    if (listen(serverSocketDescriptor_, 3) < 0) {
      HandleError("Failed to listen on server socket");
    }

    std::cout << "Server started on port " << port << std::endl;
  }

  ~TCPServer() {
    std::cout << "Closing server..." << std::endl;
    Stop();
    std::cout << "Server closed.\n";
  }

  TCPServer(const TCPServer&) = delete;

  TCPServer(TCPServer&&) = delete;

  TCPServer& operator=(const TCPServer&) = delete;

  TCPServer& operator=(TCPServer&&) = delete;

  void AcceptConnection() {
    socklen_t clientAddressLength = sizeof(clientAddress_);
    clientSocketDescriptor_ = accept(
        serverSocketDescriptor_, (struct sockaddr*)&clientAddress_, &clientAddressLength);
    if (clientSocketDescriptor_ < 0) {
      HandleError("Failed to accept client connection");
    } else {
      fmt::print("Client {} connected to server\n", clientSocketDescriptor_);
    }
  }

  [[nodiscard]] std::string Receive() const {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (recv(clientSocketDescriptor_, buffer, 4096, 0) < 0) {
      HandleError("Failed to Receive data from client");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void Send(const std::string& message) const {
    if (write(clientSocketDescriptor_, message.c_str(), message.length()) < 0) {
      HandleError("Failed to Send data to client");
    }
  }

  void CloseConnection() const { close(clientSocketDescriptor_); }

  void Stop() const {
    if (shutdown(serverSocketDescriptor_, SHUT_RDWR) < 0) {
      fmt::print("Server socket {} cannot be closed\n", serverSocketDescriptor_);
    }
  }

 private:
  static void HandleError(const std::string& errorMessage) {
    std::cerr << errorMessage << std::endl;
    exit(1);
  }
};

int main(int argc, char** argv) {
  int port = (argc == 2) ? std::stoi(argv[1]) : DEFAULT_PORT;

  TCPServer server(port);
  std::string receivedData;
  server.AcceptConnection();

  while (true) {
    receivedData = server.Receive();
    auto splitData = SplitString(receivedData, ' ');

    if (splitData.at(0) == EXIT_COMMAND) {
      break;
    }

    if (splitData.at(0) == TIME_COMMAND) {
      auto t = std::chrono::system_clock::now();
      fmt::print("Current server time: {}\n", t);
    }

    if (splitData.size() > 1 && splitData.at(0) == ECHO_COMMAND) {
      std::cout << "Received data from client command:" << std::endl;
      for (size_t i = 1; i < splitData.size(); i++) {
        std::cout << splitData.at(i) << " ";
      }
      std::cout << std::endl;
    }
  }

  server.CloseConnection();

  return 0;
}
