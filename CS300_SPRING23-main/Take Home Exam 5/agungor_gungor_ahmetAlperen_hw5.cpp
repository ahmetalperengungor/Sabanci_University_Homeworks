#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Contact{
    string firstName;
    string lastName;
    string number;
    string location;
    string connectedName;

    // operator overloading for comparison that uses connectedName to alphabetically compare contacts
    bool operator>(const Contact& other) const{
        return connectedName >= other.connectedName;
    }
    bool operator<(const Contact& other) const{
        return connectedName < other.connectedName;
    }
};

// helper function that takes in a string filename and returns a vector of contacts (unsorted)
template <typename o>
vector<o> readHelper(const string& filename){
    ifstream myfile(filename);
    string line; 
    vector<o> contacts;
    while(getline(myfile,line)){
        stringstream ss(line);
        string word;
        int wordCount = 0;
        string info[4];
        
        while (ss >> word){
            for (char& c : word){ // turning input into uppercase
                c = toupper(c);
            }
            info[wordCount] = word;
            wordCount++;

        }   
        o contact;
        contact.firstName = info[0];
        contact.lastName = info[1];
        contact.number = info[2];
        contact.location = info[3];
        contact.connectedName = contact.firstName + ' ' + contact.lastName;
        contacts.push_back(contact); 
    }
              
        return contacts;
}

// a template based class that will be used to store the contacts, and all sorting and search algorithms will be implemented
template <typename o>
class ContactClass{
public:
    vector<o> contacts;

    // constructor that takes in a filename and reads the file
    ContactClass(const string& filename){
        contacts = readHelper<o>(filename);
    }

    // helper function of insertion sort
    void insertionSortHelper(vector<o>& sortedContacts, int left, int right){
        for (int i = left + 1; i <= right; i++){
            o temp = sortedContacts[i];
            int j = i - 1;
            while (j >= left && sortedContacts[j] > temp){
                sortedContacts[j + 1] = sortedContacts[j];
                j--;
            }
            sortedContacts[j + 1] = temp;
        }
    }

    // insertion sort function that returns the sorted vector
    vector<o> insertionSort(){
        vector<o> sortedContacts = contacts;
        insertionSortHelper(sortedContacts, 0, sortedContacts.size() - 1);
        return sortedContacts;
    }

    // swap function that swaps two elements in a vector
    void swap(o& a, o& b){
        o temp = a;
        a = b;
        b = temp;
    }

    // function that finds the median of a given sub vector
    int medianFind(vector<o>& sortedContacts, int left, int right){
        int mid = (left + right) / 2;
        if (sortedContacts[left] > sortedContacts[mid]){
            swap(sortedContacts[left], sortedContacts[mid]);
        }
        if (sortedContacts[left] > sortedContacts[right]){
            swap(sortedContacts[left], sortedContacts[right]);
        }
        if (sortedContacts[mid] > sortedContacts[right]){
            swap(sortedContacts[mid], sortedContacts[right]);
        }
        swap(sortedContacts[mid], sortedContacts[right - 1]);
        return right - 1;
    }

    // particion function that returns the pivot index
    int partition(vector<o>& sortedContacts, int left, int right){
        int pivotIndex = medianFind(sortedContacts, left, right);
        o pivot = sortedContacts[pivotIndex];
        int i = left;
        int j = right - 1;
        while (true){
            while (sortedContacts[++i] < pivot){}
            while (sortedContacts[--j] > pivot){}
            if (i < j){
                swap(sortedContacts[i], sortedContacts[j]);
            }
            else{
                break;
            }
        }
        swap(sortedContacts[i], sortedContacts[right - 1]);
        return i;
    }


    // helper function of quicksort 
    void quickSortHelper(vector<o>& sortedContacts, int left, int right){
        if (left < right){
            if (right - left <= 10){
                insertionSortHelper(sortedContacts, left, right);
            }
            else{
                int pivotIndex = partition(sortedContacts, left, right);
                quickSortHelper(sortedContacts, left, pivotIndex - 1);
                quickSortHelper(sortedContacts, pivotIndex + 1, right);
            }
        }
    }

    // quick sort function that returns the sorted vector 
    // uses median as pivot (choose 3 random elements and take the median), and uses insertion sort for vectors size <10
    vector<o> quickSort(){
        vector<o> sortedContacts = contacts;
        quickSortHelper(sortedContacts, 0, sortedContacts.size() - 1);
        return sortedContacts;
    }

    
    // merge function that merges two sorted vectors
    void merge(vector<o>& sortedContacts, int left, int mid, int right){
        int i = left;
        int j = mid + 1;
        while (i <= mid && j <= right){
            if (sortedContacts[i] < sortedContacts[j]){
                i++;
            }
            else{
                o temp = sortedContacts[j];
                for (int k = j; k > i; k--){
                    sortedContacts[k] = sortedContacts[k - 1];
                }
                sortedContacts[i] = temp;
                i++;
                j++;
                mid++;
            }
        }
    }

