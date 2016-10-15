//+++++++++++++++++++++++++++++++++++++++++++
// Author:   Luis D. Chaparro
// Course:   CSCE 4600
// Due Date: June 14th 2016
// Description: Program that simulates the 
//				creation of a processes.
//+++++++++++++++++++++++++++++++++++++++++++
#include "pcb.h"
#include "scheduler.h"

#ifdef __linux
	Color::Modifier red(Color::FG_RED);
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier blue(Color::FG_BLUE);
	Color::Modifier def(Color::FG_DEFAULT);
#elif _WIN32
	#define red ""
	#define green  ""
	#define blue  ""
	#define def  ""
#endif

#pragma region functions

bool analysisBool = false;

void scheduler::start_Queuing(){
	// Create PCB's to fill table
	create_PCB();
	
	if(head != NULL){
		n = new pcb_Table;
		n = head;
		
		int arrival = 0;
		
		for(int i = 0; i < PROCESSES; i++){
			n->arrival_Time = arrival;
			n->wait_Time = 0;
			p_Queue.push(n); 				// Queues processes to get ready for schedling (pid, memory, cycles)
			n = n->next;
			arrival += 50;
		}
		//cout << p_Queue.back()->pid << endl;
	}
	else{
		cout << "There was an error...\n";
	}
}

void scheduler::round_Robin(){
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
	
	int cycle = 0;		// Keeps track of current cycle
	int total_Cycles = 0;
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while( !p_Queue.empty() ){	
		if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
		//cout <<red<<"Cycle: "<<green<< cycle << def <<endl;
		// If the remaining cycles are more than the quantum save process
		if( (p_Queue.front()->cycles) > QUANTUM ){
			int serv_Time = cycle;
			int arri_Time = p_Queue.front()->arrival_Time;
			
			p_Queue.front()->wait_Time += serv_Time - arri_Time;
			
			p_Queue.front()->cycles -= QUANTUM;					// Subtracts the quantum cycles from the process
			p_Queue.front()->arrival_Time = cycle + QUANTUM;
			p_Queue.push( p_Queue.front() );					// Saves the process if it has not finished and adds it back to be scheduled
			
			//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
			cycle += QUANTUM;
			
			//cout << red << "Process wait time: " << serv_Time << " - " << arri_Time << " = " << p_Queue.front()->wait_Time << " cycles" << def << endl;
		}
		// If the remaining cycles are equal to quantum dont save process
		else if( (p_Queue.front()->cycles) == QUANTUM ){			
			int serv_Time = cycle;
			int arri_Time = p_Queue.front()->arrival_Time;
			
			p_Queue.front()->wait_Time += serv_Time - arri_Time;
			
			p_Queue.front()->cycles -= QUANTUM;					// Subtracts the quantum cycles from the process	
			//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
			cycle += QUANTUM;
			//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << p_Queue.front()->wait_Time << " cycles" << def << endl;
			avg_wait += p_Queue.front()->wait_Time;
		}
		// If remaining cycles are less than quantum just finish cycle
		else{		
			int serv_Time = cycle;
			int arri_Time = p_Queue.front()->arrival_Time;
			//usleep(p_Queue.front()->cycles * CYCLE_MILI);					// Simulates time the cycle takes
			//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << p_Queue.front()->wait_Time << " cycles" << def << endl;
			avg_wait += p_Queue.front()->wait_Time;
		}		
		p_Queue.pop();											// Pops the process to start working with the next one
		//usleep(PENALTY * CYCLE_MILI);							// Context Switch Penalty
		total_Cycles += cycle;
		cycle += PENALTY;
	}
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << PENALTY*(total_Cycles/50) << " cycles" << endl;
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << PENALTY*(total_Cycles/50) << " cycles" << def << endl;
}

