#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Utility.hpp"

class TCPClient {
 private:
  int socketDescriptor_;

 public:
  DISABLE_COPY_AND_MOVE(TCPClient)

  TCPClient() {
    socketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor_ == -1) {
      HandleError("Failed to create socket");
    }
  }

  ~TCPClient() { CloseConnection(); }

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

  size_t Send(const std::string& message) const {
    const auto res = write(socketDescriptor_, message.c_str(), message.length());
    if (res < 0) {
      HandleError("Failed to send data to client");
    }
    return res;
  }

  [[nodiscard]] std::string Receive() const {
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    std::string receivedData;

    if (read(socketDescriptor_, buffer, sizeof(buffer)) < 0) {
      HandleError("Failed to Receive data");
    } else {
      receivedData = buffer;
    }

    return receivedData;
  }

  void UploadFile(const std::string& fileName) {
    const auto [lines, size] = SplitFile(fileName, BUFSIZ);
    Send(std::to_string(size));

    std::hash<std::string> hasher;

    for (const auto& line : lines) {
      size_t hash = hasher(line);

      Send(std::to_string(hash));
      Send(line);
    }
  }

  void ReceiveFile(const std::string& fileName) const {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
      std::cout << "Error opening file!\n";
      return;
    }

    const int bufferSize = 1024;
    char buffer[bufferSize];

    std::cout << "[LOG] : Receiving file...\n";

    // Read and write the file in chunks until there is no more data
    while (true) {
      auto bytesRead = read(socketDescriptor_, buffer, bufferSize);
      if (bytesRead <= 0) {
        break;
      }

      file.write(buffer, bytesRead);
    }

    std::cout << "[LOG] : File received and saved.\n";
  }

  void TransmitFile(const std::string& fileName) const {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
      std::cout << "Error opening file!\n";
      return;
    }

    const int bufferSize = 1024;
    char buffer[bufferSize];

    // Read and send the file in chunks until there is no more data
    while (true) {
      file.read(buffer, bufferSize);
      auto bytesRead = file.gcount();
      if (bytesRead <= 0) {
        break;
      }

      auto bytesSent = send(socketDescriptor_, buffer, bytesRead, 0);
      if (bytesSent <= 0) {
        std::cout << "Error sending data!\n";
        return;
      }
    }

    std::cout << "File transmitted.\n";
  }

  void CloseConnection() const {
    if (shutdown(socketDescriptor_, SHUT_RDWR) < 0) {
      std::cout << "Shutdown error\n" << std::endl;
    }
    if (close(socketDescriptor_) < 0) {
      std::cout << "Close error\n" << std::endl;
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
  std::string data;

  const auto commandHandler = [&](const std::vector<std::string>& splitData) {
    if (splitData.at(0) == EXIT_COMMAND) {
      exit(0);
    }

    if (splitData.size() == 2 && splitData.at(0) == UPLOAD_COMMAND) {
      client.UploadFile(splitData.at(0));
    }
  };

  client.TransmitFile("../TESTFILE");
//  client.ReceiveFile("TESTFILE_R");

//  while (true) {
//    std::cout << "Enter command to the server:\n";
//    std::cin.clear();
//    std::getline(std::cin, data);
//
//    client.Send(data);
//
//    if (data == EXIT_COMMAND) {
//      break;
//    }
//  }

  return 0;
}
