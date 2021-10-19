#include <iostream>
#include <vector>
#include <random>
#include "include/bit_map.h"

int main() {
//    int in;
    bool pass = true;
    for (int i=0; i<1000 && pass; ++i) {
        BitMap bit_map;
        std::vector<int> arr;
        for (int j=0; j<1000; ++j) {
            int r = rand() % 2;
            bit_map.push_back(r);
            arr.push_back(r);
        }

        std::cout << "bit_map size: " << bit_map.capacity() << std::endl;
        for (auto &ele:arr) {
            if (ele != bit_map.get()) {
                std::cout << "i:" << i << ", ele:" << ele << std::endl;
                pass = false;
                break;
            }
        }
    }
    if (pass) {
        std::cout << "PASS TEST" << std::endl;
    }

//    while(std::cin>>in) {
//        if(in>0) {
//            bit_map.push_back(1);
//        } else if (in==0) {
//            bit_map.push_back(0);
//        } else {
//            break;
//        }
//    }
//
//    while (!bit_map.empty() && !bit_map.at_end()) {
//        std::cout << bit_map.get() << std::endl;
//    }

    return 0;
}