void scheduler::round_Robin_4_Processor(){
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
	
	int cycle, 						// Keeps track of current cycle
		i, 
		penalty, 
		total_Cycles;		
	cycle = i = penalty = total_Cycles = 0;
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while( !p_Queue.empty() ){	
		int arrival_Counter = 0;
		for(i = 0; i < 12; i++){
			p_Queue.front()->arrival_Time = arrival_Counter;
			if(p_Queue.empty())
				break;
			if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
			//cout <<red<<"Cycle: "<<green<< cycle << def <<endl;
			// If the remaining cycles are more than the quantum save process
			if( (p_Queue.front()->cycles) > QUANTUM ){
				int serv_Time = cycle;
				int arri_Time = p_Queue.front()->arrival_Time;
				
				p_Queue.front()->wait_Time += serv_Time - arri_Time;
				
				p_Queue.front()->cycles -= QUANTUM;					// Subtracts the quantum cycles from the process
				p_Queue.front()->arrival_Time = cycle + QUANTUM;
				p_Queue.push( p_Queue.front() );					// Saves the process if it has not finished and adds it back to be scheduled
				
				//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
				cycle += QUANTUM;
				
				//cout << red << "Process wait time: " << serv_Time << " - " << arri_Time << " = " << p_Queue.front()->wait_Time << " cycles" << def << endl;
			}
			// If the remaining cycles are equal to quantum dont save process
			else if( (p_Queue.front()->cycles) == QUANTUM ){			
				int serv_Time = cycle;
				int arri_Time = p_Queue.front()->arrival_Time;
				
				p_Queue.front()->wait_Time += serv_Time - arri_Time;
				
				p_Queue.front()->cycles -= QUANTUM;					// Subtracts the quantum cycles from the process	
				//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
				cycle += QUANTUM;
				//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << p_Queue.front()->wait_Time << " cycles" << def << endl;
				avg_wait += p_Queue.front()->wait_Time;
			}
			// If remaining cycles are less than quantum just finish cycle
			else{		
				int serv_Time = cycle;
				int arri_Time = p_Queue.front()->arrival_Time;
				//usleep(p_Queue.front()->cycles * CYCLE_MILI);					// Simulates time the cycle takes
				//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << p_Queue.front()->wait_Time << " cycles" << def << endl;
				avg_wait += p_Queue.front()->wait_Time;
			}		
			p_Queue.pop();											// Pops the process to start working with the next one
			//usleep(PENALTY * CYCLE_MILI);							// Context Switch Penalty
			total_Cycles += cycle;
			cycle += PENALTY;
			arrival_Counter += 50;
		}
		//cout << red << i << def << endl;
		penalty += (total_Cycles/i);
		//cout << red << "Total_Cycles: " << total_Cycles << " Penalty: " << penalty << def << endl;
		total_Cycles = cycle = 0;
	}
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << PENALTY*penalty << " cycles" << endl;	
	
	analytics.close();
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << PENALTY*penalty << " cycles" << def << endl;	
}

void scheduler::fifo(){	
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
		
	int cycle = 0;		// Keeps track of current cycle
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while(!p_Queue.empty()){
		if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
		//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
		int arri_Time = p_Queue.front()->arrival_Time;
		int serv_Time = cycle;
		
		avg_wait = serv_Time - arri_Time;
		
		cycle += p_Queue.front()->cycles;
		cycle += PENALTY;
		//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << avg_wait << " cycles" << def << endl;
		p_Queue.pop();
	}
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << endl;	
	
	analytics.close();
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << def << endl;
}

void scheduler::fifo_4_Processor(){
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
	
	int cycle = 0;		// Keeps track of current cycle
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while(!p_Queue.empty()){
		int arrival_Counter = 0;
		for(int i = 0; i < 12; i++){
			if(p_Queue.empty())
				break;
			if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
			p_Queue.front()->arrival_Time = arrival_Counter;
			//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
			int arri_Time = p_Queue.front()->arrival_Time;
			int serv_Time = cycle;
			
			avg_wait = serv_Time - arri_Time;
			
			cycle += p_Queue.front()->cycles;
			cycle += PENALTY;
			//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << avg_wait << " cycles" << def << endl;
			p_Queue.pop();
			arrival_Counter += 50;
		}
		cycle = 0;
	}
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << endl;	
	
	analytics.close();
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << def << endl;	
}

void scheduler::sjf(){
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
	
	// Temporal vector to sort  queue
	vector<pcb_Table*> sjf_Queue;
	while(!p_Queue.empty()){
		sjf_Queue.push_back(p_Queue.front());
		p_Queue.pop();
	}
	bubbleSort(sjf_Queue);
	
	for(int i = 0; i < 50; i++){
		p_Queue.push(sjf_Queue[i]);
	}
		
	int cycle = 0;		// Keeps track of current cycle
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while(!p_Queue.empty()){
		if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
		//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
		int arri_Time = p_Queue.front()->arrival_Time;
		int serv_Time = cycle;
	
		avg_wait = serv_Time - arri_Time;
	
		cycle += p_Queue.front()->cycles;
		cycle += PENALTY;
		//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << avg_wait << " cycles" << def << endl;
		p_Queue.pop();
	}
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << endl;	
	
	analytics.close();
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << def << endl;
}

