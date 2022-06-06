/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
  clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
  bool insertValid;
  // if UTree is empty make a root
  if(_root == nullptr){
    _root = new UNode();
    _root->_dtree->insert(newAcct);
    insertValid = true;
  }

  // insert normally
  else
    insertValid = recursiveInsert(_root, newAcct);
  
  return insertValid;
}

bool UTree::recursiveInsert(UNode* curr, Account newAcct){
  // compare usernames' ascii values
  // go left to insert or traverse, update heights, and check imbalances as we go
  if(newAcct.getUsername() < curr->getUsername()){
    if(curr->_left == nullptr){
      curr->_left = new UNode();
      curr->_left->_dtree->insert(newAcct);
      updateHeight(curr);
      checkImbalance(curr);
      return true;
    }
    else if(curr->_left != nullptr){
      bool tempBool = recursiveInsert(curr->_left, newAcct);
      updateHeight(curr);
      checkImbalance(curr);
      return tempBool;
    }
  }
  // go right to insert or traverse, update heights, and check imbalances as we go
  else if(newAcct.getUsername() > curr->getUsername()){
    if(curr->_right == nullptr){
      curr->_right = new UNode();
      curr->_right->_dtree->insert(newAcct);
      updateHeight(curr);
      checkImbalance(curr);
      return true;
    }
    else if(curr->_right != nullptr){
      bool tempBool = recursiveInsert(curr->_right, newAcct);
      updateHeight(curr);
      checkImbalance(curr);
      return tempBool;
    }
  }

  // avoid having the same usernames
  else if(newAcct.getUsername() == curr->getUsername()){
    return false;
  }
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
  
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
  UNode* match = findUNode(username, _root);
  if(match == nullptr)
    return nullptr;
  else if(match->getUsername() == username){
    cout << "we found " << username << endl;
    return match;
  }
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
  retrieveRecursive(username, disc, _root);
}

DNode* UTree::retrieveRecursive(string username, int disc, UNode* node){
  if(node == nullptr){
    return nullptr;
  }
  
  retrieveRecursive(username, disc, node->_left);
  // if the UNode is not nullptr
  if(findUNode(username, node) != nullptr){
    // if the DNode is not nullptr then we have found the DNode
    // with appropriate username and disc
    if(node->getDTree()->retrieve(disc) != nullptr)
      return node->getDTree()->retrieve(disc);
  }
  retrieveRecursive(username, disc, node->_right);
}

UNode* UTree::findUNode(string username, UNode* node){
  if(node != nullptr){

    // use ascii values of usernames to find corresponding UNode
    if(node->getUsername() > username){
      return findUNode(username, node->_left);
    }

    else if(node->getUsername() < username){
      return findUNode(username, node->_right);
    }
    
    if(node->getUsername() == username)
      return node;
  }

  // did not find the appropriate UNode
  return nullptr;
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
  // i variable keeps track of users with specific username
  int i = 0;
  findUsers(username, _root, i);
  return i;
}

void UTree::findUsers(string username, UNode* node, int& i){
  if(node == nullptr)
    return;
  
  findUsers(username, node->_left, i);

  // found a UNode with appropriate username
  if(node->getDTree()->getUsername() == username){
    cout << "match " << endl;
    i++;
  }
  
  findUsers(username, node->_right, i);

}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
  deleteHelper(_root);
}

void UTree::deleteHelper(UNode* nodeToDelete){
  if(nodeToDelete == nullptr)
    return;
  
  deleteHelper(nodeToDelete->_left);
  deleteHelper(nodeToDelete->_right);
  
  nodeToDelete->_height = DEFAULT_HEIGHT;
  nodeToDelete->_left = nullptr;
  nodeToDelete->_right = nullptr;
  delete nodeToDelete;
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
  printUNodes(_root);
}

void UTree::printUNodes(UNode* node) const{
  if(node == nullptr)
    return;
  
  printUNodes(node->_left);
  // calls the DTree in each UNode and prints all accounts within them
  node->getDTree()->printAccounts();
  printUNodes(node->_right);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
  int temp1 = DEFAULT_HEIGHT - 1;
  int temp2 = DEFAULT_HEIGHT - 1;

  //check to see if children exist
  if(node->_left != nullptr)
    temp1 = node->_left->getHeight();
  
  if(node->_right != nullptr)
    temp2 = node->_right->getHeight();

  node->_height = findMax(temp1, temp2);
}

int UTree::findMax(int temp1, int temp2){
  // find the max height between left and right children
  if(temp1 > temp2)
    return temp1 + 1;
  else if(temp2 > temp1)
    return temp2 + 1;
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
  int temp1 = DEFAULT_HEIGHT - 1;
  int temp2 = DEFAULT_HEIGHT - 1;
  
  if(node->_left != nullptr)
    temp1 = node->_left->getHeight();
  
  if(node->_right != nullptr)
    temp2 = node->_right->getHeight();

  int difference = temp1 - temp2;

  // finds absolute value of difference 
  if(difference < 0)
    difference = difference * -1;

  // imbalance detected
  if(difference > 1)
    return 1;

  // no imbalance
  else if(difference >= 0 && difference <= 1)
    return 0;
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode*& node) {

// -- OR --
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UTree* UTree::rebalance(UNode* node) {

//}
//----------------
