#include "item_manager.h"
#include <iostream>

using namespace std;

static void printItem(const Item& item){
    cout << "Item ID: " << item.itemId << endl;
    cout << "Item Name: " << item.itemName << endl;
    cout << "Location: " << item.location << endl;
}

ItemManager::ItemManager() : root(nullptr){
}

ItemManager::~ItemManager(){
    destroy(root);
}

bool ItemManager::insert(Node*& current, const Item& item){
    if (current == nullptr){
        current = new Node(item);
        return true;
    }

    if (item.itemId < current->data.itemId){
        return insert(current->left, item);
    }

    if (item.itemId > current->data.itemId){
        return insert(current->right, item);
    }

    return false;
}

ItemManager::Node* ItemManager::searchById(Node* current, int itemId) const
{
    if (current == nullptr || current->data.itemId == itemId){
        return current;
    }

    if (itemId < current->data.itemId){
        return searchById(current->left, itemId);
    }

    return searchById(current->right, itemId);
}

bool ItemManager::searchByNameAndDisplay(Node* current, const string& name) const{
    if (current == nullptr){
        return false;
    }

    bool foundLeft = searchByNameAndDisplay(current->left, name);
    bool foundCurrent = false;
    bool foundRight = false;

    if (current->data.itemName == name){
        printItem(current->data);
        cout << "----------------------------" << endl;
        foundCurrent = true;
    }

    foundRight = searchByNameAndDisplay(current->right, name);

    return foundLeft || foundCurrent || foundRight;
}

bool ItemManager::update(Node* current, int itemId, const string& newName, const string& newLocation){
    Node* target = searchById(current, itemId);

    if (target == nullptr){
        return false;
    }

    target->data.itemName = newName;
    target->data.location = newLocation;
    return true;
}

ItemManager::Node* ItemManager::findMin(Node* current) const{
    while (current != nullptr && current->left != nullptr){
        current = current->left;
    }

    return current;
}

ItemManager::Node* ItemManager::deleteNode(Node* current, int itemId, bool& deleted){
    if (current == nullptr){
        return nullptr;
    }

    if (itemId < current->data.itemId){
        current->left = deleteNode(current->left, itemId, deleted);
    }
    else if (itemId > current->data.itemId){
        current->right = deleteNode(current->right, itemId, deleted);
    }
    else{
        deleted = true;

        if (current->left == nullptr && current->right == nullptr){
            delete current;
            return nullptr;
        }

        if (current->left == nullptr){
            Node* temp = current->right;
            delete current;
            return temp;
        }

        if (current->right == nullptr){
            Node* temp = current->left;
            delete current;
            return temp;
        }

        Node* successor = findMin(current->right);
        current->data = successor->data;
        current->right = deleteNode(current->right, successor->data.itemId, deleted);
    }

    return current;
}

void ItemManager::displayInOrder(Node* current) const{
    if (current == nullptr){
        return;
    }

    displayInOrder(current->left);

    cout << "Item ID: " << current->data.itemId << endl;
    cout << "Item Name: " << current->data.itemName << endl;
    cout << "Location: " << current->data.location << endl;
    cout << "----------------------------" << endl;

    displayInOrder(current->right);
}

void ItemManager::destroy(Node* current){
    if (current == nullptr){
        return;
    }

    destroy(current->left);
    destroy(current->right);
    delete current;
}

bool ItemManager::addItem(int itemId, const string& itemName, const string& location){
    Item item;
    item.itemId = itemId;
    item.itemName = itemName;
    item.location = location;

    return insert(root, item);
}

bool ItemManager::searchItemById(int itemId, Item& foundItem) const{
    Node* result = searchById(root, itemId);

    if (result == nullptr){
        return false;
    }

    foundItem = result->data;
    return true;
}

bool ItemManager::searchItemByName(const string& name) const{
    return searchByNameAndDisplay(root, name);
}

bool ItemManager::updateItem(int itemId, const string& newName, const string& newLocation){
    return update(root, itemId, newName, newLocation);
}

bool ItemManager::deleteItem(int itemId){
    bool deleted = false;
    root = deleteNode(root, itemId, deleted);
    return deleted;
}

void ItemManager::displayAllItems() const{
    if (root == nullptr)
    {
        cout << "No items found." << endl;
        return;
    }

    displayInOrder(root);
}