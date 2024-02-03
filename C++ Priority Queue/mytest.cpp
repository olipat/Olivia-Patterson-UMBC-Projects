// CMSC 341 - Fall 2023 - Project 3
/***********************************************************
 ** File:    mytest.cpp
 ** Project: CMSC 341 - Project 3
 ** Author:  Olivia Patterson
 ** Date     11/14/2023
 ** This file contains the test cases for the pqueue class
 **********************************************************/

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);


// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
        "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
        "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
        "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
        "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
        "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

//Test class functions
class Tester{
public:
    bool testInsertMin(int numNodes);
    bool testInsertMax(int numNodes);
    bool testRemoveMin(int numNodes);
    bool testRemoveMax(int numNodes);
    bool testLeftistProperty();
    bool testLeftistNPL();
    bool testPriorityFn();
    bool testMergeEmptyToNormal();
    bool testCopyNormal();
    bool testCopyEmpty();
    bool testAssignmentNormal();
    bool testAssignmentEdge();
    bool testDequeueEmpty();
    bool testMergePriorityFn();
    bool testInsertAmortizedLog();
    bool testRemoveAmortizedLog();
    double sampleTimeMeasurementInsert(PQueue& aQueue, Patient& aPatient);
    double sampleTimeMeasurementRemove(PQueue& aQueue, Patient& aPatient);
};

