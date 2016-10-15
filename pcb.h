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
#include <ctime>

#pragma region PCB_defines

#define MAX_CYCLE 11000
#define MIN_CYCLE 1000
#define MEAN_CYCLE 6000
#define MAX_MEMORY 100
#define MIN_MEMORY 1
#define MEAN_MEMORY 20

#define PROCESSES 50		// Generates 50 Processes

#pragma endregion

using namespace std;
	
#ifndef pcb_H_

#pragma region process_Table

class process_Table{
	protected:
		int total_Processes; 	// Number of processes currently on the table
		int total_Cycles; 	 	// Total number of cycles
		int total_Memory;	 	// Totale memory 
		typedef struct pcb_Table{
			int pid;			// Unique process ID
			int wait_Time;
			int arrival_Time;
			int service_Time;
			int cycles;			// Number of cycles
			int memory_Size;	// Memory size footprint
			
			pcb_Table* next;	// Keeps track of next process
			pcb_Table* prev;	// Keeps track of previous process
		} pcb_Table;
		
		pcb_Table* head;		// First Process in list
		pcb_Table* tail;		// Last Process in list
		pcb_Table* n;			// Temporal pointer to add a new process
	public:
		friend class scheduler;
		process_Table();		// Constructor that initializes values
		void create_PCB();		// Creates pcb's
		int gen_Uniq_Pid();		// Generates a unique process id
		int gen_Num_Cycles();	// Generates a random number of cycles within specified parameters 
		int gen_Mem_Size();		// Generates a random memory size within specified parameters
		void print_Processes();	// Prints all processes on linked list
		void printAnalysis(); 	// Prints an analysis of the program's data
};

#pragma endregion

#endif