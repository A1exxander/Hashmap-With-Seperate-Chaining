

#include <iostream>
#include <string>

template<typename T>
class Hashmap {
private:
    struct Node { 
        std::string key{};
        T data{};
        Node* nextPtr{nullptr};
    };

    std::size_t SIZE;
    Node** map; // Remember, array of pointers! Not just a pointer of type Node for the array, but of Node** since our node array stores node pointers! Also, while its better to just do composition or inherit a vector template class, this was done for experience. Also, no need to resize our HM if we use linked lists
    int capacity{};
public: 

    Hashmap() : SIZE{ 10 }, map { new Node* [SIZE] }, capacity{ 0 } {
    
        for (auto i{ 0 }; i < SIZE; ++i) {
        
            map[i] = nullptr;

        }

    }

    ~Hashmap() {
    
        delete[] map;
    
    }

    std::string getKey() const {
     
        std::string key;
        std::cout << "\nEnter a key  : ";
        std::getline(std::cin >> std::ws, key);
        return key;

    }

    int getData() const {  

        int data{};
        std::cout << "\nEnter a number  : ";
        std::cin  >> data;
        return data;

    }

    bool isPrime(int num) const {
    
        bool prime{ false };
        int i{ 0 };

        while (prime == false && i < num / 2) {
        
            if (num % i == 0) {          
                prime = true;
            }

            else {          
                ++i;
            }

        }

        return false;

    }

    int getClosestPrime(int num){
    
        while (!isPrime(num)) {    
            ++num;
        }

        return num;

    }

    void resizeMap(std::size_t newSize) {

        if (newSize > SIZE && !isPrime(newSize)) { // If newSize is NOT prime, we will make newSize the closest larger prime number as prime numbers tend to give less collisions
            newSize = getClosestPrime(newSize);
        }

        Node* current{ nullptr }; // For traversing each linked list of our old map
        Node* tempPtr{ nullptr }; // For traversing the linked list of our new map incase we rehash but spot is not empty
        Node** newMap{ new Node* [newSize] };
        
        for (auto i{ 0 }; i < SIZE; ++i) { // cant use std::copy since we need to rehash everything with our new size else we'd get keys in indexes that they dont belong in
        
            current = map[i];
            while (current != nullptr) {

                tempPtr = newMap[hash(current->key)];
                while (tempPtr != nullptr) {    // If our new map already has something at our desired index, we will traverse its list until its empty
                    tempPtr = tempPtr->nextPtr;
                }
                tempPtr = createNode(current->key, current->data);

                Node* temp = current;
                current = current->nextPtr;
                delete temp; // We must do this as we cant use delete[] map because that only works for a pointer to an array where you have items allocated on stack for each array index, but in this case, we dont as we have an array of pointers
            }

        }

        map = newMap;
        SIZE = newSize;

    }

    Node* createNode(std::string key, int data) const{ // Better to not return a pointer, why? because what if you dont want to return a pointer but want to populate an object on the stack? Its easy to create a node* outside of the function if you need one
    
        Node* myNode = new Node;
        myNode->key = key;
        myNode->data = data;
        return myNode;

    }

    void addNode() {
    
        std::string key{ getKey() };
        int data{ getData() };
        put(key, data);

    }

    void put(const std::string& key, int data) {

        if (contains(key)) {
        
            return;

        }

        Node* node{ createNode(key, data) };

        if (capacity / SIZE > 0.75) {
        
            resizeMap(SIZE * 2);

        }

        int address{ hash(key) };

        if (map[address] == nullptr) {
        
            map[address] = node;

        }
        else {
        
            Node* current{ map[address] };

            while (current->nextPtr != nullptr && current->data != node->data) {

                current = current->nextPtr;

            }

            if (current->nextPtr == nullptr) {
            
                current->nextPtr = node;
                ++capacity;
            
            }

        }


    }

    void remove(const std::string& key) {
    
        int address{ hash(key) };

        Node* current{ map[address] };

        while (current != nullptr && current->key != key) {
        
           current = current->nextPtr;

        }
        if (current == nullptr) {
        
            std::cout << "\nNo key with value of " << key << " in our hashmap!\n";

        }
        else { // We found the correct node containing our data
        
            Node* temp{ current->nextPtr }; // store currents location, so you can delete the data it and then move
            delete current;
            current = temp;
        }
    
    }

    bool contains(const std::string& key) const {

        int address{ hash(key) };
    
        if (map[address] == nullptr) {

            return false;

        }
        else if (map[address] != nullptr) {

            Node* current{ map[address] };

            while (current != nullptr && current->key != key) {

                current = current->nextPtr;

            }

            if (current != nullptr && current->key == key) {

                return true;

            }

            else {
            
                return false;

            }

        }

       
    }

    int hash(std::string key)  const {

        int address{0};

        for (auto i{ 0 }; i < key.size(); ++i) {
        
            address += static_cast<int>(key[i]);

        }
        
        return address % SIZE;
        
    }

    int menu() const{
    
        std::cout << "\n1. Insert Data\n2. Remove Data\n3. Contains Data\n4. Exit\n\nChoice : ";
        int choice{};
        std::cin >> choice;
        return choice;
    
    }

};

int main(){
    Hashmap<int> hm;
    bool repeat{ true };

    while (repeat == true) {
        switch (hm.menu()) {

        case 1:
            hm.addNode();
            break;
        case 2: 
            hm.remove(hm.getKey());
            break;
        case 3:
            std::cout << hm.contains(hm.getKey());
            break;
        default :
            repeat = false;
        }
    }

}


