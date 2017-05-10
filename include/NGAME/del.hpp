#pragma once
#include <functional>

class Del {
public:
  template <typename T> Del(T &&func) : func(std::forward<T>(func)) {}
  ~Del() {
        if(func)
        func(); }

  Del(const Del &) = delete;
  Del &operator=(Del &) = delete;

  Del(Del&& rhs):
      func(rhs.func)
  {
      rhs.func = nullptr;
  }

  Del& operator=(Del&& rhs)
  {
      if(this != &rhs)
      {
      func();
      func = rhs.func;
      rhs.func = nullptr;
      }
      return *this;
  }

private:
  std::function<void(void)> func;
};
