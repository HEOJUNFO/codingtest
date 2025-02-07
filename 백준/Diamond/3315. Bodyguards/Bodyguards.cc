#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
struct Group {
    ll val, cnt;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int R;
    cin >> R;
    vector<Group> rowGroups(R);
    ll totalRows = 0, totalRowSum = 0;
    for (int i = 0; i < R; i++){
        cin >> rowGroups[i].val >> rowGroups[i].cnt;
        totalRows += rowGroups[i].cnt;
        totalRowSum += rowGroups[i].val * rowGroups[i].cnt;
    }
    
    int C;
    cin >> C;
    vector<Group> colGroups(C);
    ll totalCols = 0, totalColSum = 0;
    for (int i = 0; i < C; i++){
        cin >> colGroups[i].val >> colGroups[i].cnt;
        totalCols += colGroups[i].cnt;
        totalColSum += colGroups[i].val * colGroups[i].cnt;
    }
    
    // Quick check: total bodyguards as determined by rows and columns must match.
    if(totalRowSum != totalColSum){
        cout << 0 << "\n";
        return 0;
    }
    
    // Sort row groups in descending order by required bodyguards.
    sort(rowGroups.begin(), rowGroups.end(), [](const Group &a, const Group &b){
        return a.val > b.val;
    });
    // Sort column groups in descending order.
    sort(colGroups.begin(), colGroups.end(), [](const Group &a, const Group &b){
        return a.val > b.val;
    });
    
    // Precompute prefix sums for rows.
    int Rsize = R;
    vector<ll> rowCountPrefix(Rsize), rowSumPrefix(Rsize);
    rowCountPrefix[0] = rowGroups[0].cnt;
    rowSumPrefix[0] = rowGroups[0].val * rowGroups[0].cnt;
    for (int i = 1; i < Rsize; i++){
        rowCountPrefix[i] = rowCountPrefix[i-1] + rowGroups[i].cnt;
        rowSumPrefix[i] = rowSumPrefix[i-1] + rowGroups[i].val * rowGroups[i].cnt;
    }
    
    // Precompute prefix sums for column groups.
    int Csize = C;
    vector<ll> colCountPrefix(Csize), colSumPrefix(Csize);
    colCountPrefix[0] = colGroups[0].cnt;
    colSumPrefix[0] = colGroups[0].val * colGroups[0].cnt;
    for (int i = 1; i < Csize; i++){
        colCountPrefix[i] = colCountPrefix[i-1] + colGroups[i].cnt;
        colSumPrefix[i] = colSumPrefix[i-1] + colGroups[i].val * colGroups[i].cnt;
    }
    
    // fR(k): returns sum of the top k row requirements.
    auto fR = [&](ll k) -> ll {
        // Find the group where the kth row falls using binary search on rowCountPrefix.
        int idx = int(upper_bound(rowCountPrefix.begin(), rowCountPrefix.end(), k-1) - rowCountPrefix.begin());
        ll sum = 0;
        if(idx > 0) sum = rowSumPrefix[idx-1];
        ll prevCount = (idx > 0 ? rowCountPrefix[idx-1] : 0);
        ll rem = k - prevCount;
        sum += rem * rowGroups[idx].val;
        return sum;
    };
    
    // fC(k): returns sum_{j=1}^{totalCols} min(c_j, k)
    auto fC = [&](ll k) -> ll {
        // Find how many column groups have value >= k using binary search.
        int lo = 0, hi = Csize;
        while(lo < hi){
            int mid = (lo + hi) / 2;
            if(colGroups[mid].val >= k)
                lo = mid + 1;
            else
                hi = mid;
        }
        int groupsCount = lo; // groups [0, groupsCount-1] have col value >= k.
        ll cntHigh = (groupsCount > 0 ? colCountPrefix[groupsCount-1] : 0);
        ll sumHigh = (groupsCount > 0 ? colSumPrefix[groupsCount-1] : 0);
        // For columns with requirement >= k, each contributes k.
        ll res = k * cntHigh + (totalColSum - sumHigh);
        return res;
    };
    
    // Gather candidate k-values (the breakpoints) to check the inequality.
    vector<ll> candidates;
    candidates.push_back(1);
    candidates.push_back(totalRows);
    for(auto val : rowCountPrefix){
        if(val >= 1 && val <= totalRows)
            candidates.push_back(val);
    }
    for(auto &grp : colGroups){
        if(grp.val >= 1 && grp.val <= totalRows)
            candidates.push_back(grp.val);
        if(grp.val + 1 >= 1 && grp.val + 1 <= totalRows)
            candidates.push_back(grp.val + 1);
    }
    
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());
    
    // Check the Gale–Ryser inequality for each candidate k.
    for(auto k : candidates){
        if(k > totalRows) continue;
        ll left = fR(k);
        ll right = fC(k);
        if(left > right){
            cout << 0 << "\n";
            return 0;
        }
    }
    
    cout << 1 << "\n";
    return 0;
}
