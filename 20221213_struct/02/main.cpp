//
// Created by qiany on 2023/1/6.
//
#include <iostream>

using namespace std;

int main() {
    struct Week {
        short Monday = 0;
        short Tuesday = 1;
        short Wednesday = 2;
        short Thursday = 3;
        short Friday = 4;
        short Saturday = 5;
        short Sunday = 6;
    };
    
    Week w;
    
    cout << w.Sunday << endl;
    cout << sizeof(w) << endl;
    return 0;
}
