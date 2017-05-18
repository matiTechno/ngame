#pragma once
#include <functional>

class Del final {
public:
  template <typename T> Del(T &&func) : func(std::forward<T>(func)) {}
  ~Del() {
        func(); }

    Del(const Del &) = delete;
    Del &operator=(Del &) = delete;

private:
  std::function<void(void)> func;
};
