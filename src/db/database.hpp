#pragma once

#include <any>
#include <asio/error_code.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

class Database {
private:
  std::unordered_map<std::string, std::any> data_;
  asio::io_context &io_;

public:
  template <typename T> void add(const std::string &key, T val) {
    data_[key] = std::any(val);
  }

  template <typename T> T get(const std::string &key) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
      auto val = std::any_cast<T>(it->second);
      return val;
    }
    return "";
  }

  template <typename T>
  void add_expiry(const std::string &key, T val, int milis) {
    data_[key] = std::any(val);

    auto timer = std::make_shared<asio::steady_timer>(
        io_, std::chrono::milliseconds(milis));
    timer->async_wait([this, key, timer](const asio::error_code &err) {
      if (!err) {
        auto it = data_.find(key);
        if (it != data_.end()) {
          data_.erase(it);
        }
      }
    });
  }

  Database(asio::io_context &ctx)
      : data_(std::unordered_map<std::string, std::any>()), io_(ctx) {}
};
