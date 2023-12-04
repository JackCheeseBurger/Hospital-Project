#include <string>
#include <iostream>
#include <iomanip>
#include "Transaction.h"

extern transactionLogs HeartTransactionLog;
extern transactionLogs LungTransactionLog;
extern transactionLogs PlasticTransactionLog;

#ifndef QUEUE_H
#define QUEUE_H
struct Patient {
    std::string FirstName;
    std::string LastName;
    std::string SSN;
    bool isCritical;
    Patient(std::string first, std::string last, std::string ssn, bool critical)
        : FirstName(std::move(first)), LastName(std::move(last)), SSN(std::move(ssn)), isCritical(critical) {}
};

                        // Basic queue implementation with other nessecary functions added.
class Node { 
public:            
    Patient data;
    Node* next;

    Node(Patient value) : data(std::move(value)), next(nullptr) {} // Used a class with a constructor for efficiency and code clarity.
};

class Clinic {
private:
    Node* front;
    Node* rear;
    int size;

public:         // Pre\Post-Conditions for member functions are in the Clinic.cpp file:
    std::string name;
    Clinic(std::string n);
    ~Clinic();
    
    // Clinic Elements:
    void AddPatient(Patient newPatient);  // Pretty much enqueue with added restricitions/handling,
    void AddCriticallyIllPatient(Patient newCritPatient);
    bool cancelPatient(std::string& targetSSN);
    void listAllPatients();
    std::string listRescheduledPatients();
    // Default Queue Elements:
    void dequeue();  // Default dequeue used in operate and in the destructor,
    Patient peek();
    bool isEmpty();
    bool DepartmentisFull();
};

#endif