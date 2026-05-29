#include <iostream>
#include <string>
#include "OrderManagement.h"
using namespace std;

int main() {
    OrderManagement orderSystem(5);

    int choice = -1;
    int quantity;
    string itemName;
    string location;
    string robotId;
    Order assignedOrder;

    do {
        cout << "\nWAREHOUSE ORDER MANAGEMENT" << endl;
        cout << "1. Add New Order" << endl;
        cout << "2. Assign Next Order To Robot" << endl;
        cout << "3. Complete Current Order" << endl;
        cout << "4. Display Pending Orders" << endl;
        cout << "5. Display Current Order" << endl;
        cout << "6. Display Completed Orders" << endl;
        cout << "7. Display Summary" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }  
        else {      
            cin.ignore(1000, '\n');
        }

        if (choice == 1) {
            cout << "Enter Item Name: ";
            getline(cin, itemName);

            while (itemName == "") {
                cout << "Item name cannot be blank. Enter Item Name: ";
                getline(cin, itemName);
            }

            cout << "Enter Quantity: ";
            cin >> quantity;

            while (cin.fail() || quantity <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Quantity must be a positive integer. Enter Quantity: ";
                cin >> quantity;
            }
            cin.ignore(1000, '\n');

            cout << "Enter Location: ";
            getline(cin, location);

            while (location == "") {
                cout << "Location cannot be blank. Enter Location: ";
                getline(cin, location);
            }

            int newOrderId;

            if (orderSystem.isPendingFull()) {
                cout << "Failed to add order: queue is full." << endl;
            }
            else {
                newOrderId = orderSystem.addOrder(itemName, quantity, location);
                cout << "Order added successfully." << endl;
                cout << "Generated Order ID: " << newOrderId << endl;
            }
        }
        else if (choice == 2) {
            cout << "Enter Robot ID: ";
            getline(cin, robotId);

            if (orderSystem.isPendingEmpty()) {
                cout << "Assignment failed: no pending order." << endl;
            }
            else if (orderSystem.hasCurrentProcessingOrder()) {
                cout << "Assignment failed: one order is already processing." << endl;
            }
            else if (orderSystem.assignNextOrderToRobot(robotId, assignedOrder)) {
                cout << "Order assigned successfully." << endl;
                cout << "Assigned Order ID: " << assignedOrder.orderId << endl;
                cout << "Assigned Robot ID: " << assignedOrder.robotId << endl;
            }
            else {
                cout << "Assignment failed." << endl;
            }
        }
        else if (choice == 3) {
            if (orderSystem.completeCurrentOrder()) {
                cout << "Current order completed successfully." << endl;
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
