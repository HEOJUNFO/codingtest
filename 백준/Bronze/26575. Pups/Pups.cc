#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    for (int i = 0; i < n; ++i) {
        double d, f, p;
        cin >> d >> f >> p;
        double total = d * f * p;
        cout << fixed << setprecision(2) << "$" << total << "\n";
    }

    return 0;
}
