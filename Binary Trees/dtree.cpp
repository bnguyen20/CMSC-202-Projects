/**
  * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree() {
  clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
  if(this != &rhs){
    // clear memory
    clear();
    // use copy helper
    copyStuff(_root, rhs._root);
  }
  return *this;
}

void DTree::copyStuff(DNode*& Node, DNode* rhsNode){
  if(rhsNode == nullptr)
    return;

  // traverse thru entire tree and grab appropriate values to copy
  Node = new DNode(rhsNode->_account);
  Node->_size = rhsNode->_size;
  Node->_numVacant = rhsNode->_numVacant;
  Node->_vacant = rhsNode->_vacant;

  // make sure to link the nodes
  copyStuff(Node->_left, rhsNode->_left);
  copyStuff(Node->_right, rhsNode->_right);
}

/**
 * Dynamically allocates a new DNode in the tree. 
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {
    bool insertValid;
    // empty tree
    if(_root == nullptr) {
        _root = new DNode(newAcct);
        insertValid = true;
    }
    // insert normally
    else {
        insertValid = recursiveInsert(_root, newAcct);
    }
    return insertValid;
}

bool DTree::recursiveInsert(DNode*& curr, Account newAcct){
  // if the new account's discriminator is <= current node's discriminator
  if(newAcct.getDiscriminator() < curr->_account.getDiscriminator()){
    // if there is no left child node
    if(curr->_left == nullptr){
      curr->_left = new DNode(newAcct);
      // update sizes, vacant num, check for imbalance, and rebalance if needed
      updateSize(curr);
      updateNumVacant(curr);
      if(checkImbalance(curr))
	rebalance(curr);
      return true;
    }
    // go left and keep inserting
    else if(curr->_left != nullptr){
      bool tempBool = recursiveInsert(curr->_left, newAcct);
      updateSize(curr);
      updateNumVacant(curr);
      if(checkImbalance(curr))
	rebalance(curr);
      return tempBool;
    }
  }
  // if the new account's discriminator is > current node's discriminator
  else if(newAcct.getDiscriminator() > curr->_account.getDiscriminator()){
    // if there is no right child node
    if(curr->_right == nullptr){
      curr->_right = new DNode(newAcct);
      updateSize(curr);
      updateNumVacant(curr);
      if(checkImbalance(curr))
	rebalance(curr);
      return true;
    }
    // go right and keep inserting
    else if(curr->_right != nullptr){
      bool tempBool = recursiveInsert(curr->_right, newAcct);
      updateSize(curr);
      updateNumVacant(curr);
      if(checkImbalance(curr))
        rebalance(curr);
      return tempBool;
    }
  }

  // if we found an account with the same discriminator
  else if(newAcct.getDiscriminator() == curr->_account.getDiscriminator())
    return false;
    
  return false;
}

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode*& removed) {
  if(removed == nullptr)
    return false;

  // we found the DNode with appropriate discriminator
  if(removed->getDiscriminator() == disc) {
    cout << removed->getDiscriminator() << " and " << disc << endl;
    removed->_vacant = true;
    updateNumVacant(removed);
    return true;
  }
  
  remove(disc, removed->_left);
  remove(disc, removed->_right);

}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
  DNode* findMatch = retrieveHelper(_root, disc);
  return findMatch;
}

DNode* DTree::retrieveHelper(DNode* traverse, int disc){
  if(traverse != nullptr){
    // if disc is greater than desired one, go left
    if(traverse->getDiscriminator() > disc)
      return retrieveHelper(traverse->_left, disc);

    // if disc is lesser than desired one, go right
    else if(traverse->getDiscriminator() < disc)
      return retrieveHelper(traverse->_right, disc);

    // if node is equal and is not vacant
    else if(traverse->getDiscriminator() == disc && !traverse->isVacant())
      return traverse;
  }
  // node not found
  return nullptr;
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear() {
  deleteHelper(_root);
}

void DTree::deleteHelper(DNode* nodeToDelete){
  if(nodeToDelete == nullptr)
    return;
  
  deleteHelper(nodeToDelete->_left);
  deleteHelper(nodeToDelete->_right);
  
  nodeToDelete->_size = DEFAULT_SIZE;
  nodeToDelete->_numVacant = DEFAULT_NUM_VACANT;
  nodeToDelete->_vacant = false;
  nodeToDelete->_left = nullptr;
  nodeToDelete->_right = nullptr;
  delete nodeToDelete;
  
}

/**
 * Prints all accounts' details within the DTree.
 */
