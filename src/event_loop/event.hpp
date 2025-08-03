#pragma once

#include <queue>
#include <string>
#include <vector>

#define WRITE_EVENT 0
#define READ_EVENT 1

// TODO Use ASIO single threaded as the main mechanism for scheduling tasks from
// the event queue

class Event {
private:
  int m_type;
  int m_fd;
  std::vector<std::string> m_data; // Just for convenience sake
public:
  Event(int type, int fd, std::vector<std::string> data)
      : m_type(type), m_fd(fd), m_data(std::move(data)) {}

  Event(Event &&) noexcept = default;
  Event(const Event &) = delete;
  Event &operator=(const Event &) = delete;
};

class EventLoop {
private:
  std::queue<Event> m_queue;

public:
  void push(Event &&event);
  void init();
  void poll();
  EventLoop();
  ~EventLoop();
};
