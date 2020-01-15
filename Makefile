CXXFLAGS=  -Wall -g

all: main.o hash.o generate.o
	g++  main.o hash.o generate.o -o main

main.o: main.cpp

hash.o: hash.cpp

generate.o: generate.cpp

test:	all
	./main

clean:
	rm -f *.o main users.txt transactions.txt validTransactions.txt blockChain.txt