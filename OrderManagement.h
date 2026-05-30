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

    Order currentOrder;
    bool hasCurrentOrder;

    Order dataset[5];
    int datasetSize;
    int nextDatasetIndex;

    void initializeDataset();
    Node* createNode(Order order);
    void addCompletedOrder(Order order);
    void clearList(Node*& front, Node*& rear);
    void printOrder(Order order);
    void displayList(Node* head);

public:
    OrderManagement(int maxPending = 3);
    ~OrderManagement();

    bool receiveNextOrder();
    bool processNextOrder(Order& nextOrder);
    bool completeCurrentOrder();

    bool hasMoreDatasetOrders();
    bool hasCurrentProcessingOrder();
    bool isPendingEmpty();
    bool isPendingFull();
    int getPendingCount();
    int getRemainingDatasetOrders();

    void displayPendingOrders();
    void displayCurrentOrder();
    void displayCompletedOrders();
    void displaySummary();
};

#endif