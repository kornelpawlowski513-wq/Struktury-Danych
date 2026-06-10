#pragma once

template <typename T>
struct Node {
    T value;
    Node<T>* next;

    Node() : next(nullptr) {}

    Node(T val_) : value(val_), next(nullptr) {}

    Node(T val_, Node<T>*next_ ) : value{val_}, next{next_} {}
};

template <typename T>
class LinkedList {
public:
    int size;
    Node<T>* head;

    LinkedList() : size(0), head(nullptr) {}

    ~LinkedList() {
        Node<T>* temp = head;
        while (temp != nullptr) {
            Node<T>* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    LinkedList(const LinkedList<T>& other) : size(0), head(nullptr) {
        Node<T>* temp = other.head;
        while (temp != nullptr) {
            add_back(temp->value);
            temp = temp->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) return *this;

        Node<T>* temp = head;
        while (temp != nullptr) {
            Node<T>* next = temp->next;
            delete temp;
            temp = next;
        }
        head = nullptr;
        size = 0;

        temp = other.head;
        while (temp != nullptr) {
            add_back(temp->value);
            temp = temp->next;
        }
        return *this;
    }

    void add_front(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
        size++;
    }

    void add_back(T val) {
        Node<T>* newNode = new Node<T>(val);

        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    void add_at(T val, unsigned int index) {
        if (index > size) throw std::out_of_range("Index out of range");
        
        if (index == 0) {
            add_front(val);
            return;
        }
        if(index == size) {
            add_back(val);
            return;
        }
        Node<T>* newNode = new Node<T>(val);
        Node<T>* temp = head;
        for(unsigned int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        size++;
    }

    void remove_front() {
        if (head == nullptr) return;
        
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    void remove_back() {
        if (head == nullptr) return;
        if (size == 1) {
            remove_front();
            return;
        }

        Node<T>* temp = head;
        while (temp->next->next != nullptr) {
            temp = temp->next;
        }
        
        delete temp->next; 
        temp->next = nullptr;
        size--;
    }

    void remove_at(unsigned int index) {
        if (head == nullptr) return;
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0) {
            remove_front();
            return;
        }
        Node<T>* temp = head;
        for(int i =0 ; i < index - 1 ; i++) {
            temp = temp->next;
        }
        Node<T>* temp2 = temp->next;
        Node<T>* temp3 = temp2->next;
        delete temp2;
        temp->next = temp3;
        size--;
    }

    T& operator[](unsigned int index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* temp = head;
        int i = 0;

        while (temp != nullptr && i < index) {
            temp = temp->next;
            i++;
        }
        return temp->value;
    }

    int find(T val) {
        Node<T>* temp = head;
        int i = 0;
        while(temp != nullptr) {
            if (temp->value == val) return i;
            else {
                i++;
                temp = temp->next;
            }
        }
        return -1; // -1 - nie znaleziono
    }
};
