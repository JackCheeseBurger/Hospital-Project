#include "Clinic.h"

transactionLogs HeartTransactionLog;
transactionLogs LungTransactionLog;
transactionLogs PlasticTransactionLog;

//Constructor:
Clinic::Clinic(std::string n) : front(nullptr), rear(nullptr), size(0), name(n) {}


//Destructor:
    // Dequeues everything in the queue.
Clinic::~Clinic() {
    while (!isEmpty()) {
        dequeue();
    }
}

// MEMBER FUNCTIONS:

//(AddPatient):Pre-condition:
    // None(the function can be called in any state).
void Clinic::AddPatient(Patient newPatient) {
    if (DepartmentisFull()) {
        HeartTransactionLog.saveToLog("Error", "Error failed to add patient: " + newPatient.FirstName + ' ' + newPatient.LastName);
        // TODO: ADD TRANSACTION - PATIENT CANCELLED DEPARTMENT WAS FULL
        return;
    }
    if (newPatient.SSN.empty()) {
        HeartTransactionLog.saveToLog("Error", "Error failed to add patient: " + newPatient.FirstName + ' ' + newPatient.LastName);
        // TODO: ADD TRANSACTION - PATIENT CANCELLED NO SSN
        return;
    }

    Node* newNode = new Node(newPatient);

    if (isEmpty()) {
        // If the queue is empty, the new node becomes both front and rear.
        front = rear = newNode;
    }
    else if (newPatient.isCritical) {
        // If the new patient is critical, insert it at the front.
        newNode->next = front;
        front = newNode;
    }
    else if (front->data.isCritical) {
        // If the front patient is critical, find the first non-critical patient
        // and insert the new node before it.
        Node* current = front;
        while (current->next != nullptr && current->next->data.isCritical) {
            current = current->next;
        }

        // Insert the new node after the last critical patient or at the rear if there are none.
        newNode->next = current->next;
        current->next = newNode;

        // Update rear if the new node was added at the end.
        if (newNode->next == nullptr) {
            rear = newNode;
        }
    }
    else {
        // If the front patient is not critical, add the new node at the rear.
        rear->next = newNode;
        rear = newNode;
    }
    if (name == "Heart") {          // Transaction Logging.
        HeartTransactionLog.saveToLog("Added", newPatient.FirstName + ' ' + newPatient.LastName + ' ' + newPatient.SSN);
    }else if (name == "Lung") {
        LungTransactionLog.saveToLog("Added", newPatient.FirstName + ' ' + newPatient.LastName + ' ' + newPatient.SSN);
    }
    else if (name == "Plastic") {
        PlasticTransactionLog.saveToLog("Added", newPatient.FirstName + ' ' + newPatient.LastName + ' ' + newPatient.SSN);
    }
    ++size;
}
//(AddPatient):Post-condition:
    // A new node containing the new patient is added to the queue.
    // If the queue was empty before the operation, both front and rear now point to the new node.
    // If the new patient is critical, it is inserted at the front.
    // If the front patient is critical, the new patient is inserted after the last critical patient but before non-critical ones.
    // If the front patient is not critical, the new patient is added at the rear.


//(AddCriticallyIllPatient):Pre-condition:
    // None(the function can be called in any state).

void Clinic::AddCriticallyIllPatient(Patient newCritPatient) {
    newCritPatient.isCritical = true; //Just ensures the flag is set properly kind of unnecessary.
    AddPatient(newCritPatient);
}
//(AddCriticallyIllPatient):Post-condition:
    // AddPatient called and new patient is set to critical.


//(cancelPatient):Pre-condition:
    // The clinic is not empty.
    // The targetSSN parameter is a valid SSN
bool Clinic::cancelPatient(std::string& targetSSN) {
    Node* current = front;
    Node* prev = nullptr;

    while (current != nullptr) {
        Patient currentPatient = current->data;

        // Check if the SSN matches the target SSN.
        if (currentPatient.SSN == targetSSN) {
            // Found the patient to cancel, remove the node from the queue.
            if (prev == nullptr) {
                // The canceled patient is at the front of the queue.
                front = current->next;
            }
            else {
                // The canceled patient is in the middle or at the rear of the queue.
                prev->next = current->next;

                // Update rear if the canceled patient was at the rear.
                if (current->next == nullptr) {
                    rear = prev;
                }
            }

            // Delete the node and update size.
            if (name == "Heart") {          // Transaction Logging.
                HeartTransactionLog.saveToLog("Canceled", currentPatient.FirstName + ' ' + currentPatient.LastName + ' ' + currentPatient.SSN);
            }
            else if (name == "Lung") {
                LungTransactionLog.saveToLog("Canceled", currentPatient.FirstName + ' ' + currentPatient.LastName + ' ' + currentPatient.SSN);
            }
            else if (name == "Plastic") {
                PlasticTransactionLog.saveToLog("Canceled", currentPatient.FirstName + ' ' + currentPatient.LastName + ' ' + currentPatient.SSN);
            }
            delete current;
            --size;
            return true;  // Exit the function after canceling the patient.
        }

        // Move to the next node.
        prev = current;
        current = current->next;
    }
    return false; // SSN NOT FOUND.
}
//(cancelPatient):Post-condition:
    // If a patient with the specified SSN is found in the clinic.
    // The clinic and its nods are fixed to adjust for the lost node.



void Clinic::listAllPatients() {
    Node* current = front;

    std::cout << std::left << std::setw(20) << "First Name"
        << std::setw(20) << "Last Name"
        << std::setw(15) << "SSN"
        << std::setw(10) << "Status" << std::endl;
    std::cout << std::setfill('-') << std::setw(65) << "" << std::setfill(' ') << std::endl;

    while (current != nullptr) {
        Patient currentPatient = current->data;

        // Display patient information
        std::cout << std::left << std::setw(20) << currentPatient.FirstName
            << std::setw(20) << currentPatient.LastName
            << std::setw(15) << currentPatient.SSN
            << std::setw(10) << (currentPatient.isCritical ? "Critical" : "Reg") << std::endl;

        // Move to the next node
        current = current->next;
    }
}

std::string Clinic::listRescheduledPatients() {
    std::string retString = "";
    Node* current = front;
    
    while (current != nullptr) {
        Patient currentPatient = current->data;

        // Return patient information
        retString += '\t' + currentPatient.FirstName + ' ' + currentPatient.LastName + ' ' + currentPatient.SSN + ' ' + (currentPatient.isCritical ? "Critical" : "Reg") + "\n";

        // Move to the next node
        current = current->next;
    }
    return retString;
}


//(dequeue):Pre-condition:
    // The queue is not empty.
void Clinic::dequeue() {
    if (isEmpty()) {
        // Queue is empty, nothing to dequeue.
        return;
    }

    Node* temp = front;
    front = front->next;
    delete temp;

    if (front == nullptr) {
        // Queue is now empty, update rear to nullptr.
        rear = nullptr;
    }
    --size;
}
//(dequeue):Post-condition:
    // The front node of the queue is removed.
    // If the removed node was the last node in the queue, both front and rear are set to nullptr.


//(peek):Pre-condition:
    // The queue is not empty.
Patient Clinic::peek() {
    if (isEmpty()) {
        //TODO Queue is empty do something
    }
    return front->data;
}
//(peek):Post-condition:
    // The value of the front node is returned.
    // The state of the queue remains unchanged(no modification to the structure).


bool Clinic::isEmpty() {
    return front == nullptr;
}

bool Clinic::DepartmentisFull() {
    return size >= 10;
}