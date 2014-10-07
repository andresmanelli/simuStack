#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <unistd.h>

#define STACK_RUNNING 0
#define STACK_NOT_RUNNING 1

int main(int argc, char** argv){

	unsigned int index = 1;
	time_t timestamp;
	int stackID = (int) getpid();

	static const char pid[] = "stackPid";
	static const char simus[] = "simuList";
	static const char done[] = "doneSimuList";
	static const char log[] = "logSimuStack";
	
	std::fstream pidfile;
	std::ifstream infile;
	std::ofstream donelist;
	std::ofstream logfile;
	
	int exitCode = system("./checkPid");
	
	if(WEXITSTATUS(exitCode) == STACK_RUNNING){
		
		pidfile.open(pid, std::fstream::in);
	
		if (!pidfile.is_open()){
			std::cout << std::endl << "Error while opening pid file" << std::endl;
			exit(1);
		}
		pidfile >> stackID;
		pidfile.close();
		
		std::cout << std::endl << "Stack already running with pid " << stackID << std::endl;
		exit(1);
	}
	
	pidfile.open(pid, std::fstream::out);
	
	if (!pidfile.is_open()){
		std::cout << std::endl << "Error while opening pid file" << std::endl;
		exit(1);
	}
	pidfile << stackID << std::endl;
	pidfile.close();
	
	
	std::string line;
	
	std::cout << std::endl << "Cleaning simulation list..." << std::endl;
	system("./cleanStackList");
	std::cout << "Stack ready to run." << std::endl;
	
	infile.open(simus);
	
	if (!infile.is_open()){
		std::cout << std::endl << "Error while opening simulations list" << std::endl;
		exit(1);
	}
	
	timestamp = time(0);	
	
	logfile.open(log, std::fstream::app);
	if (!logfile.is_open()){
		std::cout << std::endl << "Error while opening logfile" << std::endl;
		exit(1);
	}
	logfile << std::endl;
	logfile << "#################################################" << std::endl;
	logfile << "Lammps Stack Running..." << std::endl;
	logfile << ctime(&timestamp);
	logfile << "#################################################" << std::endl << std::endl;
	logfile.close();
	
	while(1){
		while (std::getline(infile, line,'#')){
			if(line != ""){
				logfile.open(log, std::fstream::app);
				if (!logfile.is_open()){
					std::cout << std::endl << "Error while opening logfile" << std::endl;
					exit(1);
				}
				
				timestamp = time(0);				
				
				logfile << "#################################################" << std::endl;
				logfile << "COMMAND: " << std::endl << line << std::endl;
				logfile << "Timestamp (ASCII): " << ctime(&timestamp);
				logfile << "Timestamp: " << timestamp << std::endl;
				logfile << "#################################################" << std::endl;
				int err = system(line.c_str());
				logfile << "(Up) Command completed with error code: " << err << std::endl;
				logfile << "#################################################" << std::endl << std::endl;
				
				logfile.close();
				donelist.open(done, std::fstream::app);
				if (!donelist.is_open()){
					std::cout << std::endl << "Error while opening done simulations list" << std::endl;
					exit(1);
				}
				
				donelist << index++ << " " << timestamp << std::endl;
				donelist.close();
			}
		}
		
		sleep(5);
		
		infile.clear();
		//std::cout << "Reading simulations list.." << std::endl;
	}
	
	exit(0);
}
