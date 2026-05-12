# Producer-Consumer with Condition Variables — CSE 4733 Operating Systems

A multithreaded producer-consumer implementation in C++ using `std::mutex`, `std::condition_variable`, and `std::atomic` for thread synchronization. One producer thread generates work items and two consumer threads process them concurrently from a shared queue.

---

## Features

- **Producer thread** — generates 10 base/exponent pairs and pushes them to a shared queue
- **Two consumer threads** — wait for work, pull items from the queue, and compute the result
- **Condition variable synchronization** — consumers block efficiently until work is available
- **Atomic stop flag** — `std::atomic<bool>` signals consumers when production is complete
- **Overflow detection** — safe integer exponentiation with overflow checks
- **Mutex-protected shared queue** — prevents race conditions on `std::queue` access

---

## How It Works

1. The producer pushes `(base, exponent)` pairs onto a shared queue every 100ms
2. Consumers wait on a `condition_variable` until the queue has items
3. Each consumer grabs an item, unlocks the mutex, and computes `base^exponent`
4. When the producer finishes, it sets `shouldContinue = false` and notifies all consumers
5. Consumers exit cleanly when the queue is empty and production is done

---

## Example Output

```
Produced: 32^2
Consumed(id: 1): 32^2 = 1024
Produced: 45^3
Consumed(id: 2): 45^3 = 91125
...
```

---

## How to Build and Run

### Prerequisites
- GCC / G++
- CMake 3.10+
- GMP library (`libgmp-dev`)

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run
```bash
./Lab10
```

---

## Key Concepts Demonstrated

- **Producer-consumer pattern** — classic concurrency design pattern
- **Condition variables** — efficient thread blocking without busy-waiting
- **Atomic operations** — lock-free signaling with `std::atomic<bool>`
- **RAII locking** — `std::unique_lock` for safe mutex management
- **Thread coordination** — proper join and graceful shutdown
- **Overflow-safe arithmetic** — integer exponentiation with bounds checking

---

## Built With

- C++17
- CMake
- GMP (GNU Multiple Precision)

---

## Author

**Cyrus** — Computer Science, Mississippi State University  
[GitHub](https://github.com/Cymerican)
