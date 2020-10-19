#ifndef RC4_H_INCLUDED
#define RC4_H_INCLUDED

#include <vector>
using namespace std;

class RC4 {
public:
    RC4() {}
    void init(vector<unsigned char> k) {
        key = k;
        for (int i = 0; i < 256; ++i) {
            s[i] = i;
        }
        for (int i = 0, j = 0; i < 256; ++i) {
            j = (j + key[i % key.size()] + s[i]) % 256;
            swap_s(i, j);
        }
        for (int i = 0; i < 10; ++i) {
            cout << (int)s[i] << " ";
        }
    }
    unsigned char next_K() {
        i_ = (i_ + 1) % 256;
        j_ = (j_ + s[i_]) % 256;
        swap_s(i_, j_);
        return s[(s[i_] + s[j_]) % 256];
    }
private:
    unsigned char s[256];
    vector<unsigned char> key;
    int i_ = 0;
    int j_ = 0;

    void swap_s(int i, int j) {
        int temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
};

#endif // RC4_H_INCLUDED
