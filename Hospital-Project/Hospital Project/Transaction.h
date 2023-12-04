#pragma once
#include <string>
#include <vector>
#include <fstream>

class transactionLogs {			// All of the transaction log functions are a class to help with organization and 
private:						// so that the code could be changed to have multiple hospitals all with their own logs and clinics easily.
	std::vector<std::string> Transaction_List;
	std::vector<std::string> Error_List;
	static std::vector<std::string>DefualtError;

	static void printVector(const std::vector<std::string>& vec);
public:
	static std::ofstream LogFile;

	void saveToLog(std::string saveType, std::string saveData);
	static void endDay(const transactionLogs& log1, const transactionLogs& log2, const transactionLogs& log3);

	transactionLogs();
};
