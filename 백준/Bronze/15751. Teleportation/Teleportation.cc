#include <bits/stdc++.h>
using namespace std;

int main() {
    int a, b, x, y;
    cin >> a >> b >> x >> y;

    // Direct distance without using teleporter
    int direct = abs(a - b);

    // Use teleporter in order a -> x -> y -> b
    int via_xy = abs(a - x) + abs(b - y);

    // Use teleporter in order a -> y -> x -> b
    int via_yx = abs(a - y) + abs(b - x);

    // Output the minimum distance
    cout << min({direct, via_xy, via_yx}) << "\n";

    return 0;
}
