#include "Transaction.h"
#include "Clinic.h"
#include "Functions.h"
#include <iostream>

int main() {
	Clinic heartClinic("Heart"), lungClinic("Lung"), plasticClinic("Plastic");
	loadScheduledPatients(heartClinic, lungClinic, plasticClinic);
	MainMenu(heartClinic, lungClinic, plasticClinic);

	//Prints Patients to be Rescheduled
	if (!heartClinic.isEmpty() && !lungClinic.isEmpty() && !plasticClinic.isEmpty()) {
		transactionLogs::LogFile << "\nPatients not Seen today:\n";
	}
	if (!heartClinic.isEmpty()) {
		transactionLogs::LogFile << "\n-Heart Clinic Reschedules:\n";
		transactionLogs::LogFile << heartClinic.listRescheduledPatients();
	}
	if (!lungClinic.isEmpty()) {
		transactionLogs::LogFile << "\n-Lung Clinic Reschedules:\n";
		transactionLogs::LogFile << lungClinic.listRescheduledPatients();
	}
	if (!plasticClinic.isEmpty()) {
		transactionLogs::LogFile << "\n-Plastic Clinic Reschedules:\n";
		transactionLogs::LogFile << plasticClinic.listRescheduledPatients();
	}
	return 1;
}