#include "server.hpp"
#include "asio.hpp"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using asio::ip::tcp;

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {};

Server::Server(asio::io_context &ctx, short port)
    : acceptor_(ctx, tcp::endpoint(tcp::v4(), port)) {
  accept();
};

std::optional<std::vector<std::unique_ptr<RESPType>>> Session::session_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(data_, max_length),
      [this, self](std::error_code err, std::size_t length) {
        if (!err) {
          spdlog::info("Read {} bytes from buff", length);
          // parse
          // return vector of commands to serve
          return;
        }
        spdlog::error("Error when reading from buffer");
      });
  return {};
};

void Session::session_write(std::size_t length) {
  auto self(shared_from_this());
  asio::async_write(socket_, asio::buffer(response_, length),
                    [this, self](std::error_code err, std::size_t) {
                      if (!err) {
                      }
                      spdlog::error("Error when reading from buffer");
                    });
};

void Session::serve() { session_read(); };

void Server::accept() {
  acceptor_.async_accept([this](std::error_code err, tcp::socket socket) {
    if (!err) {
      std::make_shared<Session>(std::move(socket))->serve();
    }
    accept();
  });
}

int init_server() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(6379);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "Failed to bind to port 6379\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  std::cout << "Waiting for a client to connect...\n";

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here!\n";

  auto client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                          (socklen_t *)&client_addr_len);
  std::cout << "Client connected\n";

  std::array<char, 1024> buf;

  auto data = read(client_fd, buf.data(), 1024);

  std::string res{"+PONG\r\n"};

  write(client_fd, res.data(), res.size());
  close(server_fd);
  return 0;
}
