// CMSC 341 - Spring 2021 - Project 3
// driver.cpp: sample driver for the RQueue class.

#include "rqueue.h"
using namespace std;
// Priority functions compute an integer priority for a student. Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Student &tudent);
int priorityFn2(const Student &student);
bool testCopy(const RQueue &queue);
bool testAssign(const RQueue &queue);
void measureInsertionTime(int numTrials, int N, int O);
void measureRemoveTime(int numTrials, int N, RQueue* queue);

int main() {

  RQueue queue1(priorityFn1);
  RQueue queue2(priorityFn1);

  // Create some test students and insert them into the queue
  //Student(string name, int priority, int year, int major, int group)
  Student student1("Sam",1,1,1,1);
  Student student2("Liz",5,1,2,0);
  Student student3("Nick",2,0,0,0);
  Student student4("Eva",4,3,2,1);

  queue1.insertStudent(student3);
  queue1.insertStudent(student1);
  queue1.insertStudent(student2);
  queue1.insertStudent(student4);

  std::cout << "\nqueue1 dump after inserting 4 students (Nick, Sam, Liz, Eva):" << std::endl;
  queue1.dump();
  cout << endl;

  //Testing assignment operator
  cout << "Testing assignment operator " << endl;
  if(testAssign(queue1)){
    cout << "Assignment operator test passed " << endl;
  }
  else{
    cout << "Assignment operator test failed " << endl;
  }
  
  //Testing copy constructor
  cout << "Testing copy constructor " << endl;
  if(testCopy(queue1)){
    cout << "Assignment operator test passed " << endl;
  }
  else{
    cout << "Assignment operator test failed " << endl;
  }

  Student student5("John",6,3,2,1);
  Student student6("Mia",3,0,0,0);

  queue2.insertStudent(student5);
  queue2.insertStudent(student6);

  std::cout << "\nqueue2 dump after inserting 2 students (John, Mia):" << std::endl;
  queue2.dump();

  queue1.mergeWithQueue(queue2);

  // Print the queue, change priority function, print again, change
  // again, print again.  Changing the priority function should cause
  // the heap to be rebuilt.
  
  std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after merging with queue2:\n";
  queue1.dump();

  queue1.setPriorityFn(priorityFn2);

  std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
  queue1.dump();

  queue1.setPriorityFn(priorityFn1);
  
  std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
  queue1.dump();
  
  // Remove and print students in priority order
  
  std::cout << "\nFinal queue1 in priority order:\n";
  while (queue1.numStudents() > 0) {
    Student student = queue1.getNextStudent();
    std::cout << "[" << priorityFn1(student) << "] " << student << std::endl;
  }

  //Measuring the efficiency of insertion functionality
  cout << "\nMeasuring the efficiency of insertion functionality:" << endl;
  int M = 7;//number of trials
  int N = 2000;//original input size
  measureInsertionTime(M, N, N);
  
  return 0;
}

bool testAssign(const RQueue &queue){
  RQueue copyQueue = queue;
  if(copyQueue.getPriorityFn() == queue.getPriorityFn() && copyQueue.numStudents() == queue.numStudents()){
    cout << "Printing copy queue " << endl;
    copyQueue.printStudentQueue();

    cout << "Printing original queue " << endl;
    queue.printStudentQueue();
    return true;
  }
  else
    return false;
}

int priorityFn1(const Student&student) {
  //priority value is determined based on some criteria
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  int priority = student.getYear() + student.getMajor() + student.getGroup();
  return priority;
}

int priorityFn2(const Student&student) {
  //priority is determined by an officer in registrar office
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  return student.getPriority();
}

bool testCopy(const RQueue &queue){
  RQueue copyQueue(queue);
  if(copyQueue.getPriorityFn() == queue.getPriorityFn() && copyQueue.numStudents() == queue.numStudents()){
    cout << "Printing copy queue " << endl;
    copyQueue.printStudentQueue();
    
    cout << "Printing original queue " << endl;
    queue.printStudentQueue();
    return true;
  }
  else
    return false;
}

void measureInsertionTime(int numTrials, int N, int O){
  //Measuring the efficiency of insertion algorithm with system clock ticks
  //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
  //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
  const int a = 2;//scaling factor for input size
  double T = 0.0, U = 0.0;//to store running times
  clock_t start, stop, altStart, altStop;//stores the clock ticks while running the program
  for (int k=0;k<numTrials-1;k++){
    start = clock();
    RQueue queue(priorityFn1);
    for(int i = 0; i < N; i++){
      Student student1;
      queue.insertStudent(student1); //the algorithm to be analyzed for efficiency
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    cout << "Inserting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
    
    altStart = clock();
    for(int j = 0; j < N; j++)
      queue.getNextStudent();
    altStop = clock();
    U = altStop = altStart;
    cout << "Removing " << N << " members took " << U << " clock ticks ("<< U/CLOCKS_PER_SEC << " seconds)!" << endl;
    
    N = N * a;//increase the input size by the scaling factor
  }
}

