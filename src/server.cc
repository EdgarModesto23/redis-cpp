#include "server.hpp"
#include "asio.hpp"
#include "spdlog/spdlog.h"
#include "types.hpp"
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

std::optional<std::vector<std::unique_ptr<types::RESPType>>>
Session::session_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(data_, max_length),
      [this, self](std::error_code err, std::size_t length) {
        if (!err) {
          spdlog::info("Read {} bytes from buff", length);
          std::cout << data_ << '\n';
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
  try {

    asio::io_context io_context;

    Server s(io_context, 6379);

    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
