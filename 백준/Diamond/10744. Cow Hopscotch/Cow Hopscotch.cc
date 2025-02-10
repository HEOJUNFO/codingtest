#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
 
const int MOD = 1000000007;
 
// A simple 1-D Fenwick Tree (BIT) for indices 1..n.
struct Fenw1D {
    int n;
    vector<long long> tree;
    Fenw1D(int n): n(n), tree(n+1, 0) {}
    void update(int i, long long delta) {
        for(; i <= n; i += i & -i) {
            tree[i] = (tree[i] + delta) % MOD;
        }
    }
    long long query(int i) {
        long long res = 0;
        for(; i > 0; i -= i & -i) {
            res = (res + tree[i]) % MOD;
        }
        return res;
    }
};
 
// BIT for a given color. Its indices are on the sorted list of column indices where that color appears.
struct FenwColor {
    int n;
    vector<long long> tree;
    FenwColor(int n): n(n), tree(n+1, 0) {}
    void update(int i, long long delta) {
        for(; i <= n; i += i & -i)
            tree[i] = (tree[i] + delta) % MOD;
    }
    long long query(int i) {
        long long res = 0;
        for(; i > 0; i -= i & -i)
            res = (res + tree[i]) % MOD;
        return res;
    }
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int R, C, K;
    cin >> R >> C >> K;
    // Read grid into a 0-indexed 2D vector.
    vector<vector<int>> grid(R, vector<int>(C));
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
            cin >> grid[i][j];
        }
    }
 
    // Preprocessing: for each color (which is in 1..K) store the list of column indices where it appears.
    vector<vector<int>> colorCols(K+1);
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
            int color = grid[i][j];
            colorCols[color].push_back(j);
        }
    }
    // Remove duplicates (i.e. we compress the coordinate of columns for each color)
    vector<FenwColor*> fenwColor(K+1, nullptr);
    for (int color = 1; color <= K; color++){
        if(!colorCols[color].empty()){
            sort(colorCols[color].begin(), colorCols[color].end());
            colorCols[color].erase(unique(colorCols[color].begin(), colorCols[color].end()), colorCols[color].end());
            // Create BIT for this color; size equals number of unique columns where the color appears.
            fenwColor[color] = new FenwColor(colorCols[color].size());
        }
    }
 
    // Global BIT for all dp values; columns are 0-indexed but we use 1-indexed BIT so BIT index = col+1.
    Fenw1D globalBIT(C);
 
    // dp table: dp[i][j] is the number of ways to get to cell (i,j)
    vector<vector<long long>> dp(R, vector<long long>(C, 0));
 
    // Starting position: top-left cell (0,0)
    dp[0][0] = 1;
    // Update BITs with row 0 contributions.
    globalBIT.update(0+1, dp[0][0]);
    int c0 = grid[0][0];
    // For BIT for color c0, find the position corresponding to column 0.
    int pos = int(lower_bound(colorCols[c0].begin(), colorCols[c0].end(), 0) - colorCols[c0].begin()) + 1;
    fenwColor[c0]->update(pos, dp[0][0]);
 
    // Process rows 1..R-1 (cells in the same row cannot jump because row must strictly increase).
    for (int i = 1; i < R; i++){
        // Compute dp for each cell in row i.
        for (int j = 0; j < C; j++){
            // Only previous rows contribute, so query the global BIT (for all previous rows)
            // We want contributions from cells with column index < j.
            long long total = globalBIT.query(j); // j is BIT index since BIT indices correspond to col+1.
            int colVal = grid[i][j];
            long long same = 0;
            if (fenwColor[colVal] != nullptr) {
                // In BIT for color colVal, we want to sum over contributions from cells with column < j.
                // Our BIT is built on the sorted vector "colorCols[colVal]". Find the number of indices that are < j.
                int idx = int(lower_bound(colorCols[colVal].begin(), colorCols[colVal].end(), j) - colorCols[colVal].begin());
                same = fenwColor[colVal]->query(idx);
            }
            dp[i][j] = (total - same) % MOD;
            if(dp[i][j] < 0)
                dp[i][j] += MOD;
        }
        // After processing the whole row, update the BITs with this row's dp values.
        for (int j = 0; j < C; j++){
            long long value = dp[i][j];
            if (value == 0) continue;
            globalBIT.update(j+1, value);
            int colVal = grid[i][j];
            if (fenwColor[colVal] != nullptr) {
                int pos = int(lower_bound(colorCols[colVal].begin(), colorCols[colVal].end(), j) - colorCols[colVal].begin()) + 1;
                fenwColor[colVal]->update(pos, value);
            }
        }
    }
 
    cout << dp[R-1][C-1] % MOD << "\n";
    return 0;
}
