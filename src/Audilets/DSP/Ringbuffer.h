#pragma once

#include <readerwriterqueue/readerwriterqueue.h>

#include <chrono>
#include <functional>

namespace Audilets::DSP
{
  template<typename T>
  class Ringbuffer
  {
  public:

    Ringbuffer(const size_t size, std::function<T*(size_t index)> alloc, std::function<void(T* value)> free) :
      alloc(alloc),
      free(free),
      todo(size),
      done(size)
    {
      for (size_t index = 0; index < size; ++index)
      {
        done.enqueue(alloc(index));
      }
    }

    ~Ringbuffer()
    {
      T* value;

      while (done.try_dequeue(value))
      {
        free(value);
      }

      while (todo.try_dequeue(value))
      {
        free(value);
      }
    }

    bool empty() const
    {
      return todo.peek() == nullptr;
    }

    void flush()
    {
      T* value;

      while (todo.try_dequeue(value))
      {
        done.enqueue(value);
      }
    }

    bool write(std::function<void(T& value)> callback)
    {
      T* value;

      if (!done.try_dequeue(value))
      {
        return false;
      }

      callback(*value);

      todo.enqueue(value);

      return true;
    }

    template<typename Rep, typename Per>
    bool write(const std::chrono::duration<Rep, Per>& timeout, std::function<void(T& value)> callback)
    {
      T* value;

      if (!done.wait_dequeue_timed(value, timeout))
      {
        return false;
      }

      callback(*value);

      todo.enqueue(value);

      return true;
    }

    bool read(std::function<void(T& value)> callback)
    {
      T* value;

      if (!todo.try_dequeue(value))
      {
        return false;
      }

      callback(*value);

      done.enqueue(value);

      return true;
    }

    template<typename Rep, typename Per>
    bool read(const std::chrono::duration<Rep, Per>& timeout, std::function<void(T& value)> callback)
    {
      T* value;

      if (!todo.wait_dequeue_timed(value, timeout))
      {
        return false;
      }

      callback(*value);

      done.enqueue(value);

      return true;
    }

  private:

    const std::function<T*(size_t index)> alloc;
    const std::function<void(T* value)> free;

    moodycamel::BlockingReaderWriterQueue<T*> todo;
    moodycamel::BlockingReaderWriterQueue<T*> done;
  };
}
