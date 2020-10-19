#pragma once

#include <vector>
#include <math.h>
using namespace std;

class Salsa {
public:
	Salsa() {
		keys.resize(8);
		nonces.resize(2);
		state.resize(16);
		pos.resize(2);
		pos = { 0, 0 };

	}
	void init(vector<unsigned char> key /* 32 bytes */,
		vector<unsigned char> nonce /* 8 bytes */) {
		keys[0] = to_word({ key.begin(), key.begin() + 4 });
		keys[1] = to_word({ key.begin() + 4, key.begin() + 8 });
		keys[2] = to_word({ key.begin() + 8, key.begin() + 12 });
		keys[3] = to_word({ key.begin() + 12, key.begin() + 16 });
		keys[4] = to_word({ key.begin() + 16, key.begin() + 20 });
		keys[5] = to_word({ key.begin() + 20, key.begin() + 24 });
		keys[6] = to_word({ key.begin() + 24, key.begin() + 28 });
		keys[7] = to_word({ key.begin() + 28, key.end() });

		nonces[0] = to_word({ nonce.begin(), nonce.begin() + 4 });
		nonces[1] = to_word({ nonce.begin() + 4, nonce.end() });
	}

	vector<unsigned char> encrypt(vector<unsigned char> data) {
		if (pos[0] == (unsigned int)(0xffffffff)) {
			++pos[1];
		}
		else {
			++pos[0];
		}
		state[0] = c[0];
		state[1] = keys[0];
		state[2] = keys[1];
		state[3] = keys[2];
		state[4] = keys[3];
		state[5] = c[1];
		state[6] = nonces[0];
		state[7] = nonces[1];
		state[8] = pos[0];
		state[9] = pos[1];
		state[10] = c[2];
		state[11] = keys[4];
		state[12] = keys[5];
		state[13] = keys[6];
		state[14] = keys[7];
		state[15] = c[3];

		for (int i = 0; i < 10; ++i) {
			column_round();
			row_round();
		}

		vector<unsigned char> encrypted;

		for (int i = 0; i < 16; ++i) {
			vector<unsigned char> ar = from_word(state[i]);
			for (int j = 0; j < 4; ++j) {
				encrypted.push_back(ar[j] ^ data[i*4 + j]);
			}
		}
		return encrypted;
	}
private:
	vector<unsigned int> keys;
	vector<unsigned int> nonces;
	vector<unsigned int> state;

	vector<unsigned int> c = { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574 };
	vector<unsigned int> pos;


	unsigned int to_word(vector<unsigned char> b) {
		return (unsigned int)(b[0] + (unsigned int)pow(2, 8) * b[1] + (unsigned int)pow(2, 16) * b[2] + (unsigned int)pow(2, 24) * b[3]);
	}

	vector<unsigned char> from_word(unsigned int w) {
		vector<unsigned char> ar(4);
		ar[0] = (unsigned char)(w / (unsigned int)pow(2, 24));
		ar[1] = (unsigned char)((w - ar[0] * (unsigned int)pow(2, 24)) / (unsigned int)pow(2, 16));
		ar[2] = (unsigned char)((w - ar[0] * (unsigned int)pow(2, 24) - ar[1] * (unsigned int)pow(2, 16)) / (unsigned int)pow(2, 8));
		ar[3] = (unsigned char)(w - ar[0] * (unsigned int)pow(2, 24) - ar[1] * (unsigned int)pow(2, 16) - ar[2] * (unsigned int)pow(2, 8));
		return ar;
	}

	void column_round() {
		quarter_round(0, 4, 8, 12);
		quarter_round(5, 9, 13, 1);
		quarter_round(10, 14, 2, 6);
		quarter_round(15, 3, 7, 11);
	}

	void row_round() {
		quarter_round(0, 1, 2, 3);
		quarter_round(5, 6, 7, 4);
		quarter_round(10, 11, 8, 9);
		quarter_round(15, 12, 13, 14);
	}

	unsigned int shift(unsigned int n, int pos) {
		return (unsigned int)((n << pos) | (n >> (32 - pos)));
	}

	void quarter_round(int a0, int a1, int a2, int a3) {
		state[a1] = state[a1] ^ shift((unsigned int)(state[a0] + state[a3]), 7);
		state[a2] = state[a2] ^ shift((unsigned int)(state[a1] + state[a0]), 9);
		state[a3] = state[a3] ^ shift((unsigned int)(state[a2] + state[a1]), 13);
		state[a0] = state[a0] ^ shift((unsigned int)(state[a3] + state[a2]), 18);
	}
};

