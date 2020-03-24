CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)

	
message.o: data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/message.cpp -o build/message.o

main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o
main_AE_test.o: test/main_AE_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_AE_test.cpp -o build/main_AE_test.o
main_AAM_test.o: test/main_AAM_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_AAM_test.cpp -o build/main_AAM_test.o
main_ANV_test.o: test/main_ANV_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_ANV_test.cpp -o build/main_ANV_test.o
main_PV_test.o: test/main_PV_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_PV_test.cpp -o build/main_PV_test.o
main_BPM_test.o: test/main_BPM_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_BPM_test.cpp -o build/main_BPM_test.o
main_TPM_test.o: test/main_TPM_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_TPM_test.cpp -o build/main_TPM_test.o


tests: main_AE_test.o main_AAM_test.o main_ANV_test.o main_PV_test.o main_BPM_test.o main_TPM_test.o message.o
		$(CC) -g -o bin/AE_TEST build/main_AE_test.o build/message.o
		$(CC) -g -o bin/AAM_TEST build/main_AAM_test.o build/message.o
		$(CC) -g -o bin/ANV_TEST build/main_ANV_test.o build/message.o
		$(CC) -g -o bin/PV_TEST build/main_PV_test.o build/message.o
		$(CC) -g -o bin/BPM_TEST build/main_BPM_test.o build/message.o
		$(CC) -g -o bin/TPM_TEST build/main_TPM_test.o build/message.o


#TARGET TO COMPILE ONLY IOBS SIMULATOR
simulator: main_top.o message.o 
	$(CC) -g -o bin/IOBS build/main_top.o build/message.o 
	
#TARGET TO COMPILE EVERYTHING (ABP SIMULATOR + TESTS TOGETHER)
all: simulator tests

#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*
