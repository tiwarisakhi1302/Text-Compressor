#include<iostream>
#include<vector>
#include<unordered_map>
#include<fstream>
using namespace std; 
vector<int>generating_codes(string s1); 
vector<char>encode(vector<int>V);
vector<int> code;
void decoding(vector<int>V) ;

vector<int>generating_codes(string s1) 
{ 
    cout << "Encoding"<<endl; 
    unordered_map<string, int> table; 
    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        table[ch] = i; 
    } 
  
    string first_char = "", next_char = ""; 
    first_char += s1[0]; 
    int code = 256; 
    vector<int> out_c; 
    cout << "String\toutput-code\tAddition"<<endl; 
    for (int i = 0; i < s1.length(); i++) { 
        if (i != s1.length() - 1) 
            next_char += s1[i + 1]; 
        if (table.find(first_char + next_char) != table.end()) { 
            first_char = first_char + next_char; 
        } 
        else { 
            cout << first_char << "\t\t" << table[first_char] << "\t\t" 
                 << first_char + next_char << "\t\t" << code << endl; 
            out_c.push_back(table[first_char]); 
            table[first_char + next_char] = code; 
            code++; 
            first_char = next_char; 
        } 
        next_char = ""; 
    } 
    cout << first_char << "\t\t" << table[first_char] << endl; 
    out_c.push_back(table[first_char]); 
    return (out_c); 
} 

vector<char>encode(vector<int>V)
{
    ofstream fout;
    fout.open("Encode.txt");
    for(int i=0; i<V.size(); i++)
    {
        int num=V[i];
        fout<<(char)num;
    }
    fout.close();
}
void decoding() 
{ 
    system("color B0");
    vector<int> mp;
    ifstream fin;
    fin.open("codes.txt");
    ofstream fout;
    fout.open("output.txt");
    cout << endl;
    cout << "Decoding.......\n"; 
    cout << endl;
    while (!fin.eof())
    {
        int i;
        fin>>i;
        mp.push_back(i);
    }
    unordered_map<int, string> table; 
    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        table[i] = ch; 
    } 
    int input_val = mp[0];
    int n; 
    string current_str = table[input_val]; 
    string c = ""; 
    c += current_str[0]; 
    fout<< current_str; 
    int count = 256; 
    for (int i = 0; i < mp.size() - 1; i++) { 
        n = mp[i + 1]; 
        if (table.find(n) == table.end()) { 
            current_str = table[input_val]; 
            current_str = current_str + c; 
        } 
        else { 
            current_str = table[n]; 
        } 
        fout<<current_str;
        c = ""; 
        c += current_str[0]; 
        table[count] = table[input_val] + c; 
        count++; 
        input_val = n; 
    } 
     fout.close();
} 
int main() 
{   
    vector<int> out_c ;
    int c;
    cout<<"Enter 1 for Compressing file"<<endl<<"Enter 2 for Decompressing file"<<endl<<"Enter 3 for exit"<<endl;
    cin >> c;
    switch(c)
    {
        case 1:
        {
            ifstream fin;
            fin.open("input.txt");
            ofstream fout;
            fout.open("codes.txt");
            string str="";
            while (!fin.eof())
            {
                string in_str;
                getline(fin, in_str);
                char ch=13;
                str=str+in_str+ch;
            }
            out_c = generating_codes(str);  
            cout << endl;
            for(auto itr=out_c.begin(); itr!=out_c.end();itr++)
            {
                code.push_back(*itr);
                fout<<*itr<<" ";
            }
            encode(out_c);
            fin.close();
            fout.close();
            main();
            break;
        }
        case 2:
        {
            ifstream fin;
            fin.open("Encode.txt");
            vector<char> encoded_codes;
            while (!fin.eof())
            {
                char x;
                fin>>x;
                encoded_codes.push_back(x);
            }
            decoding();
            main();
            break;
        }
        case 3:
        {
            exit(0);
            break;
        }
    }
    return(0);    
}