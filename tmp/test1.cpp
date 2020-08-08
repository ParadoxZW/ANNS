#include <bits/stdc++.h>
using namespace std;

class A{
    public:
        vector<int> &v;
        A(vector<int> &v):v(v) {}
        int operator[] (unsigned idx) {
            return v[idx] + 1;
        }
};

int main(void) {
    vector<int> v; 
    A a(v);
    std::cout << a.v.size() << std::endl;
    v.push_back(1);
    std::cout << a[0] << std::endl;
    std::cout << sizeof(unsigned ) << std::endl;
    return 0;
}