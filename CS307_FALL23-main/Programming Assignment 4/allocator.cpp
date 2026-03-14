#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;


class HeapManager{


    // node struct for the linked list implementation
    private:
        struct Node {
        int tid; // id of the thread that allocated the memory
        int size; // size of the memory block
        int start; // start idx' of the memory block
        Node* next; // pointer to the next node
        Node* prev; // pointer to the previous node

        Node(int t, int s, int st) : tid(t), size(s), start(st), next(NULL), prev(NULL) {}
    };
    
    Node* head; // head of linked list
    pthread_mutex_t lock; // lock for linked list
    
    Node* findFreeNode(int size) { // find a free node that has enough size
        Node* curr = head;
        while (curr != NULL) {
            if (curr->tid == -1 && curr->size >= size) {
                return curr;
            }
            curr = curr->next;
        }
        return NULL;
    }

    Node* findNode(int threadId, int start) { // find a node with the given thread id and start index
        Node* currentNode = head;
        while (currentNode != NULL) {
            if (currentNode->tid == threadId && currentNode->start == start) {
                return currentNode;
            }
            currentNode = currentNode->next;
        }
        return NULL;
    }

    void removeNode(Node* targetNode) { // remove the given node
        if (targetNode == head) {
            head = head->next;
        } else {
            Node* current = head;
            while (current->next != targetNode) {
                current = current->next;
            }
            current->next = targetNode->next;
        }
        delete targetNode;
    }

    void prevMerge(Node* currentNode) { // merge the given node with the previous node if it is free, done for the coalescing
        if (currentNode != head) {
            Node* previousNode = head;
            while (previousNode->next != currentNode) {
                previousNode = previousNode->next;
            }
            if (previousNode->tid == -1) {
                previousNode->size += currentNode->size;
                removeNode(currentNode);
            }
        }
    }
    // public

    public:

    ~HeapManager() {

        cout<<"Execution is done"<<endl;
        printNonLocked();

        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }



    void print() { // this is the print function that is requested in the pdf, there is also a version used inside other functions that do not use the lock
        pthread_mutex_lock(&lock);
        Node* current = head;

        if (current == NULL) { // if the list is empty
            cout << "Empty list." << endl;
            return;
        }

        cout << "[" << current->tid << "][" << current->size << "][" << current->start << "]"; // need to print the first node, otherwise if single loop used -- at the end

        while (current->next) {
            current = current->next;
            cout << "---[" << current->tid << "][" << current->size << "][" << current->start << "]";
        }
        cout << endl;
        pthread_mutex_unlock(&lock);
    }

    void printNonLocked() { // this is the print function which does not use the lock
        Node* current = head;

        if (current == NULL) {
            cout << "Empty list." << endl;
            return;
        }

        cout << "[" << current->tid << "][" << current->size << "][" << current->start << "]"; // need to print the first node, otherwise if single loop used -- at the end

        while (current->next) {
            current = current->next;
            cout << "---[" << current->tid << "][" << current->size << "][" << current->start << "]";
        }
        cout << endl;
    }

    int initHeap(int size) {
        head = new Node(-1, size, 0);
        lock = PTHREAD_MUTEX_INITIALIZER; // initalize the lock
        cout << "Memory initialized." << endl;
        pthread_mutex_unlock(&lock); // locking since this print function is the one that does not use lock, we can also call the function that uses lock but wanted to use non-lock one in all functions
        printNonLocked();
        pthread_mutex_unlock(&lock);
        return 1;
    }

    int myMalloc(int threadId, int size) {
        pthread_mutex_lock(&lock);

        Node* curr = findFreeNode(size);
        if (curr != NULL) {
            cout << "Allocated for thread " << threadId << endl;
            if (curr->size == size){
                curr->tid = threadId;
                printNonLocked();
                pthread_mutex_unlock(&lock);
                return curr->start;
            }
            else{
                Node* newNode = new Node(-1, curr->size - size, curr->start);
                curr->tid = threadId;
                curr->size = size;
                newNode ->next = curr->next;
                if(curr->next!=NULL) curr->next->prev = newNode;
                newNode -> prev = curr;
                curr->next = newNode;
                printNonLocked();
                pthread_mutex_unlock(&lock);
                return newNode->start;
            }
        }

        cout << "Can not allocate, requested size " << size << " for thread " << threadId
             << " is bigger than remaining size." << endl;
        printNonLocked();
        pthread_mutex_unlock(&lock);
        return -1;
    }

    int myFree(int threadId, int start) {
        pthread_mutex_lock(&lock);

        Node* curr = findNode(threadId, start);
        if (curr != NULL) {
            prevMerge(curr);
            curr->tid = -1;

            Node* nextNode = curr->next;
            if (nextNode != NULL && nextNode->tid == -1) {
                curr->size += nextNode->size;
                removeNode(nextNode);
            }

            cout << "Freed for thread " << threadId << endl;
            printNonLocked();
            pthread_mutex_unlock(&lock);
            return 1;
        }

        cout << "No free node for the thread " << threadId << endl;
        printNonLocked();
        pthread_mutex_unlock(&lock);
        return -1;
    }

};


