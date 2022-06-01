// C++ program for Shannon Fano Algorithm

// include header files
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

unordered_map<char, string> _codes;

// declare structure node
struct node{
    char c;
    int freq;
    string code;
};

void shannon(int l, int h, node p[]);
void storeCodes(int n,node p[]);
string Padding(string str);
int getDecimal(long int binary);
void encode();
void display(int n, node p[]);

// function to find shannon code
void shannon(int l, int h, node p[])
{
    if((l + 1) == h || l == h || l > h){
    	if (l == h || l > h)
            return;
        p[h].code.push_back('1');
        p[l].code.push_back('0');
        return;  
    }
    else{
    	int mid=(l+h)/2;
        for(int i=l; i<mid; i++){
        	p[i].code.push_back('0');
        }
        for(int i=mid; i<=h; i++){
        	p[i].code.push_back('1');
        }
        // Invoke shannon function
        shannon(l, mid-1, p);
        shannon(mid, h, p);
    }
}

//Storing the coding in the map.
void storeCodes(int n,node p[]){
	for(int i=0; i<n; i++){
		_codes.insert({p[i].c, p[i].code});
	}
}

//This function will help in Padding which
//will help for the pupose of encoding.

string Padding(string str){
	int len=str.length();
	int Padd=len%8;
	if(Padd){
		Padd=8-Padd;
	}
	for(int i=0; i<Padd; i++){
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

void encode(){
	ifstream fin;
	fin.open("input.txt");
	string str;
	while(!fin.eof()){
		char ch;
		fin>>ch;
        string Code=_codes[ch];
		str.append(Code);
	}
	fin.close();
	str=Padding(str);                                  //char 1 byte 8 byte
	ofstream fout;                //0100100100101     //int 4 byte   //
	fout.open("Encode.txt");
	for(int i=0; i<str.length(); i+=8){
		string s=str.substr(i, 8);                     //long int 01001001
		long int var=stoi(s);
		int num=getDecimal(var);
		fout<<(char)num;
	}
	fout.close();
}

// Function to sort the symbols
// based on their frequency
void sortByfrequency(int n, node p[])
{
	node temp;
    //Bubble Sort : Descending Order           
	for (int i = 0; i < n-1; i++) {
		for (int j = 0; j < n-i-1; j++) {
			if ((p[j].freq) < (p[j + 1].freq)) {
                //Custom Swap Function
				temp.freq = p[j].freq;
				temp.c = p[j].c;

				p[j].freq = p[j + 1].freq;
				p[j].c = p[j + 1].c;

				p[j + 1].freq = temp.freq;
				p[j + 1].c = temp.c;
			}
		}
	}
}

// function to display shannon codes
void display(int n, node p[])
{
	cout<<"\t\t Shanon Fano\n";
	cout<<"\t-------------------------------";
	cout <<"\n\n\tSymbol\t   Frequency\tCode";
	cout<<"\n\t------     ---------    -----";
	for (int i = 0; i < n; i++) {
		cout << "\n\t" << p[i].c<< "\t\t" << p[i].freq << "\t";
		for(int j=0; j<p[i].code.length(); j++){
			cout<<(p[i].code)[j];
		}
	}
}

// Driver code
int main()
{
	system("color B0");
    ifstream fin;
    fin.open("input.txt");
	//for calculating the frequency of every character.
	unordered_map<char, int> _map;
    while(!fin.eof()){
    	char ch;
    	fin>>ch;
    	_map[ch]++;
    }
    fin.close();
	// for(int i=0; i<s.length(); i++){
	// 	_map[s[i]]++;
	// }

	node p[_map.size()];  
    int j=0;
	for(auto itr=_map.begin(); itr!=_map.end(); itr++){
		p[j].c=itr->first;
		p[j].freq=itr->second;
		j++;
	}

	// Sorting the symbols based on
	// their frequency
	sortByfrequency(j, p);
	// Find the shannon code
	shannon(0, j-1, p);

	// Display the codes
	display(j, p);

	//Store the codes in the map
    storeCodes(j, p);

    //Convert the txt file in the encoded file.
	encode();
	cout<<"\nThe file is encoded Succcessfully";

	return 0;
}
