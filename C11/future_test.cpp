#include <iostream>
#include <future>
#include <thread>
#include <string>  
#include <chrono> 
using namespace std;


int main()
{
    std::future<int> f1 = std::async(std::launch::async, [](){  return 8; });
    std::cout << f1.get() << endl; //output: 8

    std::future<void> f2 = std::async(std::launch::async, [](){  std::cout << 8 << endl; });
    f2.wait(); //output: 8


// test 3.
    std::future<int> future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(4));
        return 8;
    });

    std::cout << "waiting...\n";
    std::future_status status;
    do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            std::cout << "deferred\n";
        }
        else if (status == std::future_status::timeout) {
            std::cout << "timeout\n";
        }
        else if (status == std::future_status::ready) {
            std::cout << "ready!\n";
        }
    } while (status != std::future_status::ready);

    std::cout << "result is " << future.get() << '\n';

    return 0;

}



int anther_test_future_main()
{
    // future from a packaged_task
    std::packaged_task<int()> task([]() {
        std::cout << "packaged_task started" << std::endl;
        return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread(std::move(task)).detach(); // launch on a thread

                                           // future from an async()
    std::future<int> f2 = std::async(std::launch::deferred, []() {
        std::cout << "Async task started" << std::endl;
        return 8; });

    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
        << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
    return 0;
}

