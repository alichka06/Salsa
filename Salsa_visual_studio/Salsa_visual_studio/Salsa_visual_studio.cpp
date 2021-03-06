#include "pch.h"
#include <fstream>
#include "Header.h"
using namespace std;

int main()
{
	vector<unsigned char> nonce = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

	vector<unsigned char> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
								  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
								  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
								  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};


	Salsa salsa;
	salsa.init(key, nonce);

	char data[64];
	vector<unsigned char> encrypted_data;

	ifstream in;
	in.open("input.txt", ios::binary | ios::in);

	while (in) {
		in.read(data, 64);

		vector<unsigned char> encrypted = salsa.encrypt({ data, data + sizeof(data) / sizeof(char) });
		encrypted_data.insert(encrypted_data.end(),
			encrypted.begin(),
			encrypted.end());
	}
	in.close();

	Salsa salsa_decrypt;
	salsa_decrypt.init(key, nonce);

	ofstream out;
	out.open("out.txt", ios::binary | ios::out);

	for (int i = 0; i < encrypted_data.size(); i += 64) {
		vector<unsigned char> encrypted = salsa_decrypt.encrypt({ encrypted_data.begin() + i,
																encrypted_data.begin() + i + 64 });
		for (int j = 0; j < 64; ++j) {
			out << encrypted[j];
		}
	}
	out.close();
}