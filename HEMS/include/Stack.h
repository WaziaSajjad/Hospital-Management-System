// #ifndef STACK_H
// #define STACK_H

// #include "Patient.h"
// #include <iostream>
// #include <iomanip>
// #include <fstream>
// using namespace std;

// // Node for stack linked list
// struct StackNode {
//     Patient data;
//     StackNode* next;
// // };

// struct Stack {
//     StackNode* top;
// };

// // Initialize stack
// void initStack(Stack &s) {
//     s.top = nullptr;
// }

// // Push patient onto stack
// void push(Stack &s, Patient p) {
//     StackNode* newNode = new StackNode{p, s.top};
//     s.top = newNode;
//     cout << "Patient pushed to discharged stack: " << p.name << endl;
// }

// // Pop patient from stack
// bool pop(Stack &s, Patient &popped) {
//     if (s.top == nullptr) {
//         cout << "Discharged stack is empty.\n";
//         return false;
//     }

//     StackNode* temp = s.top;
//     popped = temp->data;
//     s.top = s.top->next;
//     delete temp;

//     cout << "Patient popped from discharged stack: " << popped.name << endl;
//     return true;
// }

// // Display stack contents
// void displayStack(const Stack &s) {
//     if (s.top == nullptr) {
//         cout << "Discharged stack is empty.\n";
//         return;
//     }

//     cout << "\n--- Discharged Patients (Most Recent First) ---\n";
//     cout << left << setw(20) << "Name"
//          << setw(6) << "Age"
//          << setw(20) << "Condition"
//          << setw(10) << "Priority" << endl;
//     cout << string(56, '-') << endl;

//     StackNode* current = s.top;
//     while (current != nullptr) {
//         cout << left << setw(20) << current->data.name
//              << setw(6) << current->data.age
//              << setw(20) << current->data.condition;

//         switch (current->data.priority) {
//             case 1: cout << setw(10) << "Critical"; break;
//             case 2: cout << setw(10) << "Moderate"; break;
//             case 3: cout << setw(10) << "Mild"; break;
//             default: cout << setw(10) << "Unknown"; break;
//         }
//         cout << endl;
//         current = current->next;
//     }
//     cout << "------------------------\n";
// }

// // Free all nodes in stack
// void freeStack(Stack &s) {
//     while (s.top != nullptr) {
//         StackNode* temp = s.top;
//         s.top = s.top->next;
//         delete temp;
//     }
// }

// // 🔽 Save the stack to file (write from bottom to top for chronological order)
// void saveStackToFile(const Stack &s, const string &filename) {
//     // To write in order from oldest to newest, we need to reverse the stack temporarily
//     // First, collect nodes in a vector to reverse them
//     vector<Patient> tempVec;
//     StackNode* current = s.top;
//     while (current != nullptr) {
//         tempVec.push_back(current->data);
//         current = current->next;
//     }

//     ofstream out(filename);
//     if (!out) {
//         cerr << "Failed to open file for saving: " << filename << endl;
//         return;
//     }

//     // Write from oldest to newest (reverse vector)
//     for (auto it = tempVec.rbegin(); it != tempVec.rend(); ++it) {
//         out << it->name << ','
//             << it->age << ','
//             << it->condition << ','
//             << it->priority << '\n';
//     }

//     out.close();
//     cout << "Stack saved to file: " << filename << endl;
// }

// // 🔼 Load the stack from file
// void loadStackFromFile(Stack &s, const string &filename) {
//     ifstream in(filename);
//     if (!in) {
//         cerr << "No existing stack data file found: " << filename << " (starting fresh)\n";
//         return;
//     }

//     string name, condition, line;
//     int age, priority;

//     // We'll push patients in order they appear (oldest first)
//     // but since stack pushes on top, this will keep original order
//     vector<Patient> tempVec;

//     while (getline(in, line)) {
//         size_t pos1 = line.find(',');
//         size_t pos2 = line.find(',', pos1 + 1);
//         size_t pos3 = line.find(',', pos2 + 1);

//         if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

//         name = line.substr(0, pos1);
//         age = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
//         condition = line.substr(pos2 + 1, pos3 - pos2 - 1);
//         priority = stoi(line.substr(pos3 + 1));

//         tempVec.push_back(Patient(name, age, condition, priority));
//     }

//     // Push patients in order to preserve stack order (oldest at bottom)
//     for (const Patient& p : tempVec) {
//         push(s, p);
//     }

//     in.close();
//     cout << "Stack loaded from file: " << filename << endl;
// }

// #endif
