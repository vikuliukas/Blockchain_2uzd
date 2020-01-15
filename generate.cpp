#include "chain.h"

void generateUsers( std::vector<User> & Naudotojai ){
    std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<double> uni(1000.0,1000000.0);
    for(int i = 0; i < 1000; i++){
        std::string name = "Vardas" + std::to_string(i);
        double amount = uni(rng);
        User naudotojas( name, amount);
        Naudotojai.push_back(naudotojas);
    }

    std::ofstream r ("users.txt");
    for(unsigned int i = 0; i < Naudotojai.size(); i++){
        r<<Naudotojai[i].getName()<<"\n";
        r<<Naudotojai[i].getPublicKey()<<"\n";
        r<<Naudotojai[i].getBalance()<<std::setprecision(8)<<"\n";
        r<<"\n";
    }
    r.close();
}

void generateTransactions(std::vector<Transaction> & Transakcijos, std::vector<User> & Naudotojai){
    std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<double> uni(10.0,100000.0);
    std::uniform_int_distribution<int> uni1(0, 999);
    double amount;
    int a,b;
    std::string sender, receiver;
    for( int i = 0; i < 1000; i++){
        amount = uni(rng);
        a = uni1(rng);
        b = uni1(rng);
        while( a == b){
            b = uni1(rng);
        }
        sender = Naudotojai[a].getPublicKey();
        receiver = Naudotojai[b].getPublicKey();
        Transaction transakcija(amount, sender, receiver );
        Transakcijos.push_back(transakcija);
    }

    std::ofstream r ("transactions.txt");
    for(unsigned int i = 0; i < Transakcijos.size(); i++){
        r<<Transakcijos[i].getAmount()<<std::setprecision(8)<<"\n";
        r<<Transakcijos[i].getSenderKey()<<"\n";
        r<<Transakcijos[i].getReceiverKey()<<"\n";
        r<<"\n";
    }
    r.close();
}

void validateTransactions(std::vector<Transaction> & Transakcijos, std::vector<User> & Naudotojai, std::vector<Transaction> & validTransactions){
    double newAmount = 0;
    std::string testID = "", stringAmmount = "", a ="", b = "";
    for(unsigned int i = 0; i < Transakcijos.size(); i++){
        for(unsigned int j = 0; j < Naudotojai.size(); j++){
            if((Transakcijos[i].getSenderKey() == Naudotojai[j].getPublicKey()) && (Transakcijos[i].getAmount() <= Naudotojai[j].getBalance())){
                for(unsigned int k = 0; k < Naudotojai.size(); k++){
                    if(Transakcijos[i].getReceiverKey() == Naudotojai[k].getPublicKey()){
                        stringAmmount = std::to_string( Transakcijos[i].getAmount());
                        a = Naudotojai[j].getPublicKey();
                        b = Naudotojai[k].getPublicKey();
                        testID = a + b + stringAmmount;
                        if(createHash( testID ) == Transakcijos[i].getID()){
                            validTransactions.push_back(Transakcijos[i]);
                            newAmount = Naudotojai[j].getBalance() - Transakcijos[i].getAmount();
                            Naudotojai[j].setBalance( newAmount );
                            newAmount = Naudotojai[k].getBalance() + Transakcijos[i].getAmount();
                            Naudotojai[k].setBalance( newAmount );
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    std::ofstream r ("validTransactions.txt");
    for(unsigned int i = 0; i < validTransactions
    .size(); i++){
        r<<validTransactions[i].getAmount()<<std::setprecision(6)<<"\n";
        r<<validTransactions[i].getSenderKey()<<"\n";
        r<<validTransactions[i].getReceiverKey()<<"\n";
        r<<"\n";
    }
    r.close();
}

void minerAmount(std::vector<Transaction> & validTransactions, std::vector<Block> & mineriai){
    if( validTransactions.size() >= 500){
		Block a,b,c,d,e;
		mineriai.push_back(a);
		mineriai.push_back(b);
		mineriai.push_back(c);
		mineriai.push_back(d);
		mineriai.push_back(e);
	}
    if( validTransactions.size() < 500 && validTransactions.size() >= 400){
		Block a,b,c,d;
		mineriai.push_back(a);
		mineriai.push_back(b);
		mineriai.push_back(c);
		mineriai.push_back(d);
	}
    if( validTransactions.size() < 400 && validTransactions.size() >= 300){
		Block a,b,c;
		mineriai.push_back(a);
		mineriai.push_back(b);
		mineriai.push_back(c);
	}
    if( validTransactions.size() < 300 && validTransactions.size() >= 200){
		Block a,b;
		mineriai.push_back(a);
		mineriai.push_back(b);
	}
    if( validTransactions.size() < 200 && validTransactions.size() >= 100){
		Block a;
		mineriai.push_back(a);
	}
}

bool mine(Block & a){
    std::string b="", diffic = "";
    int swichCount = 0;
    for(int i = 0; i < a.diffTarget; i++){
        diffic = diffic + "0";
    }
    for(unsigned int i = 0; i < a.data_.size(); i++){
        for(unsigned int j = 0; j < a.data_.size(); i=j++){
            if(swichCount >= a.nonce){
                return false;
            }
            else{
                if( i != j){
                    std::swap(a.data_[i], a.data_[j]);
                    a.hashBlock();
                    b = a.getHash();
                    swichCount ++;
                    if(b.find( diffic, 0) == 0 && b[a.diffTarget] != '0'){
                    return true;
                    }
                }
            }
        }
    }
    return false;
}

void printChain( Blockchain & BlokoGrandine){
    std::ofstream r ("blockChain.txt");
    for(unsigned int i = 0; i < BlokoGrandine.getBlockChainSize(); i++){
        r<<BlokoGrandine.getBlockHash(i)<<"\n";
        r<<"\n";
    }
    r.close();
}