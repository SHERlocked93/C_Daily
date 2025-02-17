//
// Created by SHERlocked93 on 2023/8/1.
//

// future example
#include <chrono>   // chrono::milliseconds
#include <future>   // async, future
#include <iostream> // cout

using namespace std;

// a non-optimized way of checking for prime numbers:
bool is_prime(int x)
{
    for (int i = 2; i < x; ++i)
        if (x % i == 0)
            return false;

    return true;
}

int main()
{
    // call function asynchronously:
    future<bool> fut = async(is_prime, 444444443);

    // do something while waiting for function to set future:
    cout << "checking, please wait";
    chrono::milliseconds span(100);
    while (fut.wait_for(span) == future_status::timeout)
        cout << '.' << flush;

    bool x = fut.get(); // retrieve return value

    cout << "\n444444443 " << (x ? "is" : "is not") << " prime.\n";

    return 0;
}
