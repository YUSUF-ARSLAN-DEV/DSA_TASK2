#include "OrderManagement.h"

OrderManagement::OrderManagement(int maxPending) {
    pendingFront = NULL;
    pendingRear = NULL;
    completedFront = NULL;
    completedRear = NULL;

    nextOrderId = 1;
    pendingCount = 0;
    completedCount = 0;
    this->maxPending = maxPending;

    hasCurrentOrder = false;
}

OrderManagement::~OrderManagement() {
    clearList(pendingFront, pendingRear);
    clearList(completedFront, completedRear);
}

Node* OrderManagement::createNode(Order order) {
    Node* newNode = new Node;
    newNode->data = order;
    newNode->next = NULL;
    return newNode;
}

bool OrderManagement::orderIdExists(int orderId) {
    Node* current = pendingFront;

    while (current != NULL) {
        if (current->data.orderId == orderId) {
            return true;
        }
        current = current->next;
    }

    current = completedFront;
    while (current != NULL) {
        if (current->data.orderId == orderId) {
            return true;
        }
        current = current->next;
    }

    if (hasCurrentOrder && currentOrder.orderId == orderId) {
        return true;
    }

    return false;
}

bool OrderManagement::hasCurrentProcessingOrder() {
    return hasCurrentOrder;
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
    cout << "Order ID : " << order.orderId << endl;
    cout << "Item     : " << order.itemName << endl;
    cout << "Quantity : " << order.quantity << endl;
    cout << "Location : " << order.location << endl;
    cout << "Robot ID : " << order.robotId << endl;
    cout << "Status   : " << order.status << endl;
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

int OrderManagement::addOrder(string itemName, int quantity, string location) {
    if (pendingCount >= maxPending) {
        return -1;
    }

    if (itemName == "" || location == "" || quantity <= 0) {
        return -1;
    }

    Order newOrder;
    newOrder.orderId = nextOrderId;
    newOrder.itemName = itemName;
    newOrder.quantity = quantity;
    newOrder.location = location;
    newOrder.robotId = "-";
    newOrder.status = "PENDING";

    Node* newNode = createNode(newOrder);

    if (pendingFront == NULL) {
        pendingFront = newNode;
        pendingRear = newNode;
    } else {
        pendingRear->next = newNode;
        pendingRear = newNode;
    }

    pendingCount++;
    nextOrderId++;

    return newOrder.orderId;
}

bool OrderManagement::assignNextOrderToRobot(string robotId, Order& assignedOrder) {
    if (pendingFront == NULL) {
        return false;
    }

    if (hasCurrentOrder) {
        return false;
    }

    Node* temp = pendingFront;
    assignedOrder = temp->data;

    pendingFront = pendingFront->next;
    if (pendingFront == NULL) {
        pendingRear = NULL;
    }

    delete temp;
    pendingCount--;

    assignedOrder.robotId = robotId;
    assignedOrder.status = "PROCESSING";

    currentOrder = assignedOrder;
    hasCurrentOrder = true;

    return true;
}

bool OrderManagement::completeCurrentOrder() {
    Order finishedOrder;

    if (!hasCurrentOrder) {
        return false;
    }

    finishedOrder = currentOrder;
    finishedOrder.status = "COMPLETED";

    addCompletedOrder(finishedOrder);
    hasCurrentOrder = false;

    return true;
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
    cout << "Pending Orders   : " << pendingCount << endl;
    cout << "Completed Orders : " << completedCount << endl;
    cout << "Current Order    : " << (hasCurrentOrder ? "Yes" : "No") << endl;
    cout << "Max Capacity     : " << maxPending << endl;
}
