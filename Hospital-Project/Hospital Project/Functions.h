#pragma once
#include <string>
#include "Transaction.h"
#include "Clinic.h"

//(loadScheduledPatients):Pre-condition:
    // None(the function can be called in any state).
void loadScheduledPatients(Clinic& cHeart, Clinic& cLung, Clinic& cPlastic);
//(loadScheduledPatients):Post-condition:
    // Each Clinic Queue loaded with patients

//(MainMenu):Pre-condition:
    // Clinics must be initialized and have patients
void MainMenu(Clinic& cHeart, Clinic& cLung, Clinic& cPlastic);
//(MainMenu):Post-condition:
    // Screen is Cleared and the MainMenu is printed,
    // User input is recieved and the proper SubMenu is called.
 
//(SubMenu):Pre-condition:
    // Clinics  must be initialized and have patients.
void SubMenu(const std::string& clinicName, Clinic& clinic);
//(SubMenu):Post-condition:
    // Clears Screen and SubMenu is Printed
    // User input is recieved and proper Clinic member function or exit called,
    // All related info printied for function called,
    // Recalls SubMenu or calls MainMenu

void list_reschedules();