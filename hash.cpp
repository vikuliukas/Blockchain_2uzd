#include "chain.h"

void makelongerifneeded( hash &current ){
    if(current.getinput().length() < 60){
        std::string filler (60, '!');
        std::string copy = current.getinput();
        filler.replace(0,copy.length(),copy);
        current.setfixedinput(filler);
    }
    else{
        std::string copy = current.getinput();
        current.setfixedinput(copy);
    }
}

void hashalgorithm( hash&current ){
    std::string copy = current.getfixedinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    unsigned long long int sum[5] = {1};
    int speperating = 1;
    int i = 1;
    for(char32_t &character : input32) {
        if(speperating == 1){
            sum[0] = ( i + i ) * sum[0] + character;
        }
        if(speperating == 2){
            sum[1] = ( i + i ) * sum[1] + character;
        }
        if(speperating == 3){
            sum[2] = ( i + i ) * sum[2] + character;
        }
        if(speperating == 4){
            sum[3] = ( i + i ) * sum[3] + character;
            speperating = 0;
        }
        speperating ++;
        i++;
    }

    std::string hex [5] = {""};
    for(int i = 0; i < 4; i++){
        hex[i] = inttohex(sum[i]);
        while(hex[i].length()<16){
            hex[i] += "f";
        }
    }
    std::string unshuffledhash = hex[0] + hex[1] + hex[2] + hex[3];
    std::string shuffledhash = hashshuffle( unshuffledhash, current );

    std::vector<char> symbols(shuffledhash.begin(), shuffledhash.end());
    int numberOf0 = 0;

    for(unsigned int i = 0; i < symbols.size(); i++){
        if( symbols[i] == '0'){
            std::swap(symbols[i], symbols[numberOf0]);
            numberOf0 ++;
        }
    }

    std::string shuffledhash2(symbols.begin(), symbols.end());

    current.setoutput(shuffledhash2);
}

std::string inttohex( unsigned long long int &i ){
    std::stringstream stream;
    stream << std::hex << i;
    std::string result( stream.str() );
    return result;
}

std::string hashshuffle( std::string &unshuffledhash, hash &current ){

    std::string copy = current.getinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    int i = 1;
    int sum = 0;
    for(char32_t &character : input32) {
        sum = sum + character * i;
        i++;
    }
    unsigned seed = sum;
    std::shuffle(unshuffledhash.begin(), unshuffledhash.end(),std::default_random_engine(seed));
    return unshuffledhash;
}

std::string createHash( std::string &word){
    hash current;
    current.setinput( word );
    makelongerifneeded( current );
    hashalgorithm( current );
    return current.getoutput();
}

std::string generateMerkelRoot( std::vector<Transaction> data){
    std::vector<std::string> merkel = {};
    std::vector<std::string> merkel2 = {};
    std::string word, hashed;
    for(unsigned int i = 0; i < data.size(); i++){
        merkel.push_back( data[i].getID() );
    }
    while( merkel.size() > 1){
        if( merkel.size() % 2 == 0){
            for( unsigned i = 0; i < merkel.size(); i=i+2){
                word = merkel[i] + merkel[i+1];
                hashed = createHash(word);
                merkel2.push_back(hashed);
            }
        }
        else{
            for( unsigned i = 0; i < merkel.size() - 1; i=i+2){
                word = merkel[i] + merkel[i+1];
                hashed = createHash(word);
                merkel2.push_back(hashed);
            } 
            word = merkel[merkel.size()-1];
            hashed = createHash(word);
            merkel2.push_back(hashed);
        }
        merkel.clear();
        merkel = merkel2;
        merkel2.clear();
    }
    return merkel[0];
}

