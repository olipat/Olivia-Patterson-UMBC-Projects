// CMSC 341 - Fall 2023 - Project 3
/***********************************************************
 ** File:    pqueue.cpp
 ** Project: CMSC 341 - Project 3
 ** Author:  Olivia Patterson
 ** Date     11/14/2023
 ** This file contains the .cpp file for pqueue
 **********************************************************/
#include "pqueue.h"
//pqueue constructor
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
}
//pqueue destructor
PQueue::~PQueue(){
    clear();
}
void PQueue::clear() {
    //helper to recursively clear the heap
    recursiveClear(m_heap);
    //clear the heap and size
    m_heap = nullptr;
    m_size = 0;
}
//copy constructor
PQueue::PQueue(const PQueue& rhs){
    //set heap to null and size to 0
    m_heap = nullptr;
    m_size = 0;
    //assign types of rhs to lhs
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;

    // Recursively copy the left and right subtrees
    if (rhs.m_heap != nullptr) {
        m_heap = copySubtree(rhs.m_heap);
        m_size = subtreeSize(m_heap);
    }
}
//overloaded assignment operator
PQueue& PQueue::operator=(const PQueue& rhs) {
    //condition for self assignment
    if (this == &rhs) {
        return *this;
    }
    //clear the heap & set rhs types to lhs
    clear();
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;

    //if rhs not empty, copy to lhs
    if (rhs.m_heap != nullptr) {
        m_heap = new Node(rhs.m_heap->getPatient());
        m_heap->m_npl = rhs.m_heap->getNPL();
        m_size = 1;

        // Recursively copy the left subtree
        if (rhs.m_heap->m_left != nullptr) {
            m_heap->m_left = copySubtree(rhs.m_heap->m_left);
            m_size += subtreeSize(m_heap->m_left);
        }
        // Recursively copy the right subtree
        if (rhs.m_heap->m_right != nullptr) {
            m_heap->m_right = copySubtree(rhs.m_heap->m_right);
            m_size += subtreeSize(m_heap->m_right);
        }
    }
    //return lhs
    return *this;
}
void PQueue::mergeWithQueue(PQueue& rhs) {
    //Condition to avoid merging with itself
    if (this == &rhs) {
        throw std::domain_error("Merging with itself is not allowed.");
    }
    //Condition to avoid merging queues with different priority functions or data structures
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
        throw std::domain_error("Cannot merge queues with different priority functions or data structures.");
    }

    // Decide the type of merge based on heapType and structure
    if (m_structure == SKEW) {
        m_heap = mergeSkew(m_heap, rhs.m_heap, m_heapType);
    } else if (m_structure == LEFTIST) {
        m_heap = mergeLeftist(m_heap, rhs.m_heap, m_heapType);
    }

    // Update the size of the merged queue
    m_size += rhs.m_size;

    // Clear rhs
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
}
void PQueue::insertPatient(const Patient& patient) {
    // Create a new node with the given patient
    Node* newNode = new Node(patient);

    // decide the type of merge based on heapType and structure
    if (m_structure == SKEW) {
        m_heap = mergeSkew(m_heap, newNode, m_heapType);
    } else if (m_structure == LEFTIST) {
        m_heap = mergeLeftist(m_heap, newNode, m_heapType);
    }

    // Update the size of the queue
    m_size++;
}
int PQueue::numPatients() const{
    return m_size;
}
prifn_t PQueue::getPriorityFn() const {
    return m_priorFunc;
}
Patient PQueue::getNextPatient() {
    //conditions for empty queue
    if (m_heap == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    // Get the highest priority patient
    Patient highestPriorityPatient = m_heap->getPatient();
    //pointer to current heap
    Node* oldHeap = m_heap;

    // Depending on the data structure and heap type, merge the left and right subtrees
    //SKEW structure
    if (m_structure == SKEW) {
        //MINHEAP type
        if (m_heapType == MINHEAP) {
            m_heap = mergeSkew(m_heap->m_left, m_heap->m_right, m_heapType);
        }
        //MAXHEAP type
        else {
            m_heap = mergeSkew(m_heap->m_right, m_heap->m_left, m_heapType);
        }
    }
    //LEFTIST structure
    else if (m_structure == LEFTIST) {
        //MINHEAP type
        if (m_heapType == MINHEAP) {
            m_heap = mergeLeftist(m_heap->m_left, m_heap->m_right, m_heapType);
        }
        //MAXHEAP type
        else {
            m_heap = mergeLeftist(m_heap->m_right, m_heap->m_left, m_heapType);
        }
    }

    // Update the size of the queue & delete the old heap
    delete oldHeap;
    m_size--;

    return highestPriorityPatient;
}
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;
    m_heapType = heapType;
    //help to rebuild the queue based on new priority function
    rebuildQueueType(heapType, priFn);
}
void PQueue::setStructure(STRUCTURE structure){
    m_structure = structure;
    //help to rebuild the queue based on new structure
    rebuildQueueStructure(structure);
}
STRUCTURE PQueue::getStructure() const {
    return m_structure;
}
HEAPTYPE PQueue::getHeapType() const {
    return m_heapType;
}
void PQueue::printPatientQueue() const {
    cout << "Contents of the queue: " << endl;
    if (m_heap == nullptr) {
        cout << "The patient queue is empty." << endl;
    }
    //helper to print the patients in the queue
    else {
        preOrderPatientPrinter(m_heap);
    }
}
//Provided Function
void PQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty heap.\n" ;
    } else {
        dump(m_heap);
    }
    cout << endl;
}
//Provided Function
void PQueue::dump(Node *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
        else
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
        dump(pos->m_right);
        cout << ")";
    }
}
//Provided Function
ostream& operator<<(ostream& sout, const Patient& patient) {
    sout  << patient.getPatient()
          << ", temperature: " << patient.getTemperature()
          << ", oxygen: " << patient.getOxygen()
          << ", respiratory: " << patient.getRR()
          << ", blood pressure: " << patient.getBP()
          << ", nurse opinion: " << patient.getOpinion();
    return sout;
}
//Provided Function
ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getPatient();
    return sout;
}

