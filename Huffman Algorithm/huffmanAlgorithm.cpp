#include<iostream>
#include<unordered_map>
#include<fstream>
#include<cmath>

using namespace std;

//A Huffman tree node i.e leaf Node
struct Node{
    char ch;   //one of the input character
    int freq;   //Frequency of the character
    string huff_code;
    Node *left, *right;   //left and right child of this node
    Node(char c, int f){
        ch=c;
        freq=f;
        huff_code="";
        left=right=NULL;
    }
};

unordered_map<char, pair<int, string>> mp;

//creating heap of leaf nodes
class MinHeap{
 
    // Current size of min heap
    int size;
 
    // capacity of min heap
    int capacity;
 
    // Array of minheap node pointers
    Node* *array;

public:
    MinHeap(int c){
        size=-1;
        capacity=c;                     //This the size of the mapwhre all character are there.
        array = new Node*[capacity];   //allocated memory for the array
    }

    // A utility function to insert
    // a new node to Min Heap
    void insertMinHeap(Node *minHeapNode)
    {
        ++size;
        array[size]=minHeapNode;
        int i=size;
        //frequency of parent should be greater
        //than frequency of child node
        while (i && minHeapNode->freq < array[(i - 1) / 2]->freq) {  //cheaking the frequency of the node with
            array[i] = array[(i - 1) / 2];                           //the parent node frequency
            i = (i - 1) / 2;
        }
        //for the i the frequency of parent is greater or i is the root.
        array[i] = minHeapNode;
 
    }
    //Time complexity : log n

    // The standard minHeapify function.
    //If the parent is greater than left and right child or by any one
    //Then the heap should be minheapified
    //so we will compare the smallest value of them and swap it to the parent.
    void minHeapify(int idx)
    {
        int smallest = idx;
        int left = 2 * idx + 1;   //left child of index
        int right = 2 * idx + 2;  //right child of index
 
        if (left <= size && array[left]->freq < array[smallest]->freq)
            smallest = left;
 
        if (right <= size && array[right]->freq < array[smallest]->freq)
           smallest = right;
 
        if (smallest != idx) {
            swap(array[smallest],array[idx]);
            minHeapify(smallest);
        }
    }
    //Time complexity : log n

    // A utility function to check
    // if size of heap is 1 or not
    bool isSizeOne(){
        return (size == 0);
    }
    // A standard function to extract
    // minimum value node from heap
    Node* extractMin()
    {
        Node *node=array[0];
        swap(array[0],array[size]);
        --size;
        minHeapify(0);
        return node;
    }
    //time complexity : log n


    //this function will be giving the index of the parent of node at index i
    int parent(int i){
        return (i-1)/2;
    }

    Node* buildHuffmanTree()
    {
        Node *left, *right, *top;
        // Iterate while size of heap doesn't become 1
        while (!isSizeOne()) {
 
        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin();
        right = extractMin();
        
        // Step 3: Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = new Node('$', left->freq + right->freq);
 
        top->left = left;
        top->right = right;
 
        insertMinHeap(top);
        }
        //By doing this we will be getting the nore frequent character on the top of the tree and the 
        //less frequent characters on the bottom of the tree.
 
        // Step 4: The remaining node is the
        // root node and the tree is complete.
        return array[0];
    }
};
//This is the end of the min heap class.
//We will get the tree from the class.

void insertCodes(Node* root, string str)
{
    // Assign 0 to left edge and recur
    if(!root){
        return;
    }
    if(!root->left && !root->right){
        if(root->ch!='$')
        mp.insert({root->ch, {root->freq, str}});
    }
    insertCodes(root->left, str+"0");
    insertCodes(root->right, str+"1");
}

string Padding(string str, int &ex){
    int len=str.length();
    int Padd=len%8;
    if(Padd){
        Padd=8-Padd;
    }
    for(int i=0; i<Padd; i++){
        ex++;
        str.push_back('0');
    }
    return str;
}

