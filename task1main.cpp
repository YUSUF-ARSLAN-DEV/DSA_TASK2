#include <iostream>
#include <string>
#include "OrderManagement.h"
using namespace std;

int task1Main() {
    // The pending queue capacity is kept small so queue overflow can be demonstrated easily.
    OrderManagement orderSystem(3);

    int choice = -1;
    Order nextOrder;

    // Orders come from a fixed dataset, but the queue is still the main FIFO structure used to manage them.
    do {
        cout << "\nWAREHOUSE ORDER MANAGEMENT" << endl;
        cout << "1. Receive Next Order From Dataset" << endl;
        cout << "2. Send Next Pending Order For Processing" << endl;
        cout << "3. Mark Current Order As Completed" << endl;
        cout << "4. Display Pending Orders" << endl;
        cout << "5. Display Current Order" << endl;
        cout << "6. Display Completed Orders" << endl;
        cout << "7. Display Summary" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Reset the input stream after invalid menu input so the loop can continue safely.
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        } else {
            cin.ignore(1000, '\n');
        }

        // Simulate a new customer order arriving by loading the next row from the dataset into the pending queue.
        if (choice == 1) {
            if (!orderSystem.hasMoreDatasetOrders()) {
                cout << "No more orders in dataset." << endl;
            }
            else if (orderSystem.isPendingFull()) {
                cout << "Cannot receive new order: pending queue is full." << endl;
            }
            else if (orderSystem.receiveNextOrder()) {
                cout << "Next order received and added to pending queue." << endl;
            }
        }
        // Remove the oldest pending order and store it as the current order being processed.
        else if (choice == 2) {
            if (orderSystem.isPendingEmpty()) {
                cout << "No pending order available." << endl;
            }
            else if (orderSystem.hasCurrentProcessingOrder()) {
                cout << "One order is already being processed." << endl;
            }
            else if (orderSystem.processNextOrder(nextOrder)) {
                cout << "Next pending order sent for processing." << endl;
                cout << "Order ID: " << nextOrder.orderId << endl;
            }
        }
        // When processing finishes, move the current order into completed history.
        else if (choice == 3) {
            if (orderSystem.completeCurrentOrder()) {
                cout << "Current order marked as completed." << endl;
            } else {
                cout << "No current order to complete." << endl;
            }
        }
        else if (choice == 4) {
            orderSystem.displayPendingOrders();
        }
        else if (choice == 5) {
            orderSystem.displayCurrentOrder();
        }
        else if (choice == 6) {
            orderSystem.displayCompletedOrders();
        }
        // The summary helps show queue size, completed history count, and remaining dataset orders.
        else if (choice == 7) {
            orderSystem.displaySummary();
        }
        else if (choice == 0) {
            cout << "Program ended." << endl;
        }
        else {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    return 0;
}