void scheduler::sjf_4_Processor(){	
	fstream analytics;
	analytics.open("analytics.txt", fstream::in | fstream::app);
	
	// Temporal vector to sort  queue
	vector<pcb_Table*> sjf_Queue;
	while(!p_Queue.empty()){
		sjf_Queue.push_back(p_Queue.front());
		p_Queue.pop();
	}
	bubbleSort(sjf_Queue);
	
	for(int i = 0; i < 50; i++){
		p_Queue.push(sjf_Queue[i]);
	}
		
	int cycle = 0;		// Keeps track of current cycle
	float avg_wait = 0;
	cout << red << "Scheduling Processes..." << def << endl;
	while(!p_Queue.empty()){
		int arrival_Counter = 0;
		for(int i = 0; i < 12; i++){
			if(p_Queue.empty())
				break;
			p_Queue.front()->arrival_Time = arrival_Counter;
			
			if(analysisBool)
				cout << "Process: " << left << setw(4) << p_Queue.front()->pid << " has " << left << setw(6) << p_Queue.front()->cycles << " cycles left." << endl;
			//usleep(p_Queue.front()->cycles * CYCLE_MILI);   	// Simulates time the cycle takes
			int arri_Time = p_Queue.front()->arrival_Time;
			int serv_Time = cycle;
		
			avg_wait = serv_Time - arri_Time;
		
			cycle += p_Queue.front()->cycles;
			cycle += PENALTY;
			//cout << green << "Process " << left << setw(2) << p_Queue.front()->pid << " wait time: " << avg_wait << " cycles" << def << endl;
			p_Queue.pop();
			arrival_Counter += 50;
		}
		cycle = 0;
	}
	
	analytics << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << endl;
	analytics << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << endl;	
	
	analytics.close();
	
	cout << blue << "Average waiting time is: " << (avg_wait / PROCESSES) << " cycles" << def << endl;
	cout << blue << "Total Context Switch Penalty: " << (PENALTY * PROCESSES) << " cycles" << def << endl;
}

void scheduler::bubbleSort(vector<pcb_Table*>&sjf_Queue){
	// Bubble sort
	pcb_Table* swap;
	for (int c = 0 ; c < ( sjf_Queue.size() - 1 ); c++){
		for (int d = 0 ; d < sjf_Queue.size() - c - 1; d++){
			if (sjf_Queue[d]->cycles > sjf_Queue[d+1]->cycles){
				swap      	   = sjf_Queue[d];
				sjf_Queue[d]   = sjf_Queue[d+1];
				sjf_Queue[d+1] = swap;
			}
		}
	}
}
void beginScheduling(){
	CLEAR;
	char analysis = 'z';	
	cout << green << "Would you like to see the processes id and cycles in the analysis? (Y/N)\n>> " << def;
	cin >> analysis;
		
	if(analysis == 'Y' || analysis == 'y')
		analysisBool = true;
	while(true){
		scheduler schedule;
		int selection = -1;
			
		fstream analytics;
		analytics.open("analytics.txt", fstream::in | fstream::app);
		
		cout << red;
		cout << "1. " << green << "Round Robin\n" << red;
		cout << "2. " << green << "FIFO\n" << red;
		cout << "3. " << green << "SJF\n\n" << red;
		cout << "4. " << green << "Round Robin with 4 Processors\n" << red;
		cout << "5. " << green << "FIFO with 4 Processors\n" << red;
		cout << "6. " << green << "SJF with 4 Processors\n\n" << red;
		cout << "Select a scheduling method to use or 0 to exit...\n>> " << def;
		cin >> selection;
		
		analytics << "\n\n\n";
		switch(selection){
			case 0 :
				exit(EXIT_SUCCESS);
			case 1 : 
				schedule.start_Queuing();
				cout << "Round Robin Scheduler" << endl;
				analytics << "Round Robin Scheduler" << endl;
				schedule.round_Robin();			
				break;
			case 2 : 
				schedule.start_Queuing();
				cout << "FIFO Scheduler" << endl;
				analytics << "FIFO Scheduler" << endl;
				schedule.fifo();			
				break;
			case 3 : 
				schedule.start_Queuing();
				cout << "SJF Scheduler" << endl;
				analytics << "SJF Scheduler" << endl;
				schedule.sjf();
				break;
			case 4 : 
				schedule.start_Queuing();
				cout << "Round Robin Scheduler with 4 processors" << endl;
				analytics << "Round Robin Scheduler with 4 processors" << endl;
				schedule.round_Robin_4_Processor();			
				break;
			case 5 : 
				schedule.start_Queuing();
				cout << "FIFO Scheduler with 4 processors" << endl;
				analytics << "FIFO Scheduler with 4 processors" << endl;
				schedule.fifo_4_Processor();			
				break;
			case 6 : 
				schedule.start_Queuing();
				cout << "SJF Scheduler with 4 processors" << endl;
				analytics << "SJF Scheduler with 4 processors" << endl;
				schedule.sjf_4_Processor();
				break;
			default :
				exit(EXIT_FAILURE);
		}		
		analytics.close();
	}
}
#pragma endregion