    // helper function of merge sort that recursively calls itself
    void mergeSortHelper(vector<o>& sortedContacts, int left, int right){
        if (left < right){
            int mid = (left + right) / 2;
            mergeSortHelper(sortedContacts, left, mid);
            mergeSortHelper(sortedContacts, mid + 1, right);
            merge(sortedContacts, left, mid, right);
        }
    }

    // merge sort function that returns the sorted vector
    vector<o> mergeSort(){
        vector<o> sortedContacts = contacts;
        mergeSortHelper(sortedContacts, 0, sortedContacts.size() - 1);
        return sortedContacts;
    }

    // helper function of heap sort
    void heapify(vector<o>& sortedContacts, int n, int i){
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && sortedContacts[left] > sortedContacts[largest]){
            largest = left;
        }
        if (right < n && sortedContacts[right] > sortedContacts[largest]){
            largest = right;
        }
        if (largest != i){
            swap(sortedContacts[i], sortedContacts[largest]);
            heapify(sortedContacts, n, largest);
        }
    }

    // heap sort function that returns the sorted vector
    vector<o> heapSort(){
        vector<o> sortedContacts = contacts;
        int n = sortedContacts.size();
        for (int i = n / 2 - 1; i >= 0; i--){
            heapify(sortedContacts, n, i);
        }
        for (int i = n - 1; i >= 0; i--){
            swap(sortedContacts[0], sortedContacts[i]);
            heapify(sortedContacts, i, 0);
        }
        return sortedContacts;
    }
};

// sequential search function that prints the contact information if found and returns "Not Found" otherwise
template <typename o>
void sequentialSearch(string name, vector<o> contacts){
    int ctr = 0;
    for (int i = 0; i < contacts.size(); i++){
        if (contacts[i].connectedName.find(name) == 0){
            cout << contacts[i].connectedName + " " + contacts[i].number + " " + contacts[i].location << endl;
            ctr++;
        }  
        
    }
    if (ctr == 0){
        cout << name << " " << "Does NOT exist in the dataset";
        cout << endl;
    }
    return;
}

// sequential search function but does not print output, did in order to measure time of sequential search precisely inside loop
template <typename o>
void NONPRINTsequentialSearch(string name, vector<o> contacts){
    for (int i = 0; i < contacts.size(); i++){
        if (contacts[i].connectedName.find(name) == 0){
            continue;
        }
    }
    return;
}

// Binary search function that prints the contact information if found and returns "Not Found" otherwise
template <typename T>
void binarySearch(const string& name, const vector<T>& contacts) {
    int left = 0;
    int right = contacts.size() - 1;
    int checker = 0;

    while (left <= right && checker == 0) {
        int mid = left + (right - left) / 2;

        if (contacts[mid].connectedName.find(name) == 0) {
            // need to check for other values that are found by the searched input, therefore upgrading the default binary search function
            left = mid - 1;
            // need to invert left side print order in order to print in ascending order vector<string> temp;
            vector<string> temp;
            while(left>= 0 && contacts[left].connectedName.find(name) == 0){ // checking for left side
                string temp2 =  contacts[left].connectedName + " " + contacts[left].number + " " + contacts[left].location;
                temp.push_back(temp2);
                left--;
            }
            // printing the correctly ordered left side
            for (int i = temp.size() - 1; i >= 0; i--){
                cout << temp[i] << endl;
            }

            cout << contacts[mid].connectedName + " " + contacts[mid].number + " " + contacts[mid].location << endl; // printing the middle value

            // checking for right side
            right = mid + 1;
            while(right < contacts.size() && contacts[right].connectedName.find(name) == 0){
                cout << contacts[right].connectedName + " " + contacts[right].number + " " + contacts[right].location << endl;
                right++;
            }

                checker++;
        }
        else if (contacts[mid].connectedName > name) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }

    if (checker == 0) {
        cout << name << " Does NOT exist in the dataset" << endl;
    }
}

