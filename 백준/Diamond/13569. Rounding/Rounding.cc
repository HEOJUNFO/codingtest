#include <bits/stdc++.h>
using namespace std;
 
//----- Dinic max-flow implementation -----
 
struct Edge {
    int to, rev, cap;
};
 
struct Dinic {
    int n;
    vector<vector<Edge>> graph;
    vector<int> level, ptr;
    
    Dinic(int n): n(n), graph(n), level(n), ptr(n) {}
    
    void add_edge(int s, int t, int cap) {
        Edge a = {t, (int)graph[t].size(), cap};
        Edge b = {s, (int)graph[s].size(), 0};
        graph[s].push_back(a);
        graph[t].push_back(b);
    }
    
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int>q;
        q.push(s);
        while(!q.empty()){
            int v = q.front();
            q.pop();
            for(auto &e : graph[v]){
                if(e.cap && level[e.to] < 0){
                    level[e.to] = level[v]+1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    
    int dfs (int v, int t, int flow) {
        if(!flow) return 0;
        if(v==t) return flow;
        for(int &i = ptr[v]; i < (int)graph[v].size(); i++){
            Edge &e = graph[v][i];
            if(e.cap && level[v]+1 == level[e.to]){
                int pushed = dfs(e.to, t, min(flow, e.cap));
                if(pushed){
                    e.cap -= pushed;
                    graph[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        while(bfs(s,t)){
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s,t,INT_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};
 
//----- Main Program -----
 
// We multiply every input number by 10 so that, for example, 4.3 becomes 43.
// Then, note that the floor of 4.3 is 4 (= 43/10 using integer division).
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int M, N;
    cin >> M >> N;
    // a_int[i][j] holds the experimental data (each multiplied by 10)
    vector<vector<int>> a_int(M, vector<int>(N,0));
    // rowSum_int[i] holds the row–sum (multiplied by 10)
    vector<int> rowSum_int(M,0);
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            double tmp;
            cin >> tmp;
            int val = (int)round(tmp * 10);
            a_int[i][j] = val;
        }
        double rtmp;
        cin >> rtmp;
        rowSum_int[i] = (int)round(rtmp * 10);
    }
    // Read the N column sums
    vector<int> colSum_int(N,0);
    for (int j = 0; j < N; j++){
        double tmp;
        cin >> tmp;
        int val = (int)round(tmp * 10);
        colSum_int[j] = val;
    }
 
    // Precompute the sum of the floors (data cells) in each row and column.
    vector<int> rowFloorSum(M,0);
    for (int i = 0; i < M; i++){
        int sum = 0;
        for (int j = 0; j < N; j++){
            sum += a_int[i][j] / 10; // integer division gives the floor.
        }
        rowFloorSum[i] = sum;
    }
    vector<int> colFloorSum(N,0);
    for (int j = 0; j < N; j++){
        int sum = 0;
        for (int i = 0; i < M; i++){
            sum += a_int[i][j] / 10;
        }
        colFloorSum[j] = sum;
    }
 
    // Decide the rounding for the row–sums.
    // For a row i, if rowSum_int[i] % 10 == 0, then the only option is rowSum_int[i]/10.
    // Otherwise, allowed values are floor (rowSum_int[i]/10) or floor+1.
    vector<int> r_round(M,0); // final rounded row–sum
    vector<int> rowExtra(M,0); // extra part: 0 or 1 (only for "free" rows)
    vector<bool> rowFree(M,false);
    for (int i = 0; i < M; i++){
        int base = rowSum_int[i] / 10;
        if(rowSum_int[i] % 10 == 0){
            r_round[i] = base;
            rowExtra[i] = 0;
        } else {
            rowFree[i] = true;
            // Decision to add 0 or 1 will be made below.
        }
    }
    // Similarly for the column–sums.
    vector<int> c_round(N,0);
    vector<int> colExtra(N,0);
    vector<bool> colFree(N,false);
    for (int j = 0; j < N; j++){
        int base = colSum_int[j] / 10;
        if(colSum_int[j] % 10 == 0){
            c_round[j] = base;
            colExtra[j] = 0;
        } else {
            colFree[j] = true;
        }
    }
 
    // To have the overall totals match we need:
    //    sum_{i=0}^{M-1} r_round[i] = sum_{j=0}^{N-1} c_round[j].
    // Write r_round[i] = (rowSum_int[i]/10) + (if free then x_i else 0)
    // and c_round[j] = (colSum_int[j]/10) + (if free then y_j else 0).
    // Then the equality becomes:
    //    sum_{i free} x_i - sum_{j free} y_j = -Delta0,
    // where Delta0 = (sum_i rowSum_int[i]/10) - (sum_j colSum_int[j]/10).
    int sumRowFloorAll = 0;
    for (int i = 0; i < M; i++){
        sumRowFloorAll += rowSum_int[i] / 10;
    }
    int sumColFloorAll = 0;
    for (int j = 0; j < N; j++){
        sumColFloorAll += colSum_int[j] / 10;
    }
    int Delta0 = sumRowFloorAll - sumColFloorAll; // may be negative, zero, or positive.
    // We require: sum_{i free} x_i - sum_{j free} y_j = -Delta0.
    // A simple assignment:
    if(Delta0 <= 0){
        int needed = -Delta0; // choose exactly (-Delta0) free rows to have x_i=1; set free columns all to 0.
        for (int j = 0; j < N; j++){
            if(colFree[j]){
                colExtra[j] = 0;
                c_round[j] = (colSum_int[j] / 10) + 0;
            }
        }
        for (int i = 0; i < M; i++){
            if(rowFree[i]){
                if(needed > 0){
                    rowExtra[i] = 1;
                    needed--;
                } else {
                    rowExtra[i] = 0;
                }
                r_round[i] = (rowSum_int[i] / 10) + rowExtra[i];
            }
        }
    } else { // Delta0 > 0: set free rows x_i=0 and choose exactly Delta0 free columns with y_j=1.
        int needed = Delta0;
        for (int i = 0; i < M; i++){
            if(rowFree[i]){
                rowExtra[i] = 0;
                r_round[i] = (rowSum_int[i] / 10) + 0;
            }
        }
        for (int j = 0; j < N; j++){
            if(colFree[j]){
                if(needed > 0){
                    colExtra[j] = 1;
                    needed--;
                } else {
                    colExtra[j] = 0;
                }
                c_round[j] = (colSum_int[j] / 10) + colExtra[j];
            }
        }
    }
    // Now the totals match:
    //    sum_{i=0}^{M-1} r_round[i] == sum_{j=0}^{N-1} c_round[j].
 
    // For each row i, the data cells must sum to r_round[i]. But the sum of floors is rowFloorSum[i].
    // Hence, the number of cells in row i that must be “rounded up” (i.e. add 1) is:
    //    R_req(i) = r_round[i] - rowFloorSum[i].
    // Similarly for each column j:
    //    C_req(j) = c_round[j] - colFloorSum[j].
    vector<int> R_req(M, 0), C_req(N, 0);
    for (int i = 0; i < M; i++){
        R_req[i] = r_round[i] - rowFloorSum[i];
    }
    for (int j = 0; j < N; j++){
        C_req[j] = c_round[j] - colFloorSum[j];
    }
    int totalRowReq = 0, totalColReq = 0;
    for (int i = 0; i < M; i++) totalRowReq += R_req[i];
    for (int j = 0; j < N; j++) totalColReq += C_req[j];
    // (It must hold that totalRowReq == totalColReq.)
 
    // --- Now we set up the bipartite flow to choose which data cells to round up.
    // We build a flow network:
    //   source --> row node (capacity = R_req[i])
    //   row node --> column node (capacity = 1, for each cell)
    //   column node --> sink (capacity = C_req[j])
    //
    // Use node numbering:
    //   source = 0
    //   row nodes: 1 ... M
    //   column nodes: M+1 ... M+N
    //   sink = M+N+1
    int source = 0;
    int sink = M + N + 1;
    int V = sink + 1;
    Dinic dinic(V);
 
    // Source -> row nodes
    for (int i = 0; i < M; i++){
        dinic.add_edge(source, 1 + i, R_req[i]);
    }
    // Row nodes -> column nodes (each cell gets an edge of capacity 1)
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            dinic.add_edge(1 + i, 1 + M + j, 1);
        }
    }
    // Column nodes -> sink
    for (int j = 0; j < N; j++){
        dinic.add_edge(1 + M + j, sink, C_req[j]);
    }
 
    int flow = dinic.max_flow(source, sink);
    // (A solution is guaranteed so that flow == totalRowReq.)
 
    // Now, for each cell (i,j) we decide:
    // if an edge from row node i to column node j was “saturated” (i.e. flow 1 was sent),
    // then we set d[i][j]=1 (round up); else 0.
    vector<vector<int>> d(M, vector<int>(N, 0));
    for (int i = 0; i < M; i++){
        int node = 1 + i;
        for (auto &e : dinic.graph[node]){
            // edges to column nodes have e.to in [1+M, 1+M+N-1]
            if(e.to >= 1 + M && e.to < 1 + M + N){
                int j = e.to - (1 + M);
                if(e.cap == 0) // originally capacity was 1; if now 0, then flow 1 was sent.
                    d[i][j] = 1;
                else
                    d[i][j] = 0;
            }
        }
    }
 
    // The final rounded value for cell (i,j) is: floor(a[i][j]) + d[i][j].
    // (Recall: floor(a[i][j]) = a_int[i][j]/10)
    //
    // Now we output the table.
    // For each row, output the N rounded data values and then the rounded row sum r_round[i].
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            int cellRounded = (a_int[i][j] / 10) + d[i][j];
            cout << cellRounded << " ";
        }
        cout << r_round[i] << "\n";
    }
    // Finally, output one line with the N rounded column sums.
    for (int j = 0; j < N; j++){
        cout << c_round[j] << (j==N-1 ? "\n" : " ");
    }
 
    return 0;
}
