#include <iostream>
#include <fstream>
#include <sstream>
#include "Functions.h"
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>

void loadScheduledPatients(Clinic& cHeart, Clinic& cLung, Clinic& cPlastic) {
    std::ifstream inFile("patient.csv");
    std::string temp;
    try {
        if (!inFile.is_open()) {
            throw std::runtime_error("Critical Error: Unable to open file(Input file name should be 'patient.csv')");
        }

        std::string clinicName, line, patientFName, patientLName, patientSSN;
        int lineCount = 0;

        while (getline(inFile, line)) {
            lineCount++;

            try {
                std::stringstream lineStream(line);

                // Read clinicName
                if (!getline(lineStream, clinicName, ',')) {
                    throw std::runtime_error("File Error at line " + std::to_string(lineCount) + ": Invalid data format - clinicName");
                }

                // Read patientFName
                if (!getline(lineStream, patientFName, ',')) {
                    throw std::runtime_error("File Error at line " + std::to_string(lineCount) + ": Invalid data format - patientFName");
                }

                // Read patientLName, handle case where there might not be a last name
                if (!getline(lineStream, patientLName, ',')) {
                    patientLName = ""; // Set last name to an empty string if it's not present
                }

                // Read patientSSN
                if (!getline(lineStream, patientSSN)) {
                    throw std::runtime_error("File Error at line " + std::to_string(lineCount) + ": Invalid data format - patientSSN");
                }

                // Trim whitespaces
                clinicName.erase(std::remove_if(clinicName.begin(), clinicName.end(), std::isspace), clinicName.end());
                patientFName.erase(std::remove_if(patientFName.begin(), patientFName.end(), std::isspace), patientFName.end());
                patientLName.erase(std::remove_if(patientLName.begin(), patientLName.end(), std::isspace), patientLName.end());
                patientSSN.erase(std::remove_if(patientSSN.begin(), patientSSN.end(), std::isspace), patientSSN.end());

                // Check if any of the data is empty, indicating an issue with the CSV format
                if (clinicName.empty() || patientFName.empty() || patientSSN.empty()) {
                    throw std::runtime_error("File Error at line " + std::to_string(lineCount) + ": Empty field in data");
                }

                Patient nPatient(patientFName, patientLName, patientSSN, false);
                switch (clinicName.at(0)) {
                case 'H':
                    cHeart.AddPatient(nPatient);
                    break;
                case 'L':
                    cLung.AddPatient(nPatient);
                    break;
                case 'P':
                    cPlastic.AddPatient(nPatient);
                    break;
                case 'Q':
                    break;
                default:
                    temp = "File Error at line  " + lineCount;
                    temp += ": Unknown clinic type : " + line;
                    HeartTransactionLog.saveToLog("Error_Startup", temp);
                    break;
                }
            }
            catch (const std::exception& e) {
                HeartTransactionLog.saveToLog("Error_Startup", e.what());
            }
        }

        inFile.close();
    }
    catch (const std::exception& e) {
        HeartTransactionLog.saveToLog("Error_Startup", e.what());
    }
}

void MainMenu(Clinic& cHeart, Clinic& cLung, Clinic& cPlastic) { // MAIN MENU
    system("CLS");
    std::string input;
    while (true) {
        std::cout << "   WELCOME TO RESEARCH HOSPITAL\n";
        std::cout << "1:  Heart Clinic\n";
        std::cout << "2:  Lung Clinic\n";
        std::cout << "3:  Plastic Surgery\n";
        std::cout << "4:  Quit\n";
        std::cout << "Please enter the clinic of interest: ";
        std::cin >> input;
        if(input.size() == 1 && isdigit(input.at(0))) {
            int inp = stoi(input);
            switch (inp) {
            case 1:
                SubMenu("the Heart Clinic", cHeart);
                break;
            case 2:
                SubMenu("the Lung Clinic", cLung);
                break;
            case 3:
                SubMenu("Plastic Surgery", cPlastic);
                break;
            case 4:
                transactionLogs::endDay(HeartTransactionLog, LungTransactionLog, PlasticTransactionLog);
                return;
                break;
            default:
                break;
            }
        }
    }
}

