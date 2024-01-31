#include <functional>
#include <iostream>

#include "TCPBase.hpp"
#include "Utility.hpp"

class TCPClient : public TCPBase {
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

  void CloseConnection() const {
    if (shutdown(socketDescriptor_, SHUT_RDWR) < 0) {
      std::cout << "Shutdown error\n" << std::endl;
    }
    if (close(socketDescriptor_) < 0) {
      std::cout << "Close error\n" << std::endl;
    }
  }

  [[nodiscard]] int ClientSocket() const { return socketDescriptor_; }
};

[[noreturn]] int main(int argc, char** argv) {
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

  const auto handleConnection = [&]() {
    std::cout << "Enter your name: ";
    std::getline(std::cin, data);
    client.Send(data, client.ClientSocket());
    data = client.Receive(client.ClientSocket());
    std::cout << data << std::endl;
    //    data = client.Receive(client.ClientSocket());
    //    std::cout << data << std::endl;

    //    if (data != STRING_NO_PREVIOUS_SESSIONS) {
    //      std::getline(std::cin, data);
    //      client.Send(data, client.ClientSocket());
    //      if (data == "yes") {
    //        client.ReceiveFile(client.Receive(client.ClientSocket()),
    // client.ClientSocket());       }    }
  };

  handleConnection();

  const auto commandHandler = [&](const std::vector<std::string>& splitData) {
    if (splitData.at(0) == EXIT_COMMAND || splitData.at(0) == EXIT_ALL_COMMAND) {
      exit(0);
    }

    if (splitData.at(0) == TIME_COMMAND) {
      const auto timeString = client.Receive(client.ClientSocket());
      std::cout << timeString << std::endl;
    }

    // Send file
    if (splitData.size() == 2 && splitData.at(0) == UPLOAD_COMMAND) {
      client.SendFile(splitData.at(1), client.ClientSocket());
    }

    // Receive file
    if (splitData.size() == 2 && splitData.at(0) == DOWNLOAD_COMMAND) {
      client.ReceiveFile(splitData[1] + "_client_received", client.ClientSocket());
    }
  };

  while (true) {
    std::cout << "Enter command to the server:\n";
    std::cin.clear();
    std::getline(std::cin, data);
    auto splitData = SplitString(data, ' ');

    client.Send(data, client.ClientSocket());

    commandHandler(splitData);
  }
}
