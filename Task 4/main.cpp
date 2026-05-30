#include <iostream>
#include <string>
#include "item_manager.h"

using namespace std;

int readInt(const string& prompt){
    int value;

    while (true){
        cout << prompt;
        cin >> value;

        if (!cin.fail()){
            cin.ignore(1000, '\n');
            return value;
        }

        cout << "Invalid input. Please enter a number." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

string readLine(const string& prompt){
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void showMenu(){
    cout << endl;
    cout << "ITEM SEARCH AND MANAGEMENT MODULE" << endl;
    cout << "1. Add Item" << endl;
    cout << "2. Search Item by ID" << endl;
    cout << "3. Search Item by Name" << endl;
    cout << "4. Update Item" << endl;
    cout << "5. Delete Item" << endl;
    cout << "6. Display All Items" << endl;
    cout << "0. Exit" << endl;
}

int main(){
    ItemManager itemSystem;
    int choice;

    do{
        showMenu();
        choice = readInt("Enter your choice: ");

        if (choice == 1){
            int id = readInt("Enter item ID: ");
            string name = readLine("Enter item name: ");
            string location = readLine("Enter item location: ");

            if (itemSystem.addItem(id, name, location)){
                cout << "Item added successfully." << endl;
            }
            else{
                cout << "Item ID already exists." << endl;
            }
        }
        else if (choice == 2){
            int id = readInt("Enter item ID to search: ");
            Item foundItem;

            if (itemSystem.searchItemById(id, foundItem)){
                cout << "Item found:" << endl;
                cout << "Item ID: " << foundItem.itemId << endl;
                cout << "Item Name: " << foundItem.itemName << endl;
                cout << "Location: " << foundItem.location << endl;
            }
            else{
                cout << "Item not found." << endl;
            }
        }
        else if (choice == 3){
            string name = readLine("Enter item name to search: ");

            if (!itemSystem.searchItemByName(name))
            {
                cout << "No items found with that name." << endl;
            }
        }
        else if (choice == 4){
            int id = readInt("Enter item ID to update: ");
            string newName = readLine("Enter new item name: ");
            string newLocation = readLine("Enter new item location: ");

            if (itemSystem.updateItem(id, newName, newLocation)){
                cout << "Item updated successfully." << endl;
            }
            else{
                cout << "Item not found." << endl;
            }
        }
        else if (choice == 5){
            int id = readInt("Enter item ID to delete: ");

            if (itemSystem.deleteItem(id)){
                cout << "Item deleted successfully." << endl;
            }
            else{
                cout << "Item not found." << endl;
            }
        }
        else if (choice == 6){
            itemSystem.displayAllItems();
        }
        else if (choice == 0){
            cout << "Exiting program." << endl;
        }
        else{
            cout << "Invalid choice." << endl;
        }

    } 
    
    while (choice != 0);

    return 0;
}