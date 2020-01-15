#include "chain.h"

int main(){

	std::vector<User> Naudotojai;
	generateUsers(Naudotojai);
	
	std::vector<Transaction> Transakcijos;
	generateTransactions(Transakcijos, Naudotojai);

	std::vector<Transaction> validTransactions;
	validateTransactions(Transakcijos, Naudotojai, validTransactions);


	std::cout << "Paspauskite 1 - pradėti grandinę, 0 - viską sustabdyti \n";
	std::string ats;
	std::cin >>  ats;
	while (ats != "0" && ats != "1"){
		std::cin.clear();
		std::cin.ignore();
		std::cout << "1 - pradėti grandinei, 0 - viską nutraukti \n";
		std::cin >> ats;
	}
	if( ats == "1"){
		Blockchain BlokuGrandine;
		while(ats == "1"){
			if(validTransactions.size() < 100){
				std::cout << "Neužtenka transakcijų naujiems blokams! \n";
				printChain(BlokuGrandine);
				return 0;
			}
			std::vector<Block> mineriai = {};
			minerAmount( validTransactions, mineriai);

			for(unsigned int i = 0; i < mineriai.size() * 100; i = i + mineriai.size()){
				for(unsigned int j = 0; j < mineriai.size(); j++){
					mineriai[j].data_.push_back(validTransactions[i+j]);
				}
			}

			std::cout <<"Kiek nuliu turi būti hash pradžioje? \n";
			int dif;
			std::cin >> dif;
			std::cout <<"Kiek kartu galima apkeisti transakcijas bloko viduje? \n";
			int no;
			std::cin >> no;

			for(unsigned int i = 0; i < mineriai.size(); i++){
				mineriai[i].diffTarget = dif;
				mineriai[i].nonce = no;
				if( mine(mineriai[i]) == true){
					time_t time = std::time(nullptr);
					mineriai[i].setTimeStamp(time);
					BlokuGrandine.addBlock(mineriai[i]);
					std::cout<<"sugeneruotas blokas minerio "<< i << "\n";
					for( unsigned int j = 0; j < mineriai[i].data_.size(); j++){
						for( unsigned int k = 0; k < validTransactions.size(); k++){
							if( mineriai[i].data_[j].getID() == validTransactions[k].getID()){
								validTransactions.erase(validTransactions.begin() + k);
								break;
							}
						}
					}
					break;
				}
				else{
					std::cout << "mineriui " <<i<<" nepavyko sugeneruoti bloko \n";
				}
			}
			std::cout<<"Ar norite testi? 1 - taip, 0 - ne. \n";
			std::cin>>ats;
			while (ats != "0" && ats != "1"){
				std::cin.clear();
				std::cin.ignore();
				std::cout << "1 - testi grandinei, 0 - viską nutraukti \n";
				std::cin >> ats;
			}
		}	
		printChain(BlokuGrandine);
	}
	
	return 0;
}