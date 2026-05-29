#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

struct Order {
    int orderId;
    string itemName;
    int quantity;
    string location;
    string robotId;
    string status;
};

struct Node {
    Order data;
    Node* next;
};

#endif