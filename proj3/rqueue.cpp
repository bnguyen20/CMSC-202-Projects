// CMSC 341 - Spring 2021 - Project 3
// RQueue: 

#include "rqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

RQueue::RQueue(prifn_t priFn){
  //initialize values
  _heap = nullptr;
  _size = 0;
  priority = priFn;
}

RQueue::~RQueue(){
  clear();
}

RQueue::RQueue(const RQueue& rhs){
  //initialize values and populate heap based on rhs
  _size = rhs.numStudents();
  priority = rhs.getPriorityFn();
  copyStuff(_heap, rhs._heap);
}


RQueue& RQueue::operator=(const RQueue& rhs){
  //check for self assignment, clear memory, and initialize values/populate heap
  if(this != &rhs){
    clear();
    _size = rhs.numStudents();
    priority = rhs.getPriorityFn();
    copyStuff(_heap, rhs._heap);
  }
  return *this;
}

void RQueue::copyStuff(Node*& node, Node* rhsNode){
  if(rhsNode == nullptr)
    return;

  // traverse thru entire tree and grab appropriate values to copy
  node = new Node(rhsNode->getStudent());

  // make sure to link the nodes
  copyStuff(node->_left, rhsNode->_left);
  copyStuff(node->_right, rhsNode->_right);
}

void RQueue::insertStudent(const Student& input){
  //usual insertion
  Node* insert = new Node(input);
  _heap = insertHelper(_heap, insert);
  _size++;
}

Node* RQueue::insertHelper(Node* left, Node* right){
  Node* subRoot = nullptr;
  //only left heap exists
  if(left && !right)
    subRoot = left;
  //only right heap exists/if left heap is empty
  if(right && !left)
    subRoot = right;
  //if both heaps do not exist 
  if(!left && !right)
    subRoot = nullptr;

  if(left && right){
    //if left heap has more priority versus right heap then merge on the right of left heap
    if(priority(left->getStudent()) < priority(right->getStudent())){
      left->_right = insertHelper(left->_right, right);
      subRoot = left;
    }
    //if right heap has more priority than the left heap then 
    else{
      right->_right = insertHelper(right->_right, left);
      subRoot = right;
    }
  }

  //if we got a new sub root, then swap children of new sub root/whatever was in insertion path
  if(subRoot)
    swap(subRoot->_left, subRoot->_right);
  
  return subRoot;
}

Student RQueue::getNextStudent(){
  if(!_heap)
    throw domain_error("Empty heap");

  // get values from original root of heap before deleting
  Student returnStudent = _heap->getStudent();
  Node* leftHeap = _heap->_left;
  Node* rightHeap = _heap->_right;
  delete _heap;
  // merge the original root's left and right subheaps
  _heap = insertHelper(leftHeap, rightHeap);
  //deduct one node from heap
  _size--;
  //return original root's student
  return returnStudent;
}

void RQueue::mergeWithQueue(RQueue& rhs){
  //guard against self assignment
  if(this != &rhs){
    //ensure priority functions are the same
    if(getPriorityFn() == rhs.getPriorityFn()){
      _heap = insertHelper(_heap, rhs._heap);
    }
    else
      cout << "Queues must have same priority! " << endl;
  }
  else
    cout << "Identical queues. " << endl;

  //rhs heap is now empty
  rhs._heap = nullptr;
}

void RQueue::clear(){
  //delete RQueue and reset to default values
  deleteHelper(_heap);
  _heap = nullptr;
  _size = 0;
}

void RQueue::deleteHelper(Node* nodeToDelete){
  if(nodeToDelete == nullptr)
    return;
  
  deleteHelper(nodeToDelete->_left);
  deleteHelper(nodeToDelete->_right);
  
  delete nodeToDelete;
}

int RQueue::numStudents() const{
  return _size;
}

void RQueue::printStudentQueue() const{
  recursivePrint(_heap);
}

void RQueue::recursivePrint(Node* node) const{
  if(node != nullptr){
    //recursively print nodes using overloaded output operator
    cout << *node << endl;
    recursivePrint(node->_left);
    recursivePrint(node->_right);
  }
}

prifn_t RQueue::getPriorityFn() const{
  return priority;
}

void RQueue::setPriorityFn(prifn_t priFn){
  //change priority and rebuild heap based on old heap/priority
  priority = priFn;
  Node* temp = _heap;

  //rebuild heap
  insertStudent(temp->getStudent());
  temp = nullptr;
}

void RQueue::getStudents(Node* left, RQueue*& rhs){
  if(left != nullptr){
    //remake the heap using new priority
    rhs->insertStudent(left->getStudent());
    getStudents(left->_left, rhs);
    getStudents(left->_right, rhs);
  } 
}

void RQueue::remakeQueue(RQueue*& lhs, Node* rhs){
  if(rhs != nullptr){
    lhs->insertStudent(rhs->getStudent());
    remakeQueue(lhs, rhs->_left);
    remakeQueue(lhs, rhs->_right);
  }
}

// for debugging
void RQueue::dump() const
{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr() 
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}
