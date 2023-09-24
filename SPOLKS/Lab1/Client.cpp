#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#include "Utility.hpp"

class TCPClient {
 private:
  int socketDescriptor_;

 public:
  TCPClient() {
    socketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor_ == -1) {
      HandleError("Failed to create socket");
    }
  }

  TCPClient(const TCPClient&) = delete;

  TCPClient(TCPClient&&) = delete;

  TCPClient& operator=(const TCPClient&) = delete;

  TCPClient& operator=(TCPClient&&) = delete;

  void ConnectToServer(const std::string& ipAddress, int port) const {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, ipAddress.c_str(), &(serverAddress.sin_addr)) <= 0) {
      HandleError("Invalid address/ Address not supported");
    }

    if (connect(socketDescriptor_, (struct sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0) {
      HandleError("Connection failed");
    }
  }

  void Send(const std::string& message) const {
    if (write(socketDescriptor_, message.c_str(), message.length()) < 0) {
      HandleError("Failed to Send message");
    }
  }

  [[nodiscard]] std::string Receive() const {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (read(socketDescriptor_, buffer, sizeof(buffer)) < 0) {
      HandleError("Failed to Receive data");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void CloseConnection() const {
    if (shutdown(socketDescriptor_, SHUT_RDWR) < 0) {
      HandleError("Failed to close connection");
    }
  }

 private:
  static void HandleError(const std::string& errorMessage) {
    std::cerr << errorMessage << " (" << errno << ": " << strerror(errno) << ")"
              << std::endl;
    exit(1);
  }
};

int main(int argc, char** argv) {
  std::string ip;
  int port;
  if (argc == 3) {
    ip = argv[1];
    port = std::stoi(argv[2]);
  } else {
    ip = DEFAULT_IP;
    port = DEFAULT_PORT;
  }

  TCPClient client;
  client.ConnectToServer(ip, port);
  //  client.Send("Hello, server!");
  std::string data;

  while (true) {
    std::cout << "Enter command to the server:\n";
    std::cin.clear();
    std::getline(std::cin, data);

    client.Send(data);

    if (data == EXIT_COMMAND) {
      client.CloseConnection();
      break;
    }
  }

  return 0;
}
