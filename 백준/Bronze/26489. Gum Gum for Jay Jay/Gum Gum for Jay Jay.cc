#include <iostream>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    int count = 0;
    while (true) {
        if(!std::getline(cin, line)) break;
        count++;
    }
    cout << count << "\n";
    return 0;
}
