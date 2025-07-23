#include <bits/stdc++.h>

using namespace std;

vector<int> computeLCSLastRow(const string& A, const string& B) {
    int p = A.size();
    int q = B.size();
    vector<int> prev(q + 1, 0);
    vector<int> curr(q + 1, 0);
    for (int i = 1; i <= p; i++) {
        curr[0] = 0;
        for (int j = 1; j <= q; j++) {
            if (A[i - 1] == B[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(curr[j - 1], prev[j]);
            }
        }
        swap(prev, curr);
    }
    return prev;
}

vector<int> computeLCSSuffixLastRow(const string& A, const string& B) {
    string A_rev = A;
    reverse(A_rev.begin(), A_rev.end());
    string B_rev = B;
    reverse(B_rev.begin(), B_rev.end());
    return computeLCSLastRow(A_rev, B_rev);
}

string hirschberg(const string& X, const string& Y) {
    int m = X.size();
    int n = Y.size();
    if (m < n) return hirschberg(Y, X);
    if (m == 0) return "";
    if (m == 1) {
        size_t pos = Y.find(X[0]);
        if (pos != string::npos) return X;
        return "";
    }
    int mid = m / 2;
    string leftX = X.substr(0, mid);
    string rightX = X.substr(mid);
    vector<int> leftScore = computeLCSLastRow(leftX, Y);
    vector<int> rightScore = computeLCSSuffixLastRow(rightX, Y);
    int maxLen = -1;
    int bestK = 0;
    for (int k = 0; k <= n; ++k) {
        int temp = leftScore[k] + rightScore[n - k];
        if (temp > maxLen) {
            maxLen = temp;
            bestK = k;
        }
    }
    string leftLCS = hirschberg(leftX, Y.substr(0, bestK));
    string rightLCS = hirschberg(rightX, Y.substr(bestK));
    return leftLCS + rightLCS;
}

int main() {
    string S, T;
    cin >> S >> T;
    string lcs = hirschberg(S, T);
    cout << lcs.size() << endl;
    cout << lcs << endl;
    return 0;
}