#pragma once

#include "command.hpp"
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <cstddef>
#include <vector>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {

private:
  void session_read();
  void session_write(std::size_t length);
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  std::vector<char> response_;
  Database &db_;

public:
  Session(tcp::socket socket, Database &db);
  void serve();
};

class Server {
private:
  void accept();
  tcp::acceptor acceptor_;
  Database db_;

public:
  Server(asio::io_context &ctx, short port, Database db);
};

int init_server();