void DTree::printAccounts() const {
  recursivePrint(_root);
}

void DTree::recursivePrint(DNode *Node) const{
    if(Node == nullptr){
        return;
    }
    recursivePrint(Node->_left);

    // make sure node is vacant 
    if(!Node->isVacant())
      cout << Node->getAccount() << endl;

    recursivePrint(Node->_right);
}

/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
  int num = 0;
  int numUsers = findUsers(_root, num);
  return numUsers;
}

int DTree::findUsers(DNode* Node, int num) const{
  if(Node != nullptr){
    findUsers(Node->_left, num);
    findUsers(Node->_right, num);

    // only count nodes that are not vacant
    if(!Node->isVacant())
      num++;
  }

  return num;
}

/**
 * Updates the size of a node based on the imedaite children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* Node) {
  int temp1 = DEFAULT_SIZE - 1;
  int temp2 = DEFAULT_SIZE - 1;
  
  // if there is a left child, that size will be added
  if(Node->_left != nullptr){
    temp1 = Node->_left->getSize();
  }
  
  // if there is a right child, that size will be added
  if(Node->_right != nullptr){
    temp2 = Node->_right->getSize();
  }
  
  // add the size of the immediate children plus the root itself
  Node->_size = temp1 + temp2 + 1;
}

/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode* node) {
  int temp1 = DEFAULT_NUM_VACANT;
  int temp2 = DEFAULT_NUM_VACANT;
  int temp3 = DEFAULT_NUM_VACANT;

  // sees if left and right children exist
  if(node->_left != nullptr){
    temp1 = node->_left->getNumVacant();
  }
  if(node->_right != nullptr){
    temp1 = node->_right->getNumVacant();
  }
  // make sure node is vacant
  if(node->isVacant()){
    temp3++;
  }
  // updates num vacant of current node
  node->_numVacant = temp1 + temp2 + temp3;
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode* node) {
  int leftSize = 0;
  int rightSize = 0;
  // also checks for children's existence
  if(node->_left != nullptr)
    leftSize = node->_left->getSize();
  if(node->_right != nullptr)
    rightSize = node->_right->getSize();

  // one child has to be at least a size of 4
  if(leftSize >= 4 || rightSize >= 4){
    // left imbalance
    if(leftSize >= rightSize * 1.5){
      cout << "left imbalance at node " << node->getDiscriminator() << endl;
      return true;
    }
    // right imbalance
    else if(rightSize >= leftSize * 1.5){
      cout << "right imbalance at node " << node->getDiscriminator() << endl;
      return true;
    }
  }
  else
    return false;
}

//----------------
/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (pass by reference).
 * @param node DNode root of the subtree to balance
 */

/*
void DTree::rebalance(DNode*& node) {
}
*/

void DTree::makeArray(DNode*& Node, DNode** array, int& i){
  if(Node == nullptr)
    return;
  // inserts each DNode into proper position in array
  makeArray(Node->_left, array, i);
  array[i] = Node;
  i++;
  
  makeArray(Node->_right, array, i);
}

DNode* DTree::rebalanceArray(DNode** array, int start, int end) {
  if(start > end)
    return nullptr;

  int mid = (start + end) / 2;
  DNode* root = array[mid];

  // depending on what half we are on in the array, we will
  // go to that corresponding direction in the tree
  root->_left = rebalanceArray(array, start, mid - 1);
  root->_right = rebalanceArray(array, mid + 1, end);

  updateSize(root);
  updateNumVacant(root);
  
  return root;
}

// -- OR --

/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
void DTree::rebalance(DNode*& node) {
  // make dynamic array to hold DNodes
  DNode** array = new DNode*[node->getSize()];
  int i = 0;
  int start = 0;
  // converts binary tree to dynamic array
  makeArray(node, array, i);

  int end = node->getSize();
  // rebalances the tree/converted array
  node = rebalanceArray(array, start, end - 1);

  delete array;
  
}
//----------------

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() << 
            "\n\tDiscriminator: " << acct.getDiscriminator() <<
            "\n\tNitro: " << acct.hasNitro() << 
            "\n\tBadge: " << acct.getBadge() << 
            "\n\tStatus: " << acct.getStatus();
    return sout;
}
