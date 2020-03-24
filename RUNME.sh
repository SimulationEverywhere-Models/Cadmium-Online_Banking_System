echo "Script to execute all the atomic model test cases"
echo "Running the build of all the atomic models and top model"
make clean;make all
echo "Running all the atomic models:"
cd bin/
echo "Running the atomic model Account Access Manager"
./AAM_TEST
echo "Running the atomic model Account Access Manager was successful"
echo "Running the atomic model Account Number Verifier"
./ANV_TEST
echo "Running the atomic model Account Number Verifier was successful"
echo "Running the atomic model Bill Payment Manager"
./BPM_TEST
echo "Running the atomic model Bill Payment Manager was successful"
echo "Running the atomic model Password Verifier"
./PV_TEST
echo "Running the atomic model Password Verifier was successful"
echo "Running the atomic model Transaction Process Manager"
./TPM_TEST
echo "Running the atomic model Transaction Process Manager was successful"
echo "Running the coupled model Authentication Engine"
./AE_TEST
echo "Running the coupled model Authentication Engine was successful"
echo "Running the top coupled model Online Banking System"
./IOBS
echo "Running the top coupled model Online Banking System was successful"
echo "All the tests ran successfully, please check the simulation result folder."
echo "The input of the tests can be changed by changing the values from the specific test files in the input_data folder."
echo "All the information on the test cases is given in the report file."
