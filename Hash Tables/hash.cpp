// CMSC 341 - Spring 2021 - Project 4
// hash.cpp
// The implementation of the class HashTable
#include "hash.h"
#include "math.h"
HashTable::HashTable(unsigned size, hash_fn hash){
    //make sure number is prime and between MINPRIME and MAXPRIME
    if(size < MINPRIME)
        size = MINPRIME;
    else if(size > MAXPRIME)
        size = MAXPRIME;
    if(!isPrime(size))
        size = findNextPrime(size);
    //initializes values
    m_capacity = size;
    m_size = 0;
    m_numDeleted = 0;
    m_hash = hash;
    m_table = new Person[m_capacity];
    m_currentProb = LINEAR;
}

HashTable::~HashTable(){
    delete [] m_table;
    m_table = nullptr;
}

bool HashTable::insert(Person person){
    //calculated index value from person's name
    unsigned int hashKey = m_hash(person.key());
    unsigned int insertIndex = hashKey % tableSize();
    bool checkInsert = false;
    int i = 0;
    //if the desired index is occupied
    if(!(m_table[insertIndex] == EMPTY)){
        //linear probing
        if(m_currentProb == LINEAR){
            while(!(m_table[insertIndex] == EMPTY)){
                i++;
                insertIndex = (hashKey + i) % tableSize();
            }
            //insertion successful
            m_table[insertIndex] = person;
            checkInsert = true;
        }
        //quadratic probing
        else if(m_currentProb == QUADRATIC){
            unsigned int j = 0;
            int tempIndex = insertIndex;
            while(!(m_table[tempIndex] == EMPTY)){
                //usual quadratic probing while also checking for bounds
                j++;
                tempIndex = (hashKey % tableSize() + j*j) % tableSize();
            }
            //insertion successful
            m_table[tempIndex] = person;
            checkInsert = true;
        }
    }

    //if the desired spot is vacant, then simply insert
    else if(m_table[insertIndex] == EMPTY){
        m_table[insertIndex] = person;
        checkInsert = true;
    }

    //if insertion was successful then increase size
    if(checkInsert)
        m_size++;

    //check load factor
    if(lambda() > .5)
        setProbingPolicy(m_currentProb);

    return checkInsert;
}

bool HashTable::remove(Person person){
    unsigned int hashKey = m_hash(person.key());
    unsigned int removeIndex = hashKey % tableSize();
    bool checkRemove = false;
    int i = 0;

    //if the desired remove spot is vacant
    if(!(m_table[removeIndex] == person)){
        if(m_currentProb == LINEAR){
            while(!(m_table[removeIndex] == person)){
                i++;
                removeIndex = (hashKey + i) % tableSize();
            }
            //removal successful
            m_table[removeIndex] = DELETED;
            checkRemove = true;
        }

        else if(m_currentProb == QUADRATIC){
            unsigned int j = 0;
            int tempIndex = removeIndex;
            while(!(m_table[tempIndex] == person)){
                j++;
                tempIndex = (hashKey % tableSize() + j*j) % tableSize();
            }
            m_table[tempIndex] = DELETED;
            checkRemove = true;
        }
    }

    //simple removal
    else if(m_table[removeIndex] == person){
        m_table[removeIndex] = DELETED;
        checkRemove = true;
    }

    //removal successful
    if(checkRemove)
        m_size--;

    if(lambda() > .5)
        setProbingPolicy(m_currentProb);

    if(deletedRatio() / 4){
        setProbingPolicy(m_currentProb);
    }

    return checkRemove;
}

void HashTable::setProbingPolicy(probing prob){
    if(prob == LINEAR) {
        m_currentProb = QUADRATIC;
        cout << "quad" << endl;
    }
    if(prob == QUADRATIC) {
        m_currentProb = LINEAR;
        cout << "LINEAR" << endl;
    }
    hashHelper();
}

void HashTable::hashHelper() {
    //calculated newSize
    unsigned int newSize = findNextPrime(4*(numEntries() - m_numDeleted));
    //new table with new size
    Person* newTable = new Person[newSize];

    //mark everything empty in new table
    for(unsigned int i = 0; i < m_capacity; i++)
        newTable[i] = EMPTY;

    //keep track of old m_table
    Person* oldTable = m_table;
    //deallocate memory and have m_table assigned to new table with new size
    delete [] m_table;
    m_table = newTable;

    //make sure element inserted into the new m_table will not be empty or deleted object
    for(unsigned int j = 0; j < m_capacity; j++){
        if(!(oldTable[j] == EMPTY) && !(oldTable[j] == DELETED)){
            insert(oldTable[j]);
        }
    }
    m_capacity = newSize;

}

float HashTable::lambda() const {
    return (numEntries() / tableSize());
}

float HashTable::deletedRatio() const {
    return (m_numDeleted / numEntries());
}

void HashTable::dump() const {
    for (int i = 0; i < m_capacity; i++) {
        cout << "[" << i << "] : " << m_table[i] << endl;
    }
}

bool HashTable::isPrime(int number){
    // If number is prime this function returns true
    // otherwise it returns false
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current){
    //we won't go beyond MAXPRIME
    //the smallest prime would be next after MINPRIME
    if (current < MINPRIME) current = MINPRIME;
    for (int i=current; i<MAXPRIME; i++) {
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0)
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
