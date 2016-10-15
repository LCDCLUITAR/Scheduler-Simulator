//+++++++++++++++++++++++++++++++++++++++++++
// Author:   Luis D. Chaparro
// Course:   CSCE 4600
// Due Date: June 14th 2016
// Description: Program that simulates the 
//				creation of a processes.
//+++++++++++++++++++++++++++++++++++++++++++
#include "pcb.h"
#include "scheduler.h"

#pragma region functions

// Constructor to initialize variables
process_Table::process_Table(){
	/* initialize random seed: */
	srand( time(NULL) );
	head = NULL;
	tail = NULL;
	
	total_Memory = total_Cycles = total_Processes = 0;
}
// Gets a unique PID
int process_Table::gen_Uniq_Pid(){
	static int count = 0;
	return ++count;
}
// Gets a random memory size within specified parameters
int process_Table::gen_Mem_Size(){
	int max;
	int min;
	int mean;
	
	max = MAX_MEMORY;
	min = MIN_MEMORY;
	mean = MEAN_MEMORY;
	
	// Get a number under the mean
	if( (rand() % (max - min)) > (mean - min) ) 
		return (rand() % (mean - min)) + min;
	// Get a number over the mean
	else 
		return ( rand() % (max - mean) ) + mean;
}
// Gets a random number of cycles within specified parameters
int process_Table::gen_Num_Cycles(){
	return ( rand() % (MAX_CYCLE - MIN_CYCLE) ) + MIN_CYCLE;
}
// Creates a process table
void process_Table::create_PCB(){
	for(int i = 0; i < PROCESSES; i++){	
		n = new pcb_Table;
		if(head == NULL){
			n->pid = gen_Uniq_Pid();
			n->cycles = gen_Num_Cycles();
			n->memory_Size = gen_Mem_Size();
			
			n->prev = NULL;
			head = n;
			tail = n;
			
			total_Processes++;
			total_Cycles += n->cycles;
			total_Memory += n->memory_Size;
		}
		else{
			n->pid = gen_Uniq_Pid();
			n->cycles = gen_Num_Cycles();
			n->memory_Size = gen_Mem_Size();
			
			n->prev = tail;
			tail->next = n;
			tail = n;
			
			total_Processes++;				// Keeps track of total processes
			total_Cycles += n->cycles;		// Keeps track of total cycles
			total_Memory += n->memory_Size; // Keeps track of total memory
		}
	}
}
// Prints the process table
void process_Table::print_Processes(){
	if(head != NULL){
		n = new pcb_Table;
		n = head;
		
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
		cout << "\tTotal Processes: " << total_Processes << " Total Cycles: " << total_Cycles << " Total Memory: " << total_Memory << endl << endl;
		
		for(int i = 0; i < PROCESSES; i++){
			cout << "\tPID:  " 		 << left << setw(4) << n->pid << 
					" Cycles: " 	 << left << setw(7) << n->cycles << 
					" Memory Size: " << left << setw(3) << n->memory_Size << " KB" << endl;
			n = n->next;
		}
	}
	else{
		cout << "There was an error...\n";
	}
}
// Prints an analysis
void process_Table::printAnalysis(){
	cout << "\n+++++++++++++++++++++++++ Analysis +++++++++++++++++++++++++++++++++++++\n";
	cout << "Number of processes:      " << total_Processes << endl;
	cout << "Average number of cycles: " << total_Cycles/total_Processes << endl;
	cout << "Average memory size: 	   " << total_Memory/total_Processes << endl;
	cout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

#pragma endregion