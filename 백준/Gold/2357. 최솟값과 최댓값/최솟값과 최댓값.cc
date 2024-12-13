#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000001; // 문제에서 최대 값이 10^9 이므로, 그보다 큰 값으로 설정

int N, M;
vector<long long> arr;
vector<long long> segMin, segMax;

long long initMin(int idx, int start, int end) {
    if (start == end) return segMin[idx] = arr[start];
    int mid = (start + end) / 2;
    return segMin[idx] = min(initMin(idx*2, start, mid), initMin(idx*2+1, mid+1, end));
}

long long initMax(int idx, int start, int end) {
    if (start == end) return segMax[idx] = arr[start];
    int mid = (start + end) / 2;
    return segMax[idx] = max(initMax(idx*2, start, mid), initMax(idx*2+1, mid+1, end));
}

long long queryMin(int idx, int start, int end, int left, int right) {
    if (right < start || end < left) return INF;
    if (left <= start && end <= right) return segMin[idx];
    int mid = (start + end) / 2;
    return min(queryMin(idx*2, start, mid, left, right), queryMin(idx*2+1, mid+1, end, left, right));
}

long long queryMax(int idx, int start, int end, int left, int right) {
    if (right < start || end < left) return -INF;
    if (left <= start && end <= right) return segMax[idx];
    int mid = (start + end) / 2;
    return max(queryMax(idx*2, start, mid, left, right), queryMax(idx*2+1, mid+1, end, left, right));
}

int main(){
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);

    cin >> N >> M;
    arr.resize(N+1);
    for (int i = 1; i <= N; i++){
        cin >> arr[i];
    }

    segMin.resize(4*N, 0);
    segMax.resize(4*N, 0);

    initMin(1, 1, N);
    initMax(1, 1, N);

    while(M--){
        int a, b; cin >> a >> b;
        if(a > b) swap(a, b);
        cout << queryMin(1, 1, N, a, b) << " " << queryMax(1, 1, N, a, b) << "\n";
    }

    return 0;
}
