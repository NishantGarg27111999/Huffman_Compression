#include <iostream>
#include <string.h>
// #include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

unordered_map<char, string> hashmap;

class node
{
public:
    int freq;
    node *left;
    node *right;
    char ch;

    node(int freq, char ch, node *left, node *right)
    {
        this->freq = freq;
        this->ch = ch;
        this->left = left;
        this->right = right;
    }
    int getfreq()
    {
        return freq;
    }
    char getch()
    {
        return ch;
    }
};

class mycomparator
{
public:
    int operator()(node *n1, node *n2)
    {
        return n1->getfreq() > n2->getfreq();
    }
};

void huffman_tree(priority_queue<node, vector<node *>, mycomparator> &pq)
{

    while (pq.size() > 1)
    {
        // cout<<"mein huffman_tree   "<<pq.size()<<endl;
        node *left_child = pq.top();
        pq.pop();
        node *right_child = pq.top();

        pq.pop();

        node *temp = (node *)malloc(sizeof(node));
        temp->freq = left_child->freq + right_child->freq;
        temp->ch = '\0';
        temp->left = left_child;
        temp->right = right_child;
        pq.push(temp);
    }
}

void assign_code(node *root, string str)
{
    // cout<<"mein assign code "<<root->ch<<" "<<root->freq<<endl;

    string s1 = "";
    string s2 = "";
    if (!root->left && !root->right)
    {
        // cout<<"mein last"<<endl;
        hashmap[root->ch] = str;
    }
    else
    {

        s1 = str + "0";
        s2 = str + "1";
        assign_code(root->left, s1);
        assign_code(root->right, s2);
    }
}

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        cout << "Error in opening file\n";
        return 2;
    }
    string in = argv[1];
    string filetype = in.substr(in.find(".") + 1);
    cout << filetype << endl;
    string out = in.substr(0, in.find(".")) + "-compressed.bin";

    FILE *output = fopen(out.c_str(), "wb");

    if (output == NULL)
    {
        cout << "Error in creating output file\n";
        return 3;
    }

    string file_contents = "";
    char buff[1];
    int num_of_unique_character = 0;

    // write filetype to output file

    int ftsize = filetype.length();
    char num = ftsize + '0';
    fwrite(&num, 1, 1, output);

    for (int i = 0; i < ftsize; i++)
    {
        buff[0] = filetype[i];
        fwrite(buff, 1, 1, output);
    }

    // hold frequency of unique characters

    unordered_map<char, int> mp;

    while (fread(buff, 1, 1, input))
    {
        if (mp.find(buff[0]) == mp.end())
        {
            mp[buff[0]] = 1;
        }
        else
        {
            mp[buff[0]]++;
        }
    }
    fclose(input);
    FILE *input1 = fopen(argv[1], "r");

    priority_queue<node, vector<node *>, mycomparator> pq;

    for (auto p : mp)
    {
        node *temp = (node *)malloc(sizeof(node));
        temp->freq = p.second;
        temp->ch = p.first;
        temp->left = NULL;
        temp->right = NULL;
        pq.push(temp);
    }
    // cout<<"mein chala\n";
    huffman_tree(pq);
    assign_code(pq.top(), "");
    // cout<<"mein chala -----------\n";
    for (auto it : hashmap)
    {
        cout << it.first << "-----" << it.second << endl;
    }

    // converting file content to binary code by assigning the huffman code to each character
    while (fread(buff, 1, 1, input1))
    {
        file_contents += hashmap[buff[0]];
    }
    // cout<<file_contents<<endl;
    int padding = 0;
    if (file_contents.length() % 8 != 0)
    {
        padding = 8 - (file_contents.length() % 8);
    }

    for (int i = 0; i < padding; i++)
    {
        file_contents = "0" + file_contents;
    }
    string codes;
    for (auto iter : hashmap)
    {
        codes = "";
        codes += iter.first + iter.second;
        fwrite(codes.c_str(), codes.length() + 1, 1, output);
    }

    // write the padding given to binary code
    char null = '\0';
    char pad = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);

    //to identify the padding with 2 null characters both sides


    //take 8bit binary code->decimal->character
    //and write this character to output file
    char temp_bin[9];
    int dec,w;
    int i;
    for(int j=0;j<file_contents.length();j=j+8)            
    {
        strncpy(temp_bin,&file_contents[j],8);
        //convert to integer equivalent and then to character
        dec=0;
         i=7;
        while(i>-1)
        {
            w=temp_bin[i]=='1'?1:0;
            dec+=((int)pow(2,7-i))*w;
            i--;
        }

        //write the ascii character corresponding to decimal to output

        buff[0]=char(dec);
        fwrite(buff,1,1,output);
    }

    cout<<"File compressed successfully\n";
    
    fclose(input1);
    fclose(output);
    return 0;

}