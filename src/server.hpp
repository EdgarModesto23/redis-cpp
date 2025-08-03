#pragma once

#include "types.hpp"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <cstddef>
#include <optional>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {

private:
  std::optional<std::vector<std::unique_ptr<types::RESPType>>> session_read();
  void session_write(std::size_t length);
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  char response_[max_length];

public:
  Session(tcp::socket socket);
  void serve();
};

class Server {
private:
  void accept();
  tcp::acceptor acceptor_;

public:
  Server(asio::io_context &ctx, short port);
};

int init_server();
