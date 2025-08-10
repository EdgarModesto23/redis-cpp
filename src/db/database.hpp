#pragma once

#include <any>
#include <string>
#include <unordered_map>

class Database {
private:
  std::unordered_map<std::string, std::any> data_;

public:
  template <typename T> void add(const std::string &key, T val) {
    data_[key] = std::any(val);
  }

  template <typename T> T get(const std::string &key) const {
    return std::any_cast<T>(data_.at(key));
  }

  Database() : data_(std::unordered_map<std::string, std::any>()) {}
};

extern Database db;