//Helper Functions
Node* PQueue::mergeSkew(Node *left, Node *right, HEAPTYPE heapType) {
    //base case if left is null
    if (left == nullptr) {
        return right;
    }
    //base case if right is null
    if (right == nullptr) {
        return left;
    }
    //condition for minheap
    if (heapType == MINHEAP) {
        //if priority of left > right, swap them
        if (m_priorFunc(left->getPatient()) > m_priorFunc(right->getPatient())) {
            Node* temp = left;
            left = right;
            right = temp;
        }
    }
    //condition for maxheap
    else {
        //if priority of left < right, swap them
        if (m_priorFunc(left->getPatient()) < m_priorFunc(right->getPatient())) {
            Node* temp = left;
            left = right;
            right = temp;
        }
    }
    //merge for skew heap
    Node* temp = left->m_right;
    left->m_right = left->m_left;
    left->m_left = mergeSkew(right, temp, heapType);

    //return updated
    return left;
}
Node* PQueue::mergeLeftist(Node *left, Node *right, HEAPTYPE heapType) {
    //base case if left is null
    if (left == nullptr) {
        return right;
    }
    //base case if right is null
    if (right == nullptr) {
        return left;
    }
    //condition for minheap
    if (heapType == MINHEAP) {
        //if priority of left > right, swap them
        if (m_priorFunc(left->getPatient()) > m_priorFunc(right->getPatient())) {
            Node* temp = left;
            left = right;
            right = temp;
        }
    }
    //condition for maxheap
    else {
        //if priority of left < right, swap them
        if (m_priorFunc(left->getPatient()) < m_priorFunc(right->getPatient())) {
            Node* temp = left;
            left = right;
            right = temp;
        }
    }
    //merge for leftist heap
    left->m_right = mergeLeftist(left->m_right, right, heapType);

    //adjusting leftist heap  properties
    //if left is null, swap left and right
    if (left->m_left == nullptr) {
        left->m_left = left->m_right;
        left->m_right = nullptr;
    }
    //if left's left child has a smaller null path length than its right child, swap them
    else {
        if (left->m_left->getNPL() < left->m_right->getNPL()) {
            Node* temp = left->m_left;
            left->m_left = left->m_right;
            left->m_right = temp;
        }
        //update null path length
        left->m_npl = left->m_right->getNPL() + 1;
    }
    //return updated
    return left;
}
void PQueue::preOrderPatientPrinter(Node *node) const {
    //conditions if queue is not empty
    if (node != nullptr){
        //print patient
        cout << "[" << m_priorFunc(node->getPatient()) << "] " << node->getPatient() << endl;

        //recursively print left and right subtrees
        preOrderPatientPrinter(node->m_left);
        preOrderPatientPrinter(node->m_right);
    }
}
void PQueue::recursiveClear(Node *node) {
    //conditions if queue is not empty
    if (node != nullptr) {
        //recursively clear left and right subtrees
        recursiveClear(node->m_left);
        recursiveClear(node->m_right);
        //clear the node and delete
        node->m_right = nullptr;
        node->m_left = nullptr;
        delete node;
    }
}
void PQueue::rebuildQueueStructure(STRUCTURE structure) {
    //create a temporary array of patients
    Patient* patients = new Patient[m_size];
    int index = 0;

    // Insert all patients into the temporary array
    while (numPatients() > 0) {
        patients[index] = getNextPatient();
        index++;
    }

    // Clear the current queue
    clear();

    //set updated structure
    m_structure = structure;

    // Insert all patients back into the queue with the updated type/structure
    for (int i = 0; i < index; i++) {
        insertPatient(patients[i]);
    }

    // Deallocate the temporary array
    delete[] patients;
}
void PQueue::rebuildQueueType(HEAPTYPE heapType, prifn_t priFn){
    //create a temporary array of patients
    Patient* patients = new Patient[m_size];
    int index = 0;

    // Insert all patients into the temporary array
    while (numPatients() > 0) {
        patients[index] = getNextPatient();
        index++;
    }

    // Clear the current queue
    clear();

    //set updated heap type and priority function
    m_priorFunc = priFn;
    m_heapType = heapType;

    // Insert all patients back into the queue with the updated type/structure
    for (int i = 0; i < index; i++) {
        insertPatient(patients[i]);
    }

    // Deallocate the temporary array
    delete[] patients;
}
bool PQueue::checkHeapProperty(Node *node, prifn_t priFn, HEAPTYPE heapType) const {
    //base cade for reaching end of tree with no violations
    if (node == nullptr) {
        return true;
    }

    //initialize pointers to left and right children
    Node* leftChild = node->m_left;
    Node* rightChild = node->m_right;

    // Check heap property for the current node if not null
    // and check priority of left and curr node for violations
    if (leftChild != nullptr && ((heapType == MINHEAP && priFn(leftChild->getPatient()) < priFn(node->getPatient())) ||(heapType == MAXHEAP && priFn(leftChild->getPatient()) > priFn(node->getPatient())))) {
        return false;
    }
    // Check heap property for the current node if not null
    // and check priority of right and curr node for violations
    if (rightChild != nullptr && ((heapType == MINHEAP && priFn(rightChild->getPatient()) < priFn(node->getPatient())) ||(heapType == MAXHEAP && priFn(rightChild->getPatient()) > priFn(node->getPatient())))) {
        return false;
    }
    // Recursively check heap property for left and right subtrees
    return checkHeapProperty(leftChild, priFn, heapType) && checkHeapProperty(rightChild, priFn, heapType);
}
bool PQueue::checkLeftistProperty(Node *node) const {
    //base case for reaching end of tree with no violations
    if (node == nullptr) {
        return true;
    }

    //initialize pointers to left and right children
    Node* leftChild = node->m_left;
    Node* rightChild = node->m_right;

    // Check leftist property violations for the current node
    if (leftChild != nullptr && rightChild != nullptr && leftChild->getNPL() < rightChild->getNPL()) {
        return false;
    }

    // Recursively check leftist property for left and right subtrees
    return checkLeftistProperty(leftChild) && checkLeftistProperty(rightChild);
}
bool PQueue::checkLeftistNPL(Node *node) const {
    //base case for reaching end of tree with no violations
    if (node == nullptr) {
        return true;
    }

    //initialize pointers to left and right children
    Node* leftChild = node->m_left;
    Node* rightChild = node->m_right;

    // Check NPL values for the current node
    //checking internal nodes first
    if (leftChild != nullptr && rightChild != nullptr) {
        //NPL should be the smaller of left child NPL and right child + 1
        int expectedNPL = min(leftChild->getNPL(), rightChild->getNPL()) + 1;
        //if NPL is not equal to expected, return false
        if (node->getNPL() != expectedNPL) {
            return false;
        }
    }
    //checking external nodes
    else {
        //NPL should be 0 on external nodes
        if (node->getNPL() != 0) {
            return false;
        }
    }
    // Recursively check NPL values for left and right subtrees
    return checkLeftistNPL(leftChild) && checkLeftistNPL(rightChild);
}
Node* PQueue::copySubtree(const Node *rhsNode) {
    //base case for reaching end of tree
    if (rhsNode == nullptr) {
        return nullptr;
    }

    // Create a new node with the given patient
    Node* newNode = new Node(rhsNode->getPatient());
    newNode->m_npl = rhsNode->getNPL();

    // Recursively copy the left and right subtrees
    newNode->m_left = copySubtree(rhsNode->m_left);
    newNode->m_right = copySubtree(rhsNode->m_right);

    return newNode;
}
int PQueue::subtreeSize(const Node *node) {
    //base case for reaching end of tree
    if (node == nullptr) {
        return 0;
    }
    // Recursively calculate the size of the subtree left and right
    return 1 + subtreeSize(node->m_left) + subtreeSize(node->m_right);
}