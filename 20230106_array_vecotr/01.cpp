//
// Created by qiany on 2023/1/6.
//
#include <iostream>
#include <vector>

using namespace std;


int main() {
    // vector 新型数组，可以自动动态扩容
    vector<int> arr = {0, 1, 2, 3, 4, 5};
    cout << "size:" << arr.size() << "\tcapacity:" << arr.capacity() << "\n" << endl;
    
    // 尾部增加
    arr.insert(arr.end() - arr.size() / 2, 8);
    
    // 尾部移除
    //arr.erase(arr.end() - 2);
    
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << ", ";
    }
    //display(&arr);
    return 0;
}
