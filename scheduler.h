//+++++++++++++++++++++++++++++++++++++++++++
// Author:   Luis D. Chaparro
// Course:   CSCE 4600
// Due Date: June 14th 2016
// Description: Program that simulates the 
//				creation of a processes.
//+++++++++++++++++++++++++++++++++++++++++++
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>		// Time tracking
#include <queue>		// Allows Queues
#include <algorithm>
#include <vector>
#include <fstream>

#pragma region defines

#define CYCLE_STEP 50	// Processes arrive every 50 cycles
#define PENALTY 10		// Context Switch penalty
#define QUANTUM 50		// Round Robin Quantum cycles
#define CYCLE_MILI 1 	// 1 Cycle = 0.001 ms

#define NEWLINE cout << endl;

#ifdef _WIN32 // Windows compatibility
	#include <Windows.h>
	#define CLEAR system("cls"); // Clear the comand prompt (screen)
	#define SLEEP Sleep(1000); // Make program pause for one second before continuing
	#define KEYPRESS cout << "Press any key to continue..."; _getch(); // "Press any key to continue..."
	#define GETCH _getch
	#define null NULL
#elif __linux // Linux compatibility
	#include <unistd.h>
	#define CLEAR system("clear"); // Clear the comand prompt (screen)
	#define SLEEP sleep(1); // Make program pause for one second before continuing
	#define KEYPRESS cout << "Press [Enter] to continue..."; cin.ignore(); // "Press any key to continue..."
	#define GETCH getch
	#define null NULL
	#define INT_MAX 2147483647
#endif

#pragma endregion

#pragma region Text Color
namespace Color
{
	enum Code
	{
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 91,
		BG_GREEN = 42,
		BG_BLUE = 96,
		BG_DEFAULT = 49
	};
	class Modifier
	{
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
			operator<<(std::ostream& os, const Modifier& mod)
		{
			return os << "\033[" << mod.code << "m";
		};

	};
}
#pragma endregion

using namespace std;
	
#ifndef pcb_H_

#pragma region scheduler

class scheduler: public process_Table{
	private:
		queue<pcb_Table*> p_Queue;
		pcb_Table processes;
	public:		
		void start_Queuing();
		void round_Robin();
		void round_Robin_4_Processor();
		void fifo();
		void fifo_4_Processor();
		void sjf();
		void sjf_4_Processor();
		void bubbleSort(vector<pcb_Table*>&);
};

#pragma endregion

void beginScheduling();

#endif