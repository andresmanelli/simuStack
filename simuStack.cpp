#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <unistd.h>

#define STACK_RUNNING 0
#define STACK_NOT_RUNNING 1

using namespace std;

void openStream(fstream& str, const char* file, ios_base::openmode m){
	
	str.open(file, m);

	if (!str.is_open()){
		cout << endl << "Error while opening " << file << " file" << endl;
		exit(1);
	}
	
	return;
}

int main(int argc, char** argv){

	unsigned int index = 1;
	time_t timestamp;
	int stackID = (int) getpid();
	string line;

	static const char pid[] = "stackPid";
	static const char simus[] = "simuList";
	static const char done[] = "doneSimuList";
	static const char started[] = "startedSimuList";
	static const char log[] = "logSimuStack";
	
	fstream startedList;
	fstream pidfile;
	fstream infile;
	fstream donelist;
	fstream logfile;
	
	int exitCode = system("./checkPid");
	
	if(WEXITSTATUS(exitCode) == STACK_RUNNING){
		
		openStream(pidfile, pid, fstream::in);
		pidfile >> stackID;
		pidfile.close();
		
		cout << endl << "Stack already running with pid " << stackID << endl;
		exit(1);
	}
	
	openStream(pidfile, pid, fstream::in);
	pidfile << stackID << endl;
	pidfile.close();
	//TODO: VERIFICAR SI HABIA ALGO INICIADO Y REEMPLAZAR EL INPUT POR EL INPUT RESTART
	//TODO: REEMPLAZAR EN EL INPUT RESTART EL VALOR DE RESTART MAYOR ENCONTRADO EN LA CARPETA
	//TODO: NORMALIZAR LOS COMANDOS, i.e: PRIMERO UN CD (PARA UBICARSE)
	cout << endl << "Cleaning simulation list..." << endl;
	system("./cleanStackList");
	cout << "Stack ready to run." << endl;
	
	openStream(infile, simus, fstream::in);
		
	timestamp = time(0);	
	
	openStream(logfile, log, fstream::app);
	logfile << endl;
	logfile << "#################################################" << endl;
	logfile << "Lammps Stack Running..." << endl;
	logfile << ctime(&timestamp);
	logfile << "#################################################" << endl << endl;
	logfile.close();
	
	while(1){
		while (getline(infile, line,'#')){
			if(line != ""){
				
				timestamp = time(0);
				
				openStream(logfile, log, fstream::app);			
				logfile << "#################################################" << endl;
				logfile << "COMMAND: " << endl << line << endl;
				logfile << "Timestamp (ASCII): " << ctime(&timestamp);
				logfile << "Timestamp: " << timestamp << endl;
				logfile << "#################################################" << endl;
				int err = system(line.c_str());
				//TODO: INDICAR QUE INICIÓ EN STARTEDLIST
				logfile << "(Up) Command completed with error code: " << err << endl;
				logfile << "#################################################" << endl << endl;
				logfile.close();
				
				openStream(donelist, done, fstream::app);
				donelist << index++ << " " << timestamp << endl;
				donelist.close();
			}
		}
		
		sleep(5);
		
		/*	Borra el eofbit para poder leer las simulaciones 
		 * 	que se agreguen mientras se ejecutaba la anterior
		 */
		infile.clear();
	}
	
	exit(0);
}
