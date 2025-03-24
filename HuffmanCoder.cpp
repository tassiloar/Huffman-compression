

#include "HuffmanCoder.h"

/*
encoder
Inputs: inputFile is the path to the input text file to be compressed. 
outputFile is the path where the compressed file will be saved.
Purpose: Compresses a text file using Huffman coding, writes the 
encoded data and the serialized tree to the output file, and prints
the success message with the size of the encoded text in bits.
Outputs: No return value, but outputs a message to stdout and writes 
to the outputFile. Prints a message if the inputFile is empty.
*/
void HuffmanCoder::encoder(const string &inputFile, const string &outputFile) {

    ifstream instream;
    ofstream outFilestream; 

    open_or_die(instream, inputFile);
    open_or_die(outFilestream, outputFile);

    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, NodeComparator> 
    priority_queue = count_freqs(instream);

    if (priority_queue.size() == 0 ){
        cout << "FILENAME is empty and cannot be compressed."<<endl;
        return;
    }

    HuffmanTreeNode* Tree = construct_tree(priority_queue);

    vector<Character> characters;

    createCharacterCodes(Tree, characters, "");

    ifstream instream2;
    open_or_die(instream2, inputFile);

    string encoded_text = textToBinary(instream2, characters);
    string serialized_tree = serialize_tree(Tree);

    BinaryIO writeBinary;

    writeBinary.writeFile(outputFile, serialized_tree, encoded_text);

    cout << "Success! Encoded given text using "<<encoded_text.length()
    <<" bits."<<endl;
    

}

/*
decoder
Inputs: inputFile is the path to the input file containing the encoded data and the serialized Huffman tree. outputFile is the path where the decoded text will be saved.
Purpose: Decompresses the encoded file, reconstructs the Huffman tree, decodes the text, and writes the decoded text to the outputFile.
Outputs: No return value, but writes the decoded text to the outputFile.
*/
void HuffmanCoder::decoder(const std::string &inputFile, 
const std::string &outputFile) {

    BinaryIO writeBinary;

    pair<std::string, std::string> result = writeBinary.readFile(inputFile);

    string encoded_text = result.second;
    string serialized_tree =  result.first;

    HuffmanTreeNode * Tree = deserialize_tree(serialized_tree);

    string result_text = BinaryToText(Tree, encoded_text);

    ofstream outFilestream; 

    open_or_die(outFilestream, outputFile);

    outFilestream << result_text;

}

/*
count_freqs
Inputs: text is an ifstream connected to the input file from which characters 
will be read and their frequencies counted.
Purpose: Counts the frequency of each character in the input file and creates 
a priority queue of HuffmanTreeNode pointers sorted by frequency.
Outputs: Returns a priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, 
NodeComparator> containing the frequencies of characters.
*/
priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, 
NodeComparator> HuffmanCoder::count_freqs(ifstream &text) {

    unordered_map<char, int> frequencies;

    char current;

    while (text.get(current)) {
        frequencies[current]++;
    }

    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, 
    NodeComparator> my_pq;

    for (const auto& pair : frequencies) {
        my_pq.push(new HuffmanTreeNode(pair.first, pair.second));
    }

    return my_pq;
    
}

/*
serialize_tree
Inputs: root is a pointer to the root node of the Huffman tree to be 
serialized.
Purpose: Serializes the Huffman tree into a string representation 
that encodes the structure and characters of the tree.
Outputs: Returns a string representing the serialized Huffman tree.
*/
string HuffmanCoder::serialize_tree(HuffmanTreeNode *root) {

    if (root->isLeaf()) {
        return "L"+string(1,root->get_val());
    }
    
    string left = serialize_tree(root->get_left());
    string right = serialize_tree(root->get_right());

    return "I"+left+right;

}

