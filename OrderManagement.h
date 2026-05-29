#ifndef ORDERMANAGEMENT_H
#define ORDERMANAGEMENT_H

#include <iostream>
#include "Order.h"
using namespace std;

class OrderManagement {
private:
    Node* pendingFront;
    Node* pendingRear;
    Node* completedFront;
    Node* completedRear;

    int pendingCount;
    int completedCount;
    int maxPending;
    int nextOrderId;

    Order currentOrder;
    bool hasCurrentOrder;

    Node* createNode(Order order);
    void addCompletedOrder(Order order);
    void clearList(Node*& front, Node*& rear);
    void printOrder(Order order);
    void displayList(Node* head);

public:
    OrderManagement(int maxPending = 10);
    ~OrderManagement();

    int addOrder(string itemName, int quantity, string location);
    bool assignNextOrderToRobot(string robotId, Order& assignedOrder);
    bool completeCurrentOrder();

    bool orderIdExists(int orderId);
    bool hasCurrentProcessingOrder();

    bool isPendingEmpty();
    bool isPendingFull();
    int getPendingCount();

    void displayPendingOrders();
    void displayCurrentOrder();
    void displayCompletedOrders();
    void displaySummary();
};

#endif
