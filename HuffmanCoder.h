/*
*  HuffmanCoder.h
* Tassilo Armleder
* 2/03/24
*  Header file for the HuffmanCoder class that contains the encode and edocde functions as well as multiple helper methods
*/

#ifndef _HUFFMAN_CODER
#define _HUFFMAN_CODER

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <stdexcept>
#include "HuffmanTreeNode.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include "BinaryIO.h"
#include <stack>

class HuffmanCoder {
   public:
    void encoder(const string &inputFile, const string &outputFile);
    void decoder(const string &inputFile, const string &outputFile);

   private:
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, 
    NodeComparator> count_freqs(ifstream &text);
    string serialize_tree(HuffmanTreeNode *root);
    HuffmanTreeNode * deserialize_tree(const string &s);
    HuffmanTreeNode * construct_tree(priority_queue<HuffmanTreeNode*, 
    vector<HuffmanTreeNode*>, NodeComparator>);
    template<typename streamtype>
    void open_or_die(streamtype &stream, string file_name);
    struct Character {
        char value = '\0';
        string code = "";
    };
    void createCharacterCodes(HuffmanTreeNode* Tree,  vector<Character>& 
    characters,  string code);
    string textToBinary(ifstream &text,vector<Character> characters);
    string BinaryToText(HuffmanTreeNode * Tree, string text_binary);
    void textToFile(string text, ofstream &outputFile);

};

#endif