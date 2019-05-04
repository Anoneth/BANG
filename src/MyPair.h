/*
 * MyPair.cc
 *
 *  Created on: 22 апр. 2019 г.
 *      Author: anoneth
 */

#include <omnetpp.h>

struct MyPair {
public:
    int left;
    int right;
    MyPair() {}
    MyPair(int left, int right) {
        this->left = left;
        this->right = right;
    }
};