int main(){
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i=0;i<10;i++){
        Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
        aQueue.insertPatient(patient);
    }
    cout << "\nDump of the leftist heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    aQueue.setStructure(SKEW);
    cout << "\nDump of the skew heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn2 (MINHEAP):\n";
    aQueue.printPatientQueue();
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the skew queue with priorityFn1 (MAXHEAP):\n";
    aQueue.dump();

    //Test Cases Below
    Tester tester;
    cout << endl;
    cout << "The following are test cases for the pqueue class." << endl;
    cout << endl;

    cout << "Following is the test message for testing Insert on Minheap: " << endl;
    //Tests that insert is working normally on min
    if (tester.testInsertMin(300) == true){
        cout << "Test msg: Insert test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Insert test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing Insert on Maxheap: " << endl;
    //Tests that insert is working normally on max
    if (tester.testInsertMax(300) == true){
        cout << "Test msg: Insert test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Insert test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing Remove on Minheap: " << endl;
    //Tests that remove is working normally on min
    if (tester.testRemoveMin(300) == true){
        cout << "Test msg: Remove test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Remove test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing Remove on Maxheap: " << endl;
    //Tests that remove is working normally on max
    if (tester.testRemoveMax(300) == true){
        cout << "Test msg: Remove test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Remove test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing Leftist Property: " << endl;
    //Tests that the leftist property is working normally
    if (tester.testLeftistProperty() == true){
        cout << "Test msg: Leftist Property test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Leftist Property test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing Leftist NPL: " << endl;
    //Tests that the leftist NPL is working normally
    if (tester.testLeftistNPL() == true){
        cout << "Test msg: Leftist NPL test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Leftist NPL test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing a change in priority function: " << endl;
    //Tests that the change in priority function is working normally
    if (tester.testPriorityFn() == true){
        cout << "Test msg: Change in priority function test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Change in priority function test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing merging an empty queue with a normal queue: " << endl;
    //Tests that merging an empty queue with a normal queue is working normally
    if (tester.testMergeEmptyToNormal() == true){
        cout << "Test msg: Merge empty with normal test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Merge empty with normal test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the copy constructor on a normal queue: " << endl;
    //Tests that copying a normal queue to a new queue is working normally
    if (tester.testCopyNormal() == true){
        cout << "Test msg: Copy constructor test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Copy constructor test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the copy constructor on an edge case: " << endl;
    //Tests that copying an empty queue to a new queue is working normally
    if (tester.testCopyEmpty() == true){
        cout << "Test msg: Copy constructor test for edge case passed!" << endl;
    }
    else {
        cout << "Test msg: Copy constructor test for edge case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the assignment operator on a normal queue: " << endl;
    //Tests that assigning a normal queue to a new queue is working normally
    if (tester.testAssignmentNormal() == true){
        cout << "Test msg: Assignment operator test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Assignment operator test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the assignment operator on an edge case: " << endl;
    //Tests that assigning an empty queue to a new queue is working normally
    if (tester.testAssignmentEdge() == true){
        cout << "Test msg: Assignment operator test for edge case passed!" << endl;
    }
    else {
        cout << "Test msg: Assignment operator test for edge case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the dequeue function on an empty queue: " << endl;
    //Tests that dequeueing an empty queue throws an out_of_range exception
    if (tester.testDequeueEmpty() == true){
        cout << "Test msg: Dequeue test for empty case passed!" << endl;
    }
    else {
        cout << "Test msg: Dequeue test for empty case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the merge function on queues with different priority functions: " << endl;
    //Tests that merging queues with different priority functions throws a domain_error exception
    if (tester.testMergePriorityFn() == true){
        cout << "Test msg: Merge test for different priority functions passed!" << endl;
    }
    else {
        cout << "Test msg: Merge test for different priority functions failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the amortized time for insert: " << endl;
    //Tests that the amortized time for insert is O(log n)
    if (tester.testInsertAmortizedLog() == true){
        cout << "Test msg: Insert amortized time test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Insert amortized time test for normal case failed!" << endl;
    }
    cout << endl;

    cout << "Following is the test message for testing the amortized time for remove: " << endl;
    //Tests that the amortized time for remove is O(log n)
    if (tester.testRemoveAmortizedLog() == true){
        cout << "Test msg: Remove amortized time test for normal case passed!" << endl;
    }
    else {
        cout << "Test msg: Remove amortized time test for normal case failed!" << endl;
    }
    cout << endl;


    return 0;
}

//Provided Priority function implementation
int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}
int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}

bool Tester::testInsertMin(int numNodes){
    //create a queue and insert patients
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < numNodes; ++i) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }
    // Call checkHeapProperty to see if heap property is being preserved for min
    if (aQueue.checkHeapProperty(aQueue.m_heap, aQueue.m_priorFunc, aQueue.m_heapType) == true){ //what do I call for checkHeaproperty? I need the root, the priority, and the type
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testInsertMax(int numNodes){
    //create a queue and insert patients
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    for (int i = 0; i < numNodes; ++i) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }

    // Call checkHeapProperty that heap property is being preserved for max
    if (aQueue.checkHeapProperty(aQueue.m_heap, aQueue.m_priorFunc, aQueue.m_heapType)) {
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testRemoveMin(int numNodes){
    //create a queue to insert patients
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    //use vector to store removed patients
    std::vector<Patient> removedPatients;

    // Insert nodes
    for (int i = 0; i < numNodes; ++i) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }

    // Remove nodes and store them in vector
    for (int i = 0; i < numNodes; ++i) {
        removedPatients.push_back(aQueue.getNextPatient());
    }

    //check that patients are in correct order based on minheap
    for (size_t i = 1; i < removedPatients.size(); ++i) {
        if (priorityFn2(removedPatients[i - 1]) > priorityFn2(removedPatients[i])) {
            return false;
        }
    }
    return true;
}
bool Tester::testRemoveMax(int numNodes){
    //create a queue to insert patients
    PQueue aQueue(priorityFn1, MAXHEAP, LEFTIST);
    //use vector to store removed patients
    std::vector<Patient> removedPatients;

    // Insert nodes
    for (int i = 0; i < numNodes; ++i) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }

    // Remove nodes and store them in removedPatients
    for (int i = 0; i < numNodes; ++i) {
        removedPatients.push_back(aQueue.getNextPatient());
    }

    // Check that patients are in correct order based on maxheap
    for (size_t i = 1; i < removedPatients.size(); ++i) {
        if (priorityFn1(removedPatients[i - 1]) < priorityFn1(removedPatients[i])) {
            return false;
        }
    }
    return true;
}
bool Tester::testLeftistProperty(){
    //create a queue and insert patients
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 20; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }
    // Call checkLeftistProperty to see if leftist property is being preserved
    if (aQueue.checkLeftistProperty(aQueue.m_heap) == true){
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testLeftistNPL(){
    //create a queue and insert patients
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 20; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }
    // Call checkLeftistNPL to see if leftist NPL is being preserved
    if (aQueue.checkLeftistNPL(aQueue.m_heap)){
        return true;
    }
    else {
        return false;
    }
}
bool Tester::testPriorityFn(){
    //create a queue and insert patients
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 20; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }

    // Call checkHeapProperty to see if heap property is being preserved for min
    if (!aQueue.checkHeapProperty(aQueue.m_heap, aQueue.m_priorFunc, aQueue.m_heapType)){
        return false;
    }

    //Change the priority function and heap to max
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    // Call checkHeapProperty to see if heap property is being preserved for max
    if (!aQueue.checkHeapProperty(aQueue.m_heap, aQueue.m_priorFunc, aQueue.m_heapType)){
        return false;
    }
    return true;
}
bool Tester::testMergeEmptyToNormal(){
    // Create a queue and insert patients
    PQueue normalQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 20; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        normalQueue.insertPatient(patient);
    }

    // Create an empty queue
    PQueue emptyQueue(priorityFn2, MINHEAP, LEFTIST);
    // Merge the empty queue with the normal queue
    emptyQueue.mergeWithQueue(normalQueue);

    //check if the empty queues heap property match the normal queues heap property
    return emptyQueue.checkHeapProperty(emptyQueue.m_heap, priorityFn2, MINHEAP);
}
bool Tester::testCopyNormal(){
    // Create a queue and insert patients
    PQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        queue.insertPatient(patient);
    }

    // Create a copy using the copy constructor
    PQueue copyQueue(queue);

    // Check if sizes are the same
    if (queue.numPatients() != copyQueue.numPatients()) {
        return false;
    }

    // Check if elements are the same without modifying the original queue
    // create a temporary queue to avoid modifying the original
    PQueue tempQueue(queue);
    while (tempQueue.numPatients() > 0) {
        Patient originalPatient = tempQueue.getNextPatient();
        Patient copiedPatient = copyQueue.getNextPatient();
        if ((originalPatient.m_patient != copiedPatient.m_patient) &&
            (originalPatient.m_temperature != copiedPatient.m_temperature) &&
            (originalPatient.m_oxygen != copiedPatient.m_oxygen) &&
            (originalPatient.m_RR != copiedPatient.m_RR) &&
            (originalPatient.m_BP != copiedPatient.m_BP) &&
            (originalPatient.m_opinion != copiedPatient.m_opinion)) {
            return false;
        }
    }
    // If all checks pass, return true
    return true;
}
bool Tester::testCopyEmpty(){
    // Create an empty queue
    PQueue emptyQueue(priorityFn2, MINHEAP, LEFTIST);

    // Create a copy using the copy constructor
    PQueue copyQueue(emptyQueue);

    // Check if sizes are the same
    if (emptyQueue.numPatients() != copyQueue.numPatients()) {
        return false;
    }
    return true;
}
bool Tester::testAssignmentNormal(){
    // Create a queue and insert patients
    PQueue queue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 10; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        queue.insertPatient(patient);
    }

    // Create a new queue and assign the original queue to it
    PQueue newQueue(priorityFn2, MINHEAP, LEFTIST);
    newQueue = queue;

    // Check if sizes are the same
    if (queue.numPatients() != newQueue.numPatients()) {
        return false;
    }
    // Check if elements are the same in both queues
    while (queue.numPatients() > 0) {
        Patient patientFromQueue = queue.getNextPatient();
        Patient patientFromNewQueue = newQueue.getNextPatient();
        if (patientFromQueue.getPatient() != patientFromNewQueue.getPatient() ||
            patientFromQueue.getTemperature() != patientFromNewQueue.getTemperature() ||
            patientFromQueue.getOxygen() != patientFromNewQueue.getOxygen() ||
            patientFromQueue.getRR() != patientFromNewQueue.getRR() ||
            patientFromQueue.getBP() != patientFromNewQueue.getBP() ||
            patientFromQueue.getOpinion() != patientFromNewQueue.getOpinion()) {
            return false;
        }
    }
    // If all checks pass, return true
    return true;
}
bool Tester::testAssignmentEdge(){
    // Create an empty queue
    PQueue queue(priorityFn2, MINHEAP, LEFTIST);
    // Create a non-empty queue and insert patients
    PQueue nonEmptyQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 5; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        nonEmptyQueue.insertPatient(patient);
    }

    // Assign the empty queue to the non-empty queue
    nonEmptyQueue = queue;
    // Check if the sizes are the same (should be 0)
    if (nonEmptyQueue.numPatients() != 0) {
        return false;
    }
    // Check if the empty queue is really empty
    if (nonEmptyQueue.numPatients() > 0) {
        return false;
    }
    // If all checks pass, return true
    return true;
}
bool Tester::testDequeueEmpty(){
    try {
        // Create an empty queue
        PQueue emptyQueue(priorityFn2, MINHEAP, SKEW);
        // Attempt to dequeue from an empty queue
        emptyQueue.getNextPatient();
        // if acception not throw, return false
        return false;
    }
    //exception caught, return true
    catch (const std::out_of_range& e) {
        // The expected exception was caught, which is what we want
        return true;
    }
}
bool Tester::testMergePriorityFn(){
    try {
        // Create two queues with different priority functions
        PQueue queue1(priorityFn1, MAXHEAP, SKEW);
        PQueue queue2(priorityFn2, MINHEAP, SKEW);

        // Attempt to merge queues with different priority functions
        queue1.mergeWithQueue(queue2);

        //exceptions not throw, return false
        return false;
    }
    //exception caught, return true
    catch (const std::domain_error& e) {
        return true;
    }
}
bool Tester::testInsertAmortizedLog() {
    // Create a queue and insert 1000 nodes
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 1000; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());

        // Measure the time to insert a single node
        double elapsedTime = sampleTimeMeasurementInsert(aQueue, patient);

        // Calculate the ratio and check if it's within bounds for O(log n)
        double actualRatio = elapsedTime / log2(i + 1);
        double expectedRatio = 1.1;
        double error = 0.4;

        // If the ratio is within the bounds, return true
        if (actualRatio > (expectedRatio - error) || actualRatio < (expectedRatio + error)) {
            return true;
        }
    }
    return false;
}
bool Tester::testRemoveAmortizedLog() {
    // Create a queue and insert 1000 nodes
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i = 0; i < 1000; i++) {
        Patient patient("TestPatient", rand(), rand(), rand(), rand(), rand());
        aQueue.insertPatient(patient);
    }

    // Remove 1000 nodes
    for (int i = 0; i < 1000; i++) {
        Patient patientToRemove = aQueue.getNextPatient();

        // Measure the time to remove a single node
        double elapsedTime = sampleTimeMeasurementRemove(aQueue, patientToRemove);

        // Calculate the ratio and check if it's within bounds for O(log n)
        double actualRatio = elapsedTime / log2(1000 - i);
        double expectedRatio = 1.1;
        double error = 0.4;

        // If the ratio is within the bounds, return true
        if (actualRatio > (expectedRatio - error) || actualRatio < (expectedRatio + error)) {
            return true;
        }
    }
    return false;
}
double Tester::sampleTimeMeasurementInsert(PQueue& aQueue, Patient& aPatient) {
    //initialize variables
    double T = 0.0;
    clock_t start, stop;
    start = clock();

    // the operation to be measured
    aQueue.insertPatient(aPatient);

    stop = clock();
    T = stop - start;  // number of clock ticks the operation took
    double measureTime = T / CLOCKS_PER_SEC;  // time in seconds
    return measureTime;
}
double Tester::sampleTimeMeasurementRemove(PQueue& aQueue, Patient& aPatient) {
    double T = 0.0;  // stores running times
    clock_t start, stop;  // stores the clock ticks while running the program
    start = clock();

    // the operation to be measured
    aQueue.getNextPatient();

    stop = clock();
    T = stop - start;  // number of clock ticks the operation took
    double measureTime = T / CLOCKS_PER_SEC;  // time in seconds
    return measureTime;
}

