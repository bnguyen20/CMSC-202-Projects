#include "utree.h"
#include <random>

#define NUMACCTS 20
#define RANDDISC (distAcct(rng))

std::mt19937 rng(10);
std::uniform_int_distribution<> distAcct(0, 9999);

class Tester {
public:
  bool testBasicDTreeInsert(DTree& dtree);

  bool testBasicUTreeInsert(UTree& utree);

  bool testRetrieveDTree(DTree& dtree);

  void testPrintAccounts(DTree& dtree);

  bool testDTreeAssignOp(DTree& dtree);

  bool testDTreeRemoval(DTree& dtree);

  bool assignOpHelper(DNode* Node, DNode* rhs);

  void testPrintAccountsUTree(UTree& utree);

  bool testRetrieveUTree(UTree& utree);

  void testUTreeFindUsers(UTree& utree);

  void testUTreeRetrieveUser(UTree& utree);
};

bool Tester::testBasicDTreeInsert(DTree& dtree) {
    bool allInserted = true;
    for(int i = 0; i < NUMACCTS; i++) {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if(!dtree.insert(newAcct)) {
          cout << "Insertion on node " << disc << " did not return true" << endl;
          allInserted = false;
        }
    }
    //    cout << "Root disc is " << dtree._root->getDiscriminator() << endl;
    return allInserted;
}

bool Tester::testBasicUTreeInsert(UTree& utree) {
    string dataFile = "accounts.csv";
    try {
        utree.loadData(dataFile);
    } catch(std::invalid_argument &e) {
        std::cerr << e.what() << endl;
        return false;
    }
    return true;
}

bool Tester::testRetrieveDTree(DTree &dtree){
    int disc = 6336;
    bool test;

    //did not find any node with discriminator
    if(dtree.retrieve(disc) == nullptr){
      test = false;
    }

    // found node with discriminator
    else if(dtree.retrieve(disc)->getDiscriminator() == disc){
      cout << dtree.retrieve(disc)->getDiscriminator() << endl;
      test = true;
    }
    return test;
}

void Tester::testPrintAccounts(DTree& dtree){
  dtree.printAccounts();
}

bool Tester::testDTreeAssignOp(DTree& dtree){
  DTree copyTree;
  copyTree = dtree;
  if(assignOpHelper(copyTree._root, dtree._root))
    return false;

  cout << "Printing original tree " << endl;
  dtree.printAccounts();
  cout << "Printing copy of original tree " << endl;
  copyTree.printAccounts();

  return true;
}

bool Tester::assignOpHelper(DNode* Node, DNode* rhs){
  if(Node == rhs)
    return false;

  assignOpHelper(Node->_left, rhs->_left);
  assignOpHelper(Node->_right, rhs->_right);
}

bool Tester::testDTreeRemoval(DTree& dtree){
  bool test;
  if(dtree.remove(633, dtree._root)){
    test = true;
  }
  else if(!dtree.remove(633, dtree._root)){
    test =  false;
  }
  return test;
}
void Tester::testPrintAccountsUTree(UTree& utree){
  utree.printUsers();
}

bool Tester::testRetrieveUTree(UTree& utree){
  string username = "Brackle";
  bool test;
  if(utree.retrieve(username) == nullptr)
    test = false;
  if(utree.retrieve(username)->getUsername() == username){
    test = true;
  }
  return test;
}

void Tester::testUTreeFindUsers(UTree& utree){
  string username = "Pika";
  int num = utree.numUsers(username);
  cout << num << " users found with the name " << username << endl;
}

void Tester::testUTreeRetrieveUser(UTree& utree){
  string username = "Brackle ";
  int disc = 9550;
  utree.retrieveUser(username, disc);

}

int main() {
    Tester tester;

    /* Basic dtree tests */
    DTree dtree;

    cout << "Testing DTree insertion..." << endl;
    if(tester.testBasicDTreeInsert(dtree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting DTree:" << endl;
    dtree.dump();
    cout << endl;

    /* DTree Retrieval */
    cout << "Testing DTree Retrieval..." << endl;
    if(tester.testRetrieveDTree(dtree)){
      cout << "This discrminator is found! Test passed. " << endl;
    }
    else{
      cout << "This discriminator could not be found. Test failed.  " << endl;
    }

    /* DTree Account Printing */
    cout << "Testing DTree account printing..." << endl;
    //    tester.testPrintAccounts(dtree);

    /* DTree Removal */
    cout << "Testing DTree Removal..." << endl;
    if(tester.testDTreeRemoval(dtree))
      cout << "Removal passed. " << endl;
    else if(!tester.testDTreeRemoval(dtree))
      cout << "Removal unsuccessful. " << endl;

    /* DTree Assignment Operator Testing */
    cout << "Testing DTree Assignment Operator " << endl;
    if(tester.testDTreeAssignOp(dtree)){
      cout << "assignment op passed. " << endl;
    }
    else {
      cout << "assignment op failed. " << endl;
    }

    /* Basic UTree tests */
    UTree utree;

    cout << "\n\nTesting UTree insertion...";
    if(tester.testBasicUTreeInsert(utree)) {
      cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting UTree:" << endl;
    utree.dump();
    cout << endl;

    cout << "Printing UTree accounts " << endl;
    tester.testPrintAccountsUTree(utree);

    cout << "Testing UTree retrieval " << endl;
    if(tester.testRetrieveUTree(utree)){
      cout << "test passed" << endl;
    }
    else{
      cout << "test failed " << endl;
    }

    cout << "Getting num users for UTree " << endl;
    tester.testUTreeFindUsers(utree);

    cout << "Testing retrieving user for UTree " << endl;
    tester.testUTreeRetrieveUser(utree);

    return 0;
}
