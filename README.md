# DEVS-Package-Monitoring
This folder contains the ALTERNATE BIT PROTOCOL DEVS model implemented in Cadmium

/******************************/
/****FILE ORGANIZATION*****/
/******************************/

README.txt	
PackageMonitoring.doc
makefile

> atomics [This folder contains header files representing atomic models implemented in Cadmium]
	Excessive_Heat_Detection.hpp
	Humidity_Detection.hpp
	Light_Detection.hpp
	Tampering_Detection.hpp
	Tilting_Detection.hpp

> bin [This folder will be created automatically the first time you compile the poject.
          It will contain all the executables]
> build [This folder will be created automatically the first time you compile the poject.
             It will contain all the build files (.o) generated during compilation]

input_data [This folder contains all the sample input data to run the model and the tests]
	HEAT_input_test.txt
	HUMIDITY_input_test.txt
	SLEEP_input_test.txt
	TILT_input_test.txt

simulation_results [This folder will be created automatically the first time you compile the poject.
                    It will store the outputs from your simulations and tests]

test [This folder the unit test of the atomic models]
	main_heat_test.cpp
	main_hum_test.cpp
	main_LD_test.cpp
	main_TamperingDetection_test.cpp
	main_tilt_test.cpp

top_model [This folder contains the Monitoring Package model itself]	
	main.cpp
	
/**************/
/****STEPS****/
/**************/

0 - PackageMonitoring.docx contains the explanation of this model

1 - Update include path in the makefile in this folder and subfolders. You need to update the following lines:
	INCLUDECADMIUM=-I ../../cadmium/include
	INCLUDEDESTIMES=-I ../../DESTimes/include
    Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
	Example: INCLUDECADMIUM=-I ../../cadmium/include
	Do the same for the DESTimes library
2 - Compile the project and the tests
	2.1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the Package Monitoring folder
	2.2 - To compile the project and the tests, type in the terminal:
			make clean; make all
3 - Run individual atomic model test
	3.1 - Open the terminal in the bin folder. 
	3.2 - To run the test, type in the terminal "./NAME_OF_THE_COMPILED_FILE" (For windows, "./NAME_OF_THE_COMPILED_FILE.exe"). 
	To test heat detection, for instance, you need to type
			./ExcessiveHeatDetection_TEST (or ./ExcessiveHeatDetection_TEST.exe for Windows)
	3.3 - To check the output of the test, go to the folder simulation_results and open  "ExcessiveHeatDetection_test_output_messages.txt" and "ExcessiveHeatDetection_test_output_state"
NOTE: to run any of the other test models, simply repeat steps 3.2 and 3.3 but choosing the corresponding executable name and simulation results name.
5 - Run the top model
	5.1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
	5.2 - To run the model, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this test you need to type:
		./PM ../input_data/PM_input_test.txt (for Windows: ./PM.exe ../input_data/PM_input_test.txt)
	5.3 - To check the output of the model, go to the folder simulation_results and open "PM_output_messasges.txt" and "PM_output_state.txt"
	5.4 - To run the model with different inputs
		5.4.1. Create new .txt files with the same structure as input_PM_1.txt or input_PM_2.txt in the folder input_data
		5.4.2. Run the model using the instructions in step 3
		5.4.3. If you want to keep the output, rename "PM_output_messages.txt" and "PM_output_state.txt", or move them to a different directory for later use. Otherwise it will be 			overwritten when you run the next simulation.