/*
deserialize_tree
Inputs: s is a string representation of a serialized Huffman tree.
Purpose: Reconstructs a Huffman tree from its serialized string 
representation.
Outputs: Returns a pointer to the root node of the deserialized 
Huffman tree
*/
HuffmanTreeNode * HuffmanCoder::deserialize_tree(const string &s) {

    stack<HuffmanTreeNode*> nodeStack;

    HuffmanTreeNode* first = nullptr;

    for (int i = 0; i < s.size(); i++) {
        
        bool parent = false;
        char ch = '\0';

        if (s.at(i) == 'I') {
            parent = true;
        } else if (s.at(i) == 'L') {
            ch = s.at(i+1);
            i++;
        }

        HuffmanTreeNode* temp = new HuffmanTreeNode(ch,0);


        if (nodeStack.empty()) {
            if (i == 0) {
                first = temp;
            }
        }

        else if (nodeStack.top()->get_left() == nullptr) {
            nodeStack.top()->set_left(temp);

        }else if (nodeStack.top()->get_right() == nullptr) {
            nodeStack.top()->set_right(temp);
            nodeStack.pop();
        }

        if(parent) {
        nodeStack.push(temp);
        }

    }

    return first;

}

/*
construct_tree
Inputs: mq is a priority queue of HuffmanTreeNode pointers sorted 
by frequency.
Purpose: Constructs the Huffman tree from the priority queue by 
combining the two nodes with the lowest frequencies into a new 
parent node until only one node remains.
Outputs: Returns a pointer to the root node of the constructed 
Huffman tree 
*/
HuffmanTreeNode * HuffmanCoder::construct_tree(priority_queue<HuffmanTreeNode*,
 vector<HuffmanTreeNode*>, NodeComparator> mq) {    


    while (mq.size() > 1) {


        HuffmanTreeNode * temp1 = mq.top();
        mq.pop();
        HuffmanTreeNode * temp2 = mq.top();
        mq.pop();

        HuffmanTreeNode * parent = new HuffmanTreeNode('\0',temp1->get_freq()
        +temp2->get_freq(),temp1,temp2);

        mq.push(parent);
        
        
    }

    return mq.top();

}

/*
open_or_die
Inputs: stream is a reference to an ifstream or ofstream. file_name 
is the path to the file to be opened.
Purpose: Attempts to open a file stream; if the file cannot be opened, 
it throws a runtime error.
Outputs: No return value, but the stream is connected to the file if 
successful.
*/
template<typename streamtype>
void HuffmanCoder::open_or_die(streamtype &stream, string file_name)
{
    stream.open(file_name);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + file_name);
    }
}

/*
createCharacterCodes
Inputs: current is a pointer to the current node in the Huffman tree. 
characters is a reference to a vector that will be populated with 
characters and their Huffman codes. code is the current Huffman 
code being constructed during tree traversal.
Purpose: Recursively generates Huffman codes for each character 
in the Huffman tree and stores them in a vector.
Outputs: No return value, but populates the characters vector 
with the codes.
*/
void HuffmanCoder::createCharacterCodes(HuffmanTreeNode* current,
vector<Character>& characters, string code) {

    if (current->isLeaf()) {
        Character temp;
        temp.value = current->get_val();
        temp.code = code;

        characters.push_back(temp);
        return;
    }

    createCharacterCodes(current->get_left(), characters, code+"0");
    createCharacterCodes(current->get_right(), characters, code+"1");


}

/*
textToBinary
Inputs: text is an ifstream connected to the readable file containing 
text to encode. characters is a vector of Character objects 
representing characters and their Huffman codes.
Purpose: Encodes the text using the provided Huffman codes into 
a binary string.
Outputs: Returns a string containing the binary representation 
of the encoded text.
*/
string HuffmanCoder::textToBinary(ifstream &text,vector<Character> 
characters) {

    char current;
    string encoded = "";

    while (text.get(current)) {

        for(int i = 0; i < characters.size(); i++) {
            if (characters.at(i).value==current) {
                encoded+=characters.at(i).code;
            }
        }
    }

    return encoded;

}

/*
BinaryToText
Inputs: Tree is a pointer to the root of the Huffman tree used for 
decoding. encoded_text is a binary string representing the encoded text.
Purpose: Decodes the binary string into ASCII text using the Huffman tree.
Outputs: Returns a string containing the decoded ASCII text.
*/
string HuffmanCoder::BinaryToText(HuffmanTreeNode * Tree, 
string encoded_text) {

    HuffmanTreeNode * current = Tree;

    string decoded_text = "";

    for(int i = 0; i < encoded_text.size(); i++) {

        if( encoded_text[i] == '0') {
            current = current->get_left();

        } else if  (encoded_text[i] == '1') {
            current = current->get_right();
        }

        if (current->isLeaf()) {
            decoded_text+=current->get_val();
            current = Tree;
        }
        
    }

    return decoded_text;



}