void SubMenu(const std::string& clinicName, Clinic& clinic) { // SUB MENU
    system("CLS");
    std::string input, targSSN, temp;
    Patient p("", "", "", false);
    Patient f("", "", "", false);
    while (true) {
        std::cout << "Welcome to " + clinicName + "\n";
        std::cout << "1: - Add Patient\n";
        std::cout << "2: - Add Critical Patient\n";
        std::cout << "3: - Take out Patient for Operation\n";
        std::cout << "4: - Cancel Patient\n";
        std::cout << "5: - List Patients in Queue\n";
        std::cout << "6: - Change Departmet/Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> input;
        if (input.size() == 1 && isdigit(input.at(0))) {
            int inp = stoi(input);
            switch (inp) {
            case 1:                         // CASE 1: ADD PATIENT TO CHOSEN CLINIC AND CHECK FOR SSN INPUT ERROR
                system("CLS");
                if (clinic.DepartmentisFull() == true) {
                    std::cout << "Patients cannot be added, Clinic is full attempt saved to Transaction Log.";
                    HeartTransactionLog.saveToLog("Error", "Failed to Add Patient Manually: " + p.FirstName + ' ' + p.LastName);
                    break;
                }
                std::cout << "Enter the Patients First Name: ";
                std::cin >> p.FirstName;
                std::cout << "Enter the Patients Last Name: ";
                std::cin >> p.LastName;
                std::cout << "Enter the Patients SSN: ";
                std::cin >> p.SSN;
                if (!std::all_of(p.SSN.begin(), p.SSN.end(), [](unsigned char c) { return std::isdigit(c); }) && p.SSN.size() != 1) {
                    std::cout << "\nPatients SSN invalid 'Add Patient' Canceled.";
                    HeartTransactionLog.saveToLog("Error", "Failed to Add Patient Manually: " + p.FirstName + ' ' + p.LastName);
                    std::cin.ignore();
                    std::cin.get();
                    system("CLS");
                    break;
                }
                clinic.AddPatient(p);
                std::cout << "\nAdded: " << p.FirstName << " " << p.LastName << " " << p.SSN << "\n";
                std::cout << "\n";
                std::cin.ignore();
                std::cin.get();
                system("CLS");
                break;
            case 2:                         // CASE 2: ADD CRITICAL PATIENT TO CHOSEN CLINIC AND CHECK FOR SSN INPUT ERROR (MOSTLY SAME AS CASE 1)
                system("CLS");
                if (clinic.DepartmentisFull() == true) {
                    std::cout << "Critical Patients cannot be added, Clinic is full attempt saved to Transaction Log.";
                    break;
                }
                std::cout << "Enter the Critical Patients First Name: ";
                std::cin >> p.FirstName;
                std::cout << "Enter the Critical Patients Last Name: ";
                std::cin >> p.LastName;
                std::cout << "Enter the Critical Patients SSN: ";
                std::cin >> p.SSN;
                if (!std::all_of(p.SSN.begin(), p.SSN.end(), [](unsigned char c) { return std::isdigit(c); }) && p.SSN.size() != 1) {
                    std::cout << "\nCritical Patients SSN invalid 'Add Critical Patient' Canceled.";
                    std::cin.ignore();
                    std::cin.get();
                    system("CLS");
                    break;
                }
                clinic.AddCriticallyIllPatient(p);
                std::cout << "\nAdded with Priority: " << p.FirstName << " " << p.LastName << " " << p.SSN << "\n";
                std::cout << "\n";
                std::cin.ignore();
                std::cin.get();
                system("CLS");
                break;
                break;
            case 3:                         // CASE 3: CALLS DEQUEUE FOR CHOSEN CLINIC
                system("CLS");
                if (clinic.isEmpty()) {
                    std::cout << "No more patients.";
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                else {
                    temp = clinic.peek().FirstName + ' ' + clinic.peek().LastName;
                    std::cout << temp << " taken out for operation.";
                    if (clinicName == "the Heart Clinic") {          // Transaction Logging.
                        HeartTransactionLog.saveToLog("Operated", temp + ' ' + clinic.peek().SSN);
                    }
                    else if (clinicName == "the Lung Clinic") {
                        LungTransactionLog.saveToLog("Operated", temp + ' ' + clinic.peek().SSN);
                    }
                    else if (clinicName == "Plastic Surgery") {
                        PlasticTransactionLog.saveToLog("Operated", temp + ' ' + clinic.peek().SSN);
                    }
                    clinic.dequeue();
                }
                std::cin.ignore();
                std::cin.get();
                system("CLS");
                break;
            case 4:                         // CASE 4: CALLS CANCEL PATIENT FOR CHOSEN SSN AND CHECK FOR SSN ERROR
                system("CLS");
                std::cout << "Enter the chosen Patients SSN: ";
                if (!std::all_of(p.SSN.begin(), p.SSN.end(), [](unsigned char c) { return std::isdigit(c); }) && p.SSN.size() != 1) {
                    std::cout << "\nPatients SSN invalid 'Cancel Patient' cancelled";
                    std::cin.ignore();
                    std::cin.get();
                    system("CLS");
                    break;
                }
                std::cin >> targSSN;
                if (clinic.cancelPatient(targSSN) == false) {
                    std::cout << "Patient could not be found, check entered SSN.";
                    std::cin.ignore();
                    std::cin.get();
                }
                else {
                    std::cout << "Patient Canceled";
                    std::cin.ignore();
                    std::cin.get();
                }
                system("CLS");
                break;
            case 5:                         // CASE 5: CALLS LISTALLPATIENTS
                system("CLS");
                clinic.listAllPatients();
                std::cin.ignore();
                std::cin.get();
                system("CLS");
                break;
            case 6:                         // CASE 6: RETURNS SUBMENU BACK UP TO MAIN MENU LOOP 
                system("CLS");
                return;
            default:
                break;
            }
        }
        system("CLS");
    }
}