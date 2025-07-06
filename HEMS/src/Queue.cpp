#include "Queue.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void initQueue(PriorityQueue &q) {
    q.front = nullptr;
}

void addPatient(PriorityQueue &q, string name, int age, string condition, int priority) {
    Patient newPatient(name, age, condition, priority);
    Node* newNode = new Node{newPatient, nullptr};

    if (q.front == nullptr || priority < q.front->data.priority) {
        newNode->next = q.front;
        q.front = newNode;
    } else {
        Node* current = q.front;
        while (current->next != nullptr && current->next->data.priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    cout << "Patient added successfully.\n";
}

bool treatNextPatient(PriorityQueue &q, Patient &treated) {
    if (q.front == nullptr) {
        cout << "No patients in the queue.\n";
        return false;
    }

    Node* treatedNode = q.front;
    q.front = q.front->next;

    treated = treatedNode->data;
    delete treatedNode;

    cout << "Treated patient: " << treated.name << endl;
    return true;
}

void displayQueue(const PriorityQueue &q) {
    if (q.front == nullptr) {
        cout << "No patients in the queue.\n";
        return;
    }

    cout << "\n--- Waiting Patients ---\n";
    cout << left << setw(20) << "Name"
         << setw(6) << "Age"
         << setw(20) << "Condition"
         << setw(10) << "Priority" << endl;
    cout << string(56, '-') << endl;

    Node* current = q.front;
    while (current != nullptr) {
        cout << left << setw(20) << current->data.name
             << setw(6) << current->data.age
             << setw(20) << current->data.condition;

        switch (current->data.priority) {
            case 1: cout << setw(10) << "Critical"; break;
            case 2: cout << setw(10) << "Moderate"; break;
            case 3: cout << setw(10) << "Mild"; break;
            default: cout << setw(10) << "Unknown"; break;
        }
        cout << endl;
        current = current->next;
    }
    cout << "------------------------\n";
}

bool isEmpty(const PriorityQueue &q) {
    return q.front == nullptr;
}

void freeQueue(PriorityQueue &q) {
    while (q.front != nullptr) {
        Node* temp = q.front;
        q.front = q.front->next;
        delete temp;
    }
}

void saveQueueToFile(const PriorityQueue &q, const string &filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Failed to open file for saving: " << filename << endl;
        return;
    }

    Node* current = q.front;
    while (current != nullptr) {
        out << current->data.name << ','
            << current->data.age << ','
            << current->data.condition << ','
            << current->data.priority << '\n';
        current = current->next;
    }

    out.close();
    cout << "Queue saved to file: " << filename << endl;
}

void loadQueueFromFile(PriorityQueue &q, const string &filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "No existing data file found: " << filename << " (starting fresh)\n";
        return;
    }

    string name, condition, line;
    int age, priority;

    while (getline(in, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

        name = line.substr(0, pos1);
        age = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        condition = line.substr(pos2 + 1, pos3 - pos2 - 1);
        priority = stoi(line.substr(pos3 + 1));

        addPatient(q, name, age, condition, priority);
    }

    in.close();
    cout << "Queue loaded from file: " << filename << endl;
}
