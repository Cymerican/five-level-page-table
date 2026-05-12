#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <atomic>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <gmpxx.h>

std::queue<std::pair<int, int>> sharedQueue;
std::mutex mutex;
std::condition_variable cv;
std::atomic<bool> shouldContinue(true);

long long int_power(int base, int exp)
{
    long long result = 1;
    long long max_value = std::numeric_limits<long long>::max();

    while (exp)
    {
        if (exp % 2 == 1)
        {
            if (result > max_value / base)
            {
                std::cerr << "Overflow detected in power calculation\n";
                return -1;
            }
            result *= base;
        }
        if (exp != 1 && base > max_value / base)
        {
            std::cerr << "Overflow detected in power calculation\n";
            return -1;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

void thread1_task()
{
    int value[10] = {32, 45, 78, 90, 123, 150, 175, 200, 210, 212};
    int power[10] = {2, 3, 4, 5, 6, 7, 3, 6, 5, 4};

    for (int i = 0; i < 10; i++)
    {
        // Simulate some processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Lock, push to queue, print, notify one consumer
        std::unique_lock<std::mutex> lock(mutex);
        sharedQueue.push(std::make_pair(value[i], power[i]));
        std::cout << "Produced: " << value[i] << "^" << power[i] << "\n";
        cv.notify_one();
    }

    // Done producing - signal consumers to stop
    shouldContinue = false;
    cv.notify_all();
}

bool hasWork()
{
    return !sharedQueue.empty() || !shouldContinue;
}

void thread2_task(int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait until there's something in the queue or production is done
        cv.wait(lock, []() { return !sharedQueue.empty() || !shouldContinue; });

        // If queue is empty and producer is done, exit
        if (sharedQueue.empty() && !shouldContinue)
            break;

        // Grab the front item
        std::pair<int, int> item = sharedQueue.front();
        sharedQueue.pop();

        int base = item.first;
        int exp  = item.second;

        long long result = int_power(base, exp);

        std::cout << "Consumed(id: " << id << "): " << base << "^" << exp << " = " << result << "\n";
    }
}

int main()
{
    std::thread producer(thread1_task);
    std::thread consumer1(thread2_task, 1);
    std::thread consumer2(thread2_task, 2);

    producer.join();
    consumer1.join();
    consumer2.join();

    return 0;
}
