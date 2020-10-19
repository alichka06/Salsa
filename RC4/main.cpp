#include <iostream>
#include <vector>
#include <fstream>
#include "rc4.h"
using namespace std;

int main()
{
    vector<unsigned char> key {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    RC4 rc4_encrypt;
    rc4_encrypt.init(key);

    vector<unsigned char> encrypted;

    ifstream in;
    in.open("input.txt", ios::binary | ios::in);

    while (in) {
        char x, y;
        in.read(&x, 1);
        y = x ^ rc4_encrypt.next_K();
        encrypted.push_back(y);
    }
    in.close();

    RC4 rc4_decrypt;
    rc4_decrypt.init(key);

    ofstream out;
    out.open("out.txt", ios::binary | ios::out);

    for(vector<unsigned char>::iterator it1 = encrypted.begin(); it1+1 != encrypted.end();){
        unsigned char y;
        y = *it1 ^ rc4_decrypt.next_K();
        out << y;
        it1++;
    }
    out.close();

    return 0;
}
