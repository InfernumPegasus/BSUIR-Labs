#include <fmt/chrono.h>
#include <fmt/core.h>

#include "TCPBase.h"
#include "Utility.hpp"

class TCPServer : public TCPBase {
 private:
  int serverSocketDescriptor_;

  int clientSocketDescriptor_{};
  sockaddr_in serverAddress_{};
  sockaddr_in clientAddress_{};

 public:
  DISABLE_COPY_AND_MOVE(TCPServer)

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

  //  void UploadFile(const std::string& fileName) const {
  //    std::ifstream file(fileName, std::ios::binary);
  //    if (!file) {
  //      std::cout << "Error opening file!\n";
  //      return;
  //    }
  //
  //    const int bufferSize = BUFSIZ;
  //    char buffer[bufferSize];
  //
  //    // Read and send the file in chunks until there is no more data
  //    while (true) {
  //      file.read(buffer, bufferSize);
  //      auto bytesRead = file.gcount();
  //      if (bytesRead <= 0) {
  //        break;
  //      }
  //
  //      auto bytesSent = send(clientSocketDescriptor_, buffer, bytesRead, 0);
  //      if (bytesSent <= 0) {
  //        std::cout << "Error sending data!\n";
  //        return;
  //      }
  //    }
  //
  //    std::cout << "File '" << fileName << "' transmitted.\n";
  //  }

  //  void ReceiveFile(const std::string& fileName) const {
  //    std::ofstream file(fileName, std::ios::binary);
  //    if (!file) {
  //      std::cout << "Error opening file!\n";
  //      return;
  //    }
  //
  //    const int bufferSize = BUFSIZ;
  //    char buffer[bufferSize];
  //
  //    std::cout << "[LOG] : Receiving file...\n";
  //
  //    // Read and write the file in chunks until there is no more data
  //    while (true) {
  //      auto bytesRead = recv(clientSocketDescriptor_, buffer, bufferSize,
  //      MSG_DONTWAIT); if (bytesRead <= 0) {
  //        break;
  //      }
  //
  //      file.write(buffer, bytesRead);
  //    }
  //
  //    std::cout << "File '" << fileName << "' received and saved.\n";
  //  }

  void CloseConnection() const {
    if (close(serverSocketDescriptor_) < 0) {
      fmt::print("Server socket {} cannot be closed\n", serverSocketDescriptor_);
    }
  }

  void Stop() const {
    if (shutdown(serverSocketDescriptor_, SHUT_RDWR) < 0) {
      fmt::print("Shutdown error\n");
    }
    CloseConnection();
  }

  [[nodiscard]] int ClientSocket() const { return clientSocketDescriptor_; }
};

[[noreturn]] int main(int argc, char** argv) {
  int port = (argc == 2) ? std::stoi(argv[1]) : DEFAULT_PORT;

  TCPServer server(port);
  std::string receivedData;

  const auto commandHandler = [&](const std::vector<std::string>& splitData) {
    if (splitData.at(0) == EXIT_COMMAND) {
      exit(0);
    }

    if (splitData.at(0) == TIME_COMMAND) {
      auto t = std::chrono::system_clock::now();
      const auto timeString = fmt::format("Current server time: {}", t);
      server.Send(timeString, server.ClientSocket());
    }

    if (splitData.size() > 1 && splitData.at(0) == ECHO_COMMAND) {
      fmt::print("[{}] Received data from client command:\n", ECHO_COMMAND);
      for (size_t i = 1; i < splitData.size(); i++) {
        std::cout << splitData.at(i) << " ";
      }
      std::cout << std::endl;
    }

    if (splitData.size() == 2 && splitData.at(0) == UPLOAD_COMMAND) {
      server.ReceiveFile(splitData[1] + "_server_received", server.ClientSocket());
    }

    if (splitData.size() == 2 && splitData.at(0) == DOWNLOAD_COMMAND) {
      server.UploadFile(splitData[1], server.ClientSocket());
    }
  };

  server.AcceptConnection();

  while (true) {
    receivedData = server.Receive(server.ClientSocket());
    auto splitData = SplitString(receivedData, ' ');

    commandHandler(splitData);
  }
}
