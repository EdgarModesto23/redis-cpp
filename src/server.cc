#include "server.hpp"
#include "asio.hpp"
#include "parser.hpp"
#include "spdlog/spdlog.h"
#include <arpa/inet.h>
#include <asio/error.hpp>
#include <asio/error_code.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <spdlog/spdlog.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

using asio::ip::tcp;

using namespace parser;

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {};

Server::Server(asio::io_context &ctx, short port)
    : acceptor_(ctx, tcp::endpoint(tcp::v4(), port)) {
  accept();
};

void Session::session_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(data_, max_length),
      [this, self](std::error_code err, std::size_t length) {
        if (!err) {
          std::string raw_data(data_, data_ + length);
          auto parsed = parse_request(raw_data);
          spdlog::debug("Found {} commands", parsed.size());
          for (const auto &command : parsed) {
            auto result = command.get()->serve();
            auto bytes = result.get()->to_bytes();
            response_.insert(response_.end(), bytes.begin(), bytes.end());
          }
          session_write(response_.size());
        } else {
          if (err == asio::error::eof) {
            return;
          }
          if (err == asio::error::operation_aborted) {
            spdlog::error(
                "Operation aborted when attempting to read from buffer: {}",
                err.message());
            return;
          } else {
            spdlog::error("Error when reading from buffer: {}", err.message());
          }
        }
      });
};

void Session::session_write(std::size_t length) {
  auto self(shared_from_this());
  asio::async_write(socket_, asio::buffer(response_, length),
                    [this, self](asio::error_code err, std::size_t l) {
                      if (!err) {
                        spdlog::info("Wrote {} bytes", l);
                        response_.clear();
                        session_read();
                      }
                    });
};

void Session::serve() {
  spdlog::info("Accepted client");
  session_read();
};

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
