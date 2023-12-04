#include "Transaction.h"

transactionLogs::transactionLogs() {
	Transaction_List = { };
}

std::vector<std::string> transactionLogs::DefualtError{ };

std::ofstream transactionLogs::LogFile("transaction_log.txt");


void transactionLogs::saveToLog(std::string saveType, std::string saveData)
{
	if (saveType == "Error_Startup") {
		DefualtError.push_back(saveData);
	}
	else if (saveType == "Error") {
		DefualtError.push_back(saveData);
		}
	else {
		if (saveType == "Added") {
			Transaction_List.push_back("Added Patient " + saveData);
		}
		if (saveType == "Canceled") {
			Transaction_List.push_back("Canceled Patient " + saveData);
		}
		if (saveType == "Operated") {
			Transaction_List.push_back("Operated on Patient " + saveData);
		}
	}
}

void transactionLogs::endDay(const transactionLogs& log1, const transactionLogs& log2, const transactionLogs& log3) {
	// Print transaction list for log1
	LogFile << "=== Heart Clinic ===\n";
	LogFile << "Transaction Log:\n";
	printVector(log1.Transaction_List);

	// Print transaction list for log2
	LogFile << "=== Lung Clinic ===\n";
	LogFile << "Transaction Log:\n";
	printVector(log2.Transaction_List);

	// Print transaction list for log3
	LogFile << "=== Plastic Surgery ===\n";
	LogFile << "Transaction Log:\n";
	printVector(log3.Transaction_List);

	// Prints Default Error
	LogFile << "\nStatic Default Error List:\n";
	printVector(transactionLogs::DefualtError);
}

void transactionLogs::printVector(const std::vector<std::string>& vec) {
	for (const auto& item : vec) {
		LogFile << "  " << item << "\n";
	}
};
