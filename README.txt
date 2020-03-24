This folder contains the INTERNET ONLINE BANKING SYSTEM DEVS model implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt	
InternetOnlineBankingSystem.doc : I also have provided a .pdf format just in case the doc file shows some formatting errors
makefile
RUNME.sh

atomics [This folder contains atomic models implemented in Cadmium]
	AccountAccessManager.hpp
	AccountNumberVerifier.hpp
	BillPaymentManager.hpp
	PasswordVerifier.hpp
	TransactionProcessManager.hpp
bin [This folder will be created automatically the first time you compile the project.
     It will contain all the executables]
build [This folder will be created automatically the first time you compile the project.
       It will contain all the build files (.o) generated during compilation]
data_structures [This folder contains message data structure used in the model]
	message.hpp
	message.cpp
input_data [This folder contains all the input data to run the model and the tests. All the test cases are being tested through the input files. Input files contain all the possible test cases.]
	IOBS_input_test.txt (inputs for top model)
	AE_input_test.txt (inputs for coupled model Authentication Engine)
	AAM_input_test.txt
	ANV_input_test.txt
	BPM_input_test.txt
	PV_input_test.txt
	TPM_input_test.txt
simulation_results [This folder will be created automatically the first time you compile the project.
                    It will store the outputs from your simulations and tests]
test [This folder the unit test of the atomic models]
	main_AAM_test.cpp
	main_ANV_test.cpp
	main_BPM_test.cpp
	main_PV_test.cpp
	main_TPM_test.cpp
	main_AE_test.cpp (This is a coupled model Authentication Engine which contains a coupling of ANV, BPM and PV)
top_model [This folder contains the IOBS top model]	
	main.cpp
script to build and run the atomic models, coupled model and the top model automatically
	RUNME.sh
/**********************************/
/****STEPS FOR MANUAL EXECUTION****/
/**********************************/

0 - InternetOnlineBankingSystem.doc contains the explanation of this model

1 - Update include path in the makefile in this folder and subfolders. You need to update the following lines:
	INCLUDECADMIUM=-I ../../cadmium/include
	INCLUDEDESTIMES=-I ../../DESTimes/include
    Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
	Example: INCLUDECADMIUM=-I ../../cadmium/include
	Do the same for the DESTimes library
    NOTE: if you follow the step by step installation guide you will not need to update these paths.

2 - Compile the project and the tests
	1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the Online Banking System folder
	2 - To compile the project and the tests, type in the terminal:
			make clean; make all

3 - To run individual atomic model test:
	For example, to run Account Access Manager test
	1 - Open the terminal in the bin folder. 
	2 - To run the test, type in the terminal "./NAME_OF_THE_COMPILED_FILE" (For windows, "./NAME_OF_THE_COMPILED_FILE.exe"). 
	For this specific test you need to type:
			./AAM_TEST (or ./AAM_TEST.exe for Windows)
	3 - To check the output of the test, go to the folder simulation_results and open  "AAM_test_output_messages.txt" and "AAM_test_output_state.txt"

4 - To run atomic model tests you just need to select the proper executable name in step 3.2 and check the output in the folder simulation_results.
	For example, ./PV_TEST (./PV_TEST.exe in case of Windows) or ./ANV_TEST (./ANV_TEST.exe in case of Windows) etc. will give an output in the simulation_results folder as "NAMEOFTHEATOMICMODEL_output_messages.txt" and "NAMEOFTHEATOMICMODEL_output_state.txt" where NAMEOFTHEATOMICMODEL can be AAM, ANV, PV, BPM or TPM.
			
5 - Run the top model
	1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
	2 - To run the model, type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this test you need to type:
		./IOBS (for Windows: ./IOBS.exe)
	3 - To check the output of the model, go to the folder simulation_results and open "IOBS_output_messages.txt" and "IOBS_output_state.txt"
	4 - To run the model with different inputs
		4.1. Edit the .txt file with the name input_IOBS_test.txt in the folder input_data and add your inputs
		4.2. Run the model using the instructions in step 2
		4.3. If you want to keep the output, rename a "IOBS_output_messages.txt" and "IOBS_output_state.txt". Otherwise it will be overwritten when you run the next simulation.


/**********************************/
/****STEPS FOR SCRIPT EXECUTION****/
/**********************************/

1 - you need to follow the step by step installation guide for cadmium to avoid the location problems.

2 - To make the script file RUNME.sh executable:
	1 - Open the terminal in the Online Banking System folder where the RUNME.sh file is located.
	2 - In the terminal type this command "chmod +x RUNME.sh" without quotes.

3 - Run the RUNME.sh executable by this command in the same terminal
	Type "./RUNME.sh" without quotes to start the script.

About the Script: 
	This script compiles all the atomic models, coupled model and top model of the Online Banking System creating the executables and the object files. Later the script automatically runs the atomic models, coupled model and the top model and the output of the simulation can be seen in the newly created folder "simulation_results".
This script basically automates the building and execution of the model and makes it easier to compile and execute everything at the same time. This script file is tested on linux Ubuntu and it works perfectly compiling and executing the models. Some further instructions can be found out during the execution of the script.
