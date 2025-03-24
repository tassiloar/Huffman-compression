###
### Makefile for Zap Project
###
### Author:  Tassilo Armleder

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 


zap: main.o HuffmanCoder.o HuffmanTreeNode.o BinaryIO.o
	$(CXX) $(LDFLAGS) -o zap $^

main.o: main.cpp HuffmanCoder.h
	$(CXX) $(CXXFLAGS) -c main.cpp 

HuffmanCoder.o: HuffmanCoder.cpp HuffmanCoder.h HuffmanTreeNode.h BinaryIO.h
	$(CXX) $(CXXFLAGS) -c HuffmanCoder.cpp

clean:
	@find . -type f \( \
		-name '*.o' ! -name 'HuffmanTreeNode.o' ! -name 'BinaryIO.o' \
		! -name 'ZapUtil.o' \) -exec rm -f {} \;
	@rm -f *~ a.out

