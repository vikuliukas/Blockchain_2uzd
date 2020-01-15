#ifndef CHAIN_H
#define CHAIN_H

#include <iostream>
#include <vector>
#include <chrono>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <bits/stdc++.h>
#include <random>

class hash{
    private:
    std::string input_;
    std::string fixedinput_;
    std::string output_;

    public:
    std::vector<int> values = {};
    hash(){
        input_ = "";
        fixedinput_ = "";
        output_ = "";
    }
    hash( std::string input ){
        input_ = input;
        fixedinput_ = "";
        output_ = "";
    }
    void setinput( std::string &input ) { input_ = input; }
    void setfixedinput( std::string &fixed) { fixedinput_ = fixed; }
    void setoutput( std::string &output) { output_ = output; }
    std::string const getinput () { return input_; }
    std::string const getfixedinput () { return fixedinput_; }
    std::string const getoutput () { return output_; }
    ~hash(){};
};

void makelongerifneeded( hash &current );
void hashalgorithm( hash &current );
std::string inttohex( unsigned long long int &i );
std::string hashshuffle( std::string &unshuffledhash, hash &current );
std::string createHash( std::string &word);

class User{
private:

    std::string name_;
    double balance_;
public:

    std::string publicKey_;

    User(){
        name_ = "";
        balance_ = 0;
        publicKey_ = "";
    };
    User(std::string name, double balance){
        name_ = name;
        balance_ = balance;
        publicKey_ = createHash( name );
    }
    User(std::string name){
        name_ =  name;
        balance_ = 0;
        publicKey_ = createHash( name );
    }
    void setBalance( double balance){ balance_ = balance; };
    const double getBalance() { return balance_; };
    const std::string getName() { return name_; };
    const std::string getPublicKey() { return publicKey_; };
    ~User(){};
};

void generateUsers( std::vector<User> & Naudotojai );

class Transaction{
private:

    std::string ID_;
public:

    double amount_;
    std::string senderKey_;
    std::string receiverKey_;
    
    Transaction(){};
    
    Transaction(double amount, std::string senderKey, std::string receiverKey){
        amount_ = amount;
        senderKey_ = senderKey;
        receiverKey_ = receiverKey;
        std::string stringAmmount = std::to_string( amount );
        std::string temp = senderKey_ + receiverKey_ + stringAmmount;
        ID_ = createHash( temp );
    };

    const double getAmount() { return amount_; };
    const std::string getSenderKey() { return senderKey_; };
    const std::string getReceiverKey() { return receiverKey_; };
    const std::string getID() {return ID_; };
    ~Transaction(){};
};

void generateTransactions(std::vector<Transaction> & Transakcijos, std::vector<User> & Naudotojai);
void validateTransactions(std::vector<Transaction> & Transakcijos, std::vector<User> & Naudotojai, std::vector<Transaction> & validTransactions);
std::string generateMerkelRoot(std::vector<Transaction> data);

class Block{
private:
        
    std::string blockHash_;
    std::string previousHash_;
    std::string merkelRootHash_;
    time_t timestamp_;

public:
    std::vector<Transaction> data_;
    int nonce, diffTarget;

    Block(){
        data_ = {};
        blockHash_ = "";
        previousHash_ = "";
        merkelRootHash_ ="";
        timestamp_ = 0;
        nonce = 0;
        diffTarget = 0;
    };
    Block(std::vector<Transaction> & data, std::string & previousHash){
        previousHash_ = previousHash;
        data_ = data;
        merkelRootHash_ = generateMerkelRoot(data);
        std::string word = "";
        word = merkelRootHash_ + previousHash_;
        blockHash_ = createHash(word);
        timestamp_ = 0;
        nonce = 0; 
        diffTarget = 0;
    }
    void hashBlock(){
        merkelRootHash_ = generateMerkelRoot(data_);
        std::string word = "";
        word = merkelRootHash_ + previousHash_;
        blockHash_ = createHash(word);
    }
    void setTimeStamp( time_t & time){ timestamp_ = time; };
    std::string const getHash() { return blockHash_; };
    std::string const getPreviousHash() {return previousHash_; };
    ~Block(){ data_.clear(); };
};

class Blockchain{
private:

    std::vector<Block> chain_;
    void createGenesisBlock() {
        Block genesis;
        chain_.push_back(genesis);
    }

public:

    Blockchain(){ createGenesisBlock(); };
    const unsigned int getBlockChainSize() { return chain_.size(); };
    const std::string getBlockHash(int a) { return chain_[a].getHash(); };
    void addBlock( Block & a){
        chain_.push_back(a);
    }
    ~Blockchain(){ chain_.clear(); };
};

void minerAmount(std::vector<Transaction> & validTransactions, std::vector<Block> & mineriai);
bool mine(Block & a);
void printChain( Blockchain & BlokoGrandine);

#endif