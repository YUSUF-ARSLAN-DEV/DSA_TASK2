#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

struct Order {
    string orderId;
    string customerId;
    string itemId;
    int quantity;
    string orderTime;
    string orderStatus;
};

struct Node {
    Order data;
    Node* next;
};

#endif