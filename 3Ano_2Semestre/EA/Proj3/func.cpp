#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;



void printVector(vector<int> vec) {
    cout << '[';
    for (int i = 0; i < (int) vec.size(); i++) {
        cout << vec[i];
        if (i != (int) vec.size() - 1)
            cout << ", "; 
    
    }
    cout << ']'<<endl;
}


void printVectorDepends(vector<struct Operation*> vec) {
    cout << '[';
    for (int i = 0; i < (int) vec.size(); i++) {
        cout << vec[i]->id;
        if (i != (int) vec.size() - 1)
            cout << ", "; 
    
    }
    cout << ']'<<endl;
}

void printOperations(vector<struct Operation*> operations) {
    for (int i = 0; i < (int) operations.size(); i++) {
        cout << operations[i]->id << " == " << operations[i]->cost << ": ";
        printVector(operations[i]->dependenciesIds);
        printVectorDepends(operations[i]->parents);
        printVectorDepends(operations[i]->childrens);

    }
}