// non print binary search
template <typename T>
void NONPRINTbinarySearch(const string& name, const vector<T>& contacts) {
    int left = 0;
    int right = contacts.size() - 1;
    int checker = 0;

    while (left <= right && checker == 0) {
        int mid = left + (right - left) / 2;

        if (contacts[mid].connectedName.find(name) == 0) {
            // need to check for other values that are found by the searched input, therefore upgrading the default binary search function
            left = mid - 1;
            // checking for left side
            while(left>= 0 && contacts[left].connectedName.find(name) == 0){ // checking for left side
                left--;
            }

            // checking for right side
            right = mid + 1;
            while(right < contacts.size() && contacts[right].connectedName.find(name) == 0){
                right++;
            }
            checker++;
        }
        else if (contacts[mid].connectedName > name) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
}



int main(){

    //enter the filename as user input
	string filename;
    string inputName;
	cout<< "Please enter the contact file name: " << endl;
	cin>>filename;
    cout << "Please enter the word to be queried: " << endl;
    getline(cin>>ws, inputName);
    cout << endl;
    cout << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;
    // turning the input name to uppercase
    for (int i = 0; i < inputName.length(); i++)
    {
        if (isalpha(inputName.at(i)))
        {
            inputName.at(i) = toupper(inputName.at(i));
        }
    }


    // creating the ContactClass object with the given filename
    ContactClass<Contact> contacts(filename);

    // insertion sort and checking time
    auto start = high_resolution_clock::now();
    vector<Contact> quickSortedContacts = contacts.quickSort();
    auto quickTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    cout << "Quick Sort Time: " << quickTime.count() << " Nanoseconds" << endl;

    // quick sort and checking time
    start = high_resolution_clock::now();
    vector<Contact> insertionSortedContacts = contacts.insertionSort();
    auto insertionTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    cout << "Insertion Sort Time: " << insertionTime.count() << " Nanoseconds" << endl;


    // merge sort and checking time
    start = high_resolution_clock::now();
    vector<Contact> mergeSortedContacts = contacts.mergeSort();
    auto mergeTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    cout << "Merge Sort Time: " << mergeTime.count() << " Nanoseconds" << endl;

    // heap sort and checking time
    start = high_resolution_clock::now();
    vector<Contact> heapSortedContacts = contacts.heapSort();
    auto heapTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    cout << "Heap Sort Time: " << heapTime.count() << " Nanoseconds" << endl;
    cout << endl;

    /* the code below is for checking whether the vectors are sorted correctly or not for each sorting algorithm
    // printing the sorted vectors
    cout << "Insertion Sort: " << endl;
    for (int i = 0; i < insertionSortedContacts.size(); i++){
        cout << insertionSortedContacts[i].firstName << ' ' << insertionSortedContacts[i].lastName << ' ' << insertionSortedContacts[i].number << ' ' << insertionSortedContacts[i].location << endl;
    }
    cout << endl;
    cout << "Quick Sort: " << endl;
    for (int i = 0; i < quickSortedContacts.size(); i++){
        cout << quickSortedContacts[i].firstName << ' ' << quickSortedContacts[i].lastName << ' ' << quickSortedContacts[i].number << ' ' << quickSortedContacts[i].location << endl;
    }
    cout << endl;
    cout << "Merge Sort: " << endl;
    for (int i = 0; i < mergeSortedContacts.size(); i++){
        cout << mergeSortedContacts[i].firstName << ' ' << mergeSortedContacts[i].lastName << ' ' << mergeSortedContacts[i].number << ' ' << mergeSortedContacts[i].location << endl;
    }
    cout << endl;
    cout << "Heap Sort: " << endl;
    for (int i = 0; i < heapSortedContacts.size(); i++){
        cout << heapSortedContacts[i].firstName << ' ' << heapSortedContacts[i].lastName << ' ' << heapSortedContacts[i].number << ' ' << heapSortedContacts[i].location << endl;
    }
    cout << endl;

    // printing the original file 
    cout << "Original File: " << endl;
    for (int i = 0; i < contacts.contacts.size(); i++){
        cout << contacts.contacts[i].firstName << ' ' << contacts.contacts[i].lastName << ' ' << contacts.contacts[i].number << ' ' << contacts.contacts[i].location << endl;
    }*/
    

    cout << "Searcing for: " << inputName << endl;
    cout << "======================================" << endl;

    // binary search and checking time
    int M = 100; // searching 100 times inside loop without printing, then calling the function once more and printing the output
    start = high_resolution_clock::now();
    for (int i = 0; i < M; i++){
        NONPRINTbinarySearch(inputName, quickSortedContacts);
    }
    auto binaryTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    binarySearch(inputName, quickSortedContacts);
    cout << "Binary Search Time: " << binaryTime.count() / M << " nanoseconds" << endl;
    cout << endl;

    // sequential search and checking time
    cout << "Search results for Sequential Search:" << endl;
    int N = 100; // searching 100 times inside loop without printing, then calling the function once more and printing the output
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++){
        NONPRINTsequentialSearch(inputName, quickSortedContacts);
    }
    auto sequentialTime = duration_cast<nanoseconds>(high_resolution_clock::now() - start);
    sequentialSearch(inputName, quickSortedContacts);
    cout << "Sequential Search Time: " << sequentialTime.count() / N << " nanoseconds" << endl;
    cout << endl;
    cout << "SpeedUp between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << (sequentialTime.count() * M) / (binaryTime.count() * N) << endl; // adding the M and N values to the denominator to take the loops into account
    cout << endl;
    cout << "SpeedUps between Sorting Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << float(insertionTime.count()) / float(quickTime.count()) << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << float(mergeTime.count()) / float(quickTime.count()) << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp =  " << float(heapTime.count()) / float(quickTime.count()) << endl;

    return 0;
}