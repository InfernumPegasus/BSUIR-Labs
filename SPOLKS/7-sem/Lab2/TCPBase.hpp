#ifndef SERVER_TCPBASE_HPP
#define SERVER_TCPBASE_HPP

#include <arpa/inet.h>
#include <fmt/core.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "Utility.hpp"

class TCPBase {
 public:
  long Send(const std::string& message, int socketFd) {
    const auto bytes = send(socketFd, message.c_str(), message.length(), 0);
    if (bytes < 0) {
      HandleError("Failed to send data.");
    }
    return bytes;
//    std::cout << "Sent bytes: " << bytes << std::endl;
  }

  [[nodiscard]] std::string Receive(int socketFd) {
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (recv(socketFd, buffer, sizeof(buffer), 0) < 0) {
      HandleError("Failed to receive data.");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void SendFile(const std::string& fileName, int socketFd) {
    const auto [lines, size] = SplitFile(fileName, BUFSIZ);
    fmt::print("File '{}' size: {} bytes\n", fileName, size);

    Send(std::to_string(size), socketFd);
    long summaryBytes = 0;

    for (const auto& line : lines) {
      summaryBytes += Send(line, socketFd);
    }
    if (size != summaryBytes) {
      std::cout << "size != summaryBytes\n";
      exit(0);
    }
  }

  void ReceiveFile(const std::string& fileName, int socketFd) {
    const auto str = Receive(socketFd);
    long size = std::stol(str);
    std::cout << size << "\n";

    if (size > 0) {
      std::ofstream file(fileName, std::ios::binary);
      if (!file) {
        std::cout << "Error opening file!\n";
        return;
      }

      const int bufferSize = BUFSIZ;
      char buffer[bufferSize];

      // Read and write the file in chunks until there is no more data
      while (true) {
        auto bytesRead = recv(socketFd, buffer, bufferSize, MSG_DONTWAIT);
        if (bytesRead <= 0) {
          break;
        }

        file.write(buffer, bytesRead);
      }

      std::cout << "File '" << fileName << "' received and saved\n";
    } else {
      std::cout << "File cannot be received\n";
    }
  }

 protected:
  static void HandleError(const std::string& errorMessage) {
    std::cerr << errorMessage << " (" << errno << ": " << strerror(errno) << ")"
              << std::endl;
    exit(1);
  }
};

#endif  // SERVER_TCPBASE_HPP
