#include <arpa/inet.h>
#include <fmt/core.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>
#include <thread>

#include "Utility.hpp"

class TCPSocket {
 private:
  int socketDescriptor_;

 public:
  TCPSocket() {
    socketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor_ == -1) {
      handleError("Failed to create socket");
    }
  }

  void connectToServer(const std::string& ipAddress, int port) const {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, ipAddress.c_str(), &(serverAddress.sin_addr)) <= 0) {
      handleError("Invalid address/ Address not supported");
    }

    if (connect(socketDescriptor_, (struct sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0) {
      handleError("Connection failed");
    }
  }

  void send(const std::string& message) const {
    if (write(socketDescriptor_, message.c_str(), message.length()) < 0) {
      handleError("Failed to send message");
    }
  }

  [[nodiscard]] std::string receive() const {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (read(socketDescriptor_, buffer, sizeof(buffer)) < 0) {
      handleError("Failed to receive data");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void closeConnection() const {
    if (close(socketDescriptor_) < 0) {
      handleError("Failed to close connection");
    }
  }

 private:
  static void handleError(const std::string& errorMessage) {
    std::cerr << errorMessage << " (" << errno << ": " << strerror(errno) << ")"
              << std::endl;
    exit(1);
  }
};

int main() {
  TCPSocket clientSocket;
  clientSocket.connectToServer("127.0.0.1", 8080);
  clientSocket.send("Hello, server!");
  std::string data;

  while (true) {
    std::cout << "Send data to the server:\n";
    std::cin.clear();
    std::getline(std::cin, data);

    clientSocket.send(data);

    if (data == EXIT_COMMAND) {
      clientSocket.closeConnection();
      break;
    }
  }

  return 0;
}
