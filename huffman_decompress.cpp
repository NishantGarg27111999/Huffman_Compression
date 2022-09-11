#include<iostream>
#include<bits/stdc++.h>
using namespace std;



string dectobin(int decimal){
    string s="";
    for(int i = 7; i>=0; i--){
        if(decimal>>i & 1)s += '1';
        else s += '0';
    }
    return s;
}

int main(int argc,char *argv[])
{

    FILE *input=fopen(argv[1],"rb");
    int num_of_unique_char=0;
    if(input==NULL)
    {
        cout<<"Error in opening input file\n";
        return 2;
    }
    string in=argv[1];
    string out=in.substr(0,in.find("-compressed"))+"-decompressed.";

    char buff[1];
    //Read file extension length and file extension itself

    fread(buff,1,1,input);
    int fsize=buff[0]-'0';
    while(fsize--)
    {
        fread(buff,1,1,input);
        out+=buff[0];
    }

    FILE *output=fopen(out.c_str(),"w");

    if(output==NULL)
    {
        cout<<"Error in creating output file\n";
        return 3;
    }

    unordered_map<string,char> decode_map;
    string s="";
    int flag=0;

    //Read and fill hashmap to decode later

    while(fread(buff,1,1,input))
    {
        if(buff[0]!='\0')
        {
            flag=0;
            s+=buff[0];
        }
        else{
            num_of_unique_char++;
            flag++;
            if(flag==2)
            {
                break;
            }
            decode_map[s.substr(1,s.length()-1)]=s[0];
            s="";
        }
    }

    //read and store padding and read extra null character
    fread(buff,1,1,input);
    int padding =buff[0]-'0';
    fread(buff,1,1,input);
    fread(buff,1,1,input);


    //Actual binary code starts form here

    int decimal;
    s="";
    while(fread(buff,1,1,input))
    {
        //get decimal euivalent of character
        decimal=buff[0];
        //append the binary equivalent to s
        s=s+dectobin(decimal);
    }
    //Ignore the padding and convert the binary code back to original text

    int start=0;
    if(padding==0)
    {
        s="0"+s;
        start=1;
    }
    else{
        start=padding;
    }

    for(int i=1;s[start]!='\0';i++)
    {
        if(decode_map.find(s.substr(start,i))!=decode_map.end())
        {
            fwrite(&(decode_map[s.substr(start,i)]),1,1,output);
            start=start+i;
            i=0;
        }
    }

    cout<<"Decompressed file successfully\n";
    fclose(input);
    fclose(output);
    return 0;




    
}