#include <iostream>
#include <set>
using namespace std;

struct Node {
    int v;
    int idx;
    Node(int v, int idx) : v(v), idx(idx) {};
    bool friend operator < (Node a, Node b) {
        return a.v < b.v;
    }
};

int main() {
    set<Node> s;
    s.insert(Node(1, 2));
    s.insert(Node(3, 2));
    s.insert(Node(3, 4));
    s.insert(Node(4, 4));
    s.insert(Node(3, 2));
    s.insert(Node(2, 2));
    for (auto i:s) {
        std::cout << i.v << ' ' << i.idx << std::endl;
    }
    std::cout << s.size() << std::endl;
    s.erase(--s.end());
    std::cout << s.size() << std::endl;
    s.erase(--s.end());
    std::cout << s.size() << std::endl;
    s.erase(--s.end());
    std::cout << s.size() << std::endl;
    return 0;
}