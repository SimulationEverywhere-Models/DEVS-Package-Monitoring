CC=g++ 
CFLAGS=-std=c++17 

INCLUDECADMIUM=-I ../../cadmium/include 
INCLUDEDESTIMES=-I ../../DESTimes/include 

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME 
bin_folder := $(shell mkdir -p bin) 
build_folder := $(shell mkdir -p build) 
results_folder := $(shell mkdir -p simulation_results) 

#TARGET TO COMPILE LD TEST 
main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o

main_LightDetection_test.o: test/main_LightDetection_test.cpp 
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_LightDetection_test.cpp -o build/main_LightDetection_test.o 
	
main_TamperingDetection_test.o: test/main_TamperingDetection_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_TamperingDetection_test.cpp -o build/main_TamperingDetection_test.o
	
main_ExcessiveHeatDetection_test.o: test/main_ExcessiveHeatDetection_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_ExcessiveHeatDetection_test.cpp -o build/main_ExcessiveHeatDetection_test.o
	
main_TiltingDetection_test.o: test/main_TiltingDetection_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_TiltingDetection_test.cpp -o build/main_TiltingDetection_test.o
	
main_HumidityDetection_test.o: test/main_HumidityDetection_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_HumidityDetection_test.cpp -o build/main_HumidityDetection_test.o

main_BOD_test.o: test/main_BOD_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_BOD_test.cpp -o build/main_BOD_test.o

tests: main_LightDetection_test.o main_TamperingDetection_test.o main_ExcessiveHeatDetection_test.o main_TiltingDetection_test.o main_HumidityDetection_test.o main_BOD_test.o

	$(CC) -g -o bin/LightDetection_TEST build/main_LightDetection_test.o
	$(CC) -g -o bin/TamperingDetection_TEST build/main_TamperingDetection_test.o
	$(CC) -g -o bin/ExcessiveHeatDetection_TEST build/main_ExcessiveHeatDetection_test.o
	$(CC) -g -o bin/TiltingDetection_TEST build/main_TiltingDetection_test.o
	$(CC) -g -o bin/HumidityDetection_TEST build/main_HumidityDetection_test.o
	$(CC) -g -o bin/BOD_TEST build/main_BOD_test.o
	
#TARGET TO COMPILE ONLY PM SIMULATOR
simulator: main_top.o 
	$(CC) -g -o bin/PM build/main_top.o

#TARGET TO COMPILE EVERYTHING 
all: simulator tests 

#CLEAN COMMANDS 
clean:  
	rm -f bin/* build/* 