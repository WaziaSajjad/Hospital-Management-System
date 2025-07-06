#ifndef QUEUE_H
#define QUEUE_H

#include "Patient.h"
#include <string>

// Node for linked list
struct Node {
    Patient data;
    Node* next;
};

struct PriorityQueue {
    Node* front;
};

// Function declarations
void initQueue(PriorityQueue &q);
void addPatient(PriorityQueue &q, std::string name, int age, std::string condition, int priority);
bool treatNextPatient(PriorityQueue &q, Patient &treated);
void displayQueue(const PriorityQueue &q);
bool isEmpty(const PriorityQueue &q);
void freeQueue(PriorityQueue &q);
void saveQueueToFile(const PriorityQueue &q, const std::string &filename);
void loadQueueFromFile(PriorityQueue &q, const std::string &filename);

#endif
