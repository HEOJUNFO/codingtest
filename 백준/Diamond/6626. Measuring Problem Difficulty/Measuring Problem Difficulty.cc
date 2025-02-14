#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

struct Node {
    int y, z; // y = position in second permutation, z = position in third permutation.
};

int N;
vector<Node> arr, temp;

// Binary Indexed Tree (Fenwick tree) for indices 1..N.
vector<ll> BIT;
int sizeBIT;

void initBIT(int n) {
    sizeBIT = n;
    BIT.assign(n + 1, 0);
}

inline void update(int i, ll delta) {
    for(; i <= sizeBIT; i += i & (-i))
        BIT[i] += delta;
}

inline ll query(int i) {
    ll s = 0;
    for(; i > 0; i -= i & (-i))
        s += BIT[i];
    return s;
}

// CDQ Divide-and-Conquer: arr[l..r] is in order of the first permutation.
// We want to count pairs (i, j) with l <= i < j <= r such that:
//    arr[i].y < arr[j].y and arr[i].z < arr[j].z.
// In the recursion the array gets merged (sorted by y) so that cross–pairs
// can be counted using a BIT keyed on z.
ll cdq(int l, int r) {
    if(l >= r) return 0;
    int mid = (l + r) / 2;
    ll res = 0;
    res += cdq(l, mid);
    res += cdq(mid + 1, r);
    
    int i = l, j = mid + 1, k = 0;
    // We'll record BIT updates for left–side elements so we can later remove them.
    vector<int> leftUpdated;
    
    // Merge the two halves while counting cross–pairs.
    while(i <= mid && j <= r) {
        if(arr[i].y <= arr[j].y) {
            update(arr[i].z, 1);
            leftUpdated.push_back(arr[i].z);
            temp.push_back(arr[i]);
            i++;
        } else {
            // For an element in the right half, count how many left–side elements have z < current z.
            res += query(arr[j].z - 1);
            temp.push_back(arr[j]);
            j++;
        }
    }
    while(i <= mid) {
        update(arr[i].z, 1);
        leftUpdated.push_back(arr[i].z);
        temp.push_back(arr[i]);
        i++;
    }
    while(j <= r) {
        res += query(arr[j].z - 1);
        temp.push_back(arr[j]);
        j++;
    }
    // Remove BIT updates (only for left half elements added earlier).
    for(auto z : leftUpdated) {
        update(z, -1);
    }
    // Copy merged (sorted by y) result back into arr[l..r].
    for(int t = 0; t < (int)temp.size(); t++) {
        arr[l + t] = temp[t];
    }
    temp.clear();
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    while(true) {
        cin >> N;
        if(N == 0) break;
        
        // Read the three permutations.
        vector<int> perm1(N), perm2(N), perm3(N);
        for (int i = 0; i < N; i++)
            cin >> perm1[i];
        for (int i = 0; i < N; i++)
            cin >> perm2[i];
        for (int i = 0; i < N; i++)
            cin >> perm3[i];
        
        // Build position arrays for second and third permutations.
        // pos2[x] gives the position (1-indexed) of problem x in perm2.
        // pos3[x] gives the position (1-indexed) of problem x in perm3.
        vector<int> pos2(N + 1), pos3(N + 1);
        for (int i = 0; i < N; i++) {
            pos2[perm2[i]] = i + 1;
        }
        for (int i = 0; i < N; i++) {
            pos3[perm3[i]] = i + 1;
        }
        
        // Build array 'arr' in the order of the first permutation.
        // Each Node stores (y, z) = (position in perm2, position in perm3).
        arr.resize(N);
        for (int i = 0; i < N; i++) {
            int prob = perm1[i];
            arr[i].y = pos2[prob];
            arr[i].z = pos3[prob];
        }
        
        // Initialize BIT for coordinate range [1, N].
        initBIT(N);
        
        // Count the number of pairs with consistent ordering.
        ll ans = cdq(0, N - 1);
        cout << ans << "\n";
    }
    
    return 0;
}
