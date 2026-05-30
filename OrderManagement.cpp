#include "OrderManagement.h"

OrderManagement::OrderManagement(int maxPending) {
    pendingFront = NULL;
    pendingRear = NULL;
    completedFront = NULL;
    completedRear = NULL;

    pendingCount = 0;
    completedCount = 0;
    this->maxPending = maxPending;

    hasCurrentOrder = false;

    datasetSize = 5;
    nextDatasetIndex = 0;

    initializeDataset();
}

OrderManagement::~OrderManagement() {
    clearList(pendingFront, pendingRear);
    clearList(completedFront, completedRear);
}

void OrderManagement::initializeDataset() {
    dataset[0] = {"O001", "C101", "I205", 2, "09:00", "Pending"};
    dataset[1] = {"O002", "C102", "I110", 1, "09:05", "Pending"};
    dataset[2] = {"O003", "C103", "I330", 3, "09:10", "Pending"};
    dataset[3] = {"O004", "C104", "I450", 2, "09:15", "Pending"};
    dataset[4] = {"O005", "C105", "I120", 1, "09:20", "Pending"};
}

Node* OrderManagement::createNode(Order order) {
    Node* newNode = new Node;
    newNode->data = order;
    newNode->next = NULL;
    return newNode;
}

void OrderManagement::addCompletedOrder(Order order) {
    Node* newNode = createNode(order);

    if (completedFront == NULL) {
        completedFront = newNode;
        completedRear = newNode;
    } else {
        completedRear->next = newNode;
        completedRear = newNode;
    }

    completedCount++;
}

void OrderManagement::clearList(Node*& front, Node*& rear) {
    Node* temp;

    while (front != NULL) {
        temp = front;
        front = front->next;
        delete temp;
    }

    rear = NULL;
}

void OrderManagement::printOrder(Order order) {
    cout << "Order ID      : " << order.orderId << endl;
    cout << "Customer ID   : " << order.customerId << endl;
    cout << "Item ID       : " << order.itemId << endl;
    cout << "Quantity      : " << order.quantity << endl;
    cout << "Order Time    : " << order.orderTime << endl;
    cout << "Order Status  : " << order.orderStatus << endl;
}

void OrderManagement::displayList(Node* head) {
    Node* current = head;

    if (current == NULL) {
        cout << "No orders found." << endl;
        return;
    }

    while (current != NULL) {
        printOrder(current->data);
        cout << "------------------------" << endl;
        current = current->next;
    }
}

bool OrderManagement::receiveNextOrder() {
    if (!hasMoreDatasetOrders() || isPendingFull()) {
        return false;
    }

    Order newOrder = dataset[nextDatasetIndex];
    Node* newNode = createNode(newOrder);

    if (pendingFront == NULL) {
        pendingFront = newNode;
        pendingRear = newNode;
    } else {
        pendingRear->next = newNode;
        pendingRear = newNode;
    }

    pendingCount++;
    nextDatasetIndex++;

    return true;
}

bool OrderManagement::processNextOrder(Order& nextOrder) {
    if (pendingFront == NULL || hasCurrentOrder) {
        return false;
    }

    Node* temp = pendingFront;
    nextOrder = temp->data;

    pendingFront = pendingFront->next;
    if (pendingFront == NULL) {
        pendingRear = NULL;
    }

    delete temp;
    pendingCount--;

    nextOrder.orderStatus = "Processing";
    currentOrder = nextOrder;
    hasCurrentOrder = true;

    return true;
}

bool OrderManagement::completeCurrentOrder() {
    Order finishedOrder;

    if (!hasCurrentOrder) {
        return false;
    }

    finishedOrder = currentOrder;
    finishedOrder.orderStatus = "Completed";

    addCompletedOrder(finishedOrder);
    hasCurrentOrder = false;

    return true;
}

bool OrderManagement::hasMoreDatasetOrders() {
    return nextDatasetIndex < datasetSize;
}

bool OrderManagement::hasCurrentProcessingOrder() {
    return hasCurrentOrder;
}

bool OrderManagement::isPendingEmpty() {
    return pendingFront == NULL;
}

bool OrderManagement::isPendingFull() {
    return pendingCount >= maxPending;
}

int OrderManagement::getPendingCount() {
    return pendingCount;
}

int OrderManagement::getRemainingDatasetOrders() {
    return datasetSize - nextDatasetIndex;
}

void OrderManagement::displayPendingOrders() {
    cout << "\nPending Orders" << endl;
    cout << "==============\n";
    displayList(pendingFront);
}

void OrderManagement::displayCurrentOrder() {
    cout << "\nCurrent Order Being Processed" << endl;
    cout << "=============================\n";

    if (!hasCurrentOrder) {
        cout << "No current order." << endl;
        return;
    }

    printOrder(currentOrder);
}

void OrderManagement::displayCompletedOrders() {
    cout << "\nCompleted Orders" << endl;
    cout << "================\n";
    displayList(completedFront);
}

void OrderManagement::displaySummary() {
    cout << "\nSystem Summary" << endl;
    cout << "==============\n";
    cout << "Pending Orders           : " << pendingCount << endl;
    cout << "Completed Orders         : " << completedCount << endl;
    cout << "Current Order            : " << (hasCurrentOrder ? "Yes" : "No") << endl;
    cout << "Remaining Dataset Orders : " << getRemainingDatasetOrders() << endl;
    cout << "Max Pending Capacity     : " << maxPending << endl;
}