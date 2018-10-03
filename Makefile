all:
	g++ -O3 -std=c++11 -pg src/mainFile.cpp -o out1
main1.o: src/main1.cpp src/Board.h
	g++ -std=c++11 -c src/main1.cpp
Board.o: src/Board.cpp src/Board.h
	g++ -c src/Board.cpp
test:
	./out1
clean:
	rm out*
	rm *.out
	rm *.o