int getDecimal(long int binary){  
    int decimal = 0;  
    int n = 0;  
    while(true){  
      if(binary == 0){  
        break;  
      } else {  
          int temp = binary%10;  
          decimal += temp*(pow(2, n));  
          binary = binary/10;  
          n++;  
       }  
    }  
    return decimal;  
}  

int ex=0;    //extra padded length
string str;  //string will be in form of binary string
void encode(){
    ifstream fin;
    fin.open("input.txt");
    while(!fin.eof()){
        char ch;
        fin>>ch;
        string Code=mp[ch].second;
        str.append(Code);
    }
    fin.close();
    str=Padding(str, ex);  //we are generating the codes corresponding to the binary substring of length 8.
    ofstream fout;
    fout.open("Encode.txt");
    for(int i=0; i<str.length(); i+=8){
        string s=str.substr(i, 8);
        long int var=stoi(s);
        int num=getDecimal(var);
        fout<<(char)num;
    }
    fout.close();
}

void decode(Node *root){
    for(int i=0; i<ex; i++){
        str.pop_back();
    }
    string ans = "";
    struct Node* curr = root;
    for(int i=0;i<str.size();i++)
    {
        if (str[i] == '0')
           curr = curr->left;
        else
           curr = curr->right;
  
        // reached leaf node
        if (curr->left==NULL and curr->right==NULL)
        {
            ans += curr->ch;
            curr = root;
        }
    }
    ofstream fout;
    fout.open("decode.txt");
    for(int i=0; i<ans.length(); i++){
        fout<<ans[i];
    }
}

void display(unordered_map<char, pair<int, string>> m)
{
    cout<<"\t       Huffman Algorithm\n";
    cout<<"\t-------------------------------";
    cout <<"\n\n\tSymbol\t   Frequency\tCode";
    cout<<"\n\t------     ---------    -----";
    for (auto itr = m.begin(); itr != m.end(); itr++) {
        cout << "\n\t" << itr->first<< "\t\t" << itr->second.first << "\t"<<itr->second.second<<endl;
    }
}

void chooseEncodeDecode(Node *root, bool &encoded, bool &decoded){
    if(encoded){
        cout<<"\nYour File is encoded Successfully"<<endl;
        cout<<"-----------------------------------------"<<endl;
    }
    else{
        cout<<"\nPlease encode the file first"<<endl;
    }
    cout<<"\n\npress 1 to encode "<<endl;
    cout<<"\n\npress 2 to decode "<<endl;
    cout<<"\n\npress 0 to exit"<<endl;
    int choice;
    cout<<"\n\nEnter Choice : ";
    cin>>choice;
    cout<<"-----------------------------------------"<<endl;
    switch(choice){
        case 1:
        encode();
        encoded=true;
        break;
        case 2:
        if(encoded){
        decode(root);
        decoded=true;
        cout<<"Your File is decoded Successfully"<<endl;
        }
        break;
        case 0:
        exit(0);
    }
}

int main(){
    ifstream fin;
    fin.open("input.txt");
    //for calculating the frequency of every character.
    system("color B0");
    unordered_map<char, int> _map;
    while(!fin.eof()){
        char ch;
        fin>>ch;
        _map[ch]++;
    }
    fin.close();
    MinHeap obj(_map.size()); 
    for(auto itr=_map.begin(); itr!=_map.end(); itr++){
        Node *node = new Node(itr->first, itr->second);
        obj.insertMinHeap(node);
    }
    Node *root=obj.buildHuffmanTree();
    insertCodes(root, "");
    display(mp);
    //The codes generated are the prefix codes that means the code generated for a character
    //cannot be the prefix for another character.
    
    bool encoded=false;
    bool decoded=false;
    cout<<"--------------------------------------------------------";
    chooseEncodeDecode(root, encoded, decoded);
    cout<<encoded<<decoded<<endl;
    while(!encoded || !decoded){
        system("CLS");
        chooseEncodeDecode(root, encoded, decoded);
    }
}


//Time complexity : O(nlogn)  , here n is distinct charcters in file
//beacuse we calling the the extract min function 2*(n-1) times
//extract min takes logn time