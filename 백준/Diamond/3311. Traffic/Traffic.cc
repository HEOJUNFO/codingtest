#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    long long A, B;
    cin >> n >> m >> A >> B;
    vector<pair<long long, long long>> pos(n + 1);
    vector<bool> is_west(n + 1, false), is_east(n + 1, false);
    vector<int> wests, easts;
    for (int i = 1; i <= n; i++) {
        long long x, y;
        cin >> x >> y;
        pos[i] = {x, y};
        if (x == 0) {
            is_west[i] = true;
            wests.push_back(i);
        }
        if (x == A) {
            is_east[i] = true;
            easts.push_back(i);
        }
    }
    vector<vector<int>> graph(n + 1), rev_graph(n + 1);
    for (int i = 0; i < m; i++) {
        int c, d, k;
        cin >> c >> d >> k;
        graph[c].push_back(d);
        rev_graph[d].push_back(c);
        if (k == 2) {
            graph[d].push_back(c);
            rev_graph[c].push_back(d);
        }
    }
    // Trim unnecessary nodes
    vector<bool> reachable_from_west(n + 1, false);
    queue<int> q;
    for (int w : wests) {
        if (!reachable_from_west[w]) {
            reachable_from_west[w] = true;
            q.push(w);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (!reachable_from_west[v]) {
                reachable_from_west[v] = true;
                q.push(v);
            }
        }
    }
    vector<int> useful_easts;
    for (int e : easts) {
        if (reachable_from_west[e]) {
            useful_easts.push_back(e);
        }
    }
    vector<bool> can_reach_east(n + 1, false);
    queue<int> q2;
    for (int e : useful_easts) {
        if (!can_reach_east[e]) {
            can_reach_east[e] = true;
            q2.push(e);
        }
    }
    while (!q2.empty()) {
        int u = q2.front();
        q2.pop();
        for (int v : rev_graph[u]) {
            if (!can_reach_east[v]) {
                can_reach_east[v] = true;
                q2.push(v);
            }
        }
    }
    // Collect useful nodes
    vector<int> useful_nodes;
    for (int i = 1; i <= n; i++) {
        if (reachable_from_west[i] && can_reach_east[i]) {
            useful_nodes.push_back(i);
        }
    }
    // Kosaraju iterative
    vector<bool> visited(n + 1, false);
    vector<int> post_order;
    for (int start : useful_nodes) {
        if (visited[start]) continue;
        stack<pair<int, int>> stk; // u, child_idx
        stk.push({start, 0});
        visited[start] = true;
        while (!stk.empty()) {
            auto& p = stk.top();
            int u = p.first;
            int& idx = p.second;
            bool done = true;
            while (idx < (int)graph[u].size()) {
                int v = graph[u][idx];
                idx++;
                if (!reachable_from_west[v] || !can_reach_east[v]) continue;
                if (!visited[v]) {
                    visited[v] = true;
                    stk.push({v, 0});
                    done = false;
                    break;
                }
            }
            if (done) {
                post_order.push_back(u);
                stk.pop();
            }
        }
    }
    // Now post_order has postorder, now fill order stack
    stack<int> order;
    for (int i = 0; i < (int)post_order.size(); i++) {
        order.push(post_order[i]);
    }
    // Second pass
    vector<int> scc(n + 1, -1);
    int comp_id = 0;
    visited.assign(n + 1, false);
    while (!order.empty()) {
        int u = order.top();
        order.pop();
        if (visited[u]) continue;
        // iterative DFS for component
        stack<int> stk;
        stk.push(u);
        visited[u] = true;
        scc[u] = comp_id;
        while (!stk.empty()) {
            int cur = stk.top();
            stk.pop();
            for (int v : rev_graph[cur]) {
                if (!reachable_from_west[v] || !can_reach_east[v]) continue;
                if (!visited[v]) {
                    visited[v] = true;
                    scc[v] = comp_id;
                    stk.push(v);
                }
            }
        }
        comp_id++;
    }
    // Build DAG
    vector<set<int>> temp_dag(comp_id);
    for (int u = 1; u <= n; u++) {
        if (scc[u] == -1) continue;
        for (int v : graph[u]) {
            if (scc[v] == -1) continue;
            if (scc[u] != scc[v]) {
                temp_dag[scc[u]].insert(scc[v]);
            }
        }
    }
    vector<vector<int>> dag(comp_id);
    for (int i = 0; i < comp_id; i++) {
        for (int j : temp_dag[i]) {
            dag[i].push_back(j);
        }
    }
    // Build rev_dag and outdeg
    vector<vector<int>> rev_dag(comp_id);
    vector<int> outdeg(comp_id, 0);
    for (int i = 0; i < comp_id; i++) {
        for (int j : dag[i]) {
            outdeg[i]++;
            rev_dag[j].push_back(i);
        }
    }
    // Sort useful_easts by increasing y
    vector<int> sorted_easts = useful_easts;
    sort(sorted_easts.begin(), sorted_easts.end(), [&](int a, int b) {
        return pos[a].second < pos[b].second;
    });
    int qq = sorted_easts.size();
    vector<int> east_idx(n + 1, 0);
    for (int i = 0; i < qq; i++) {
        east_idx[sorted_easts[i]] = i + 1; // 1 to qq
    }
    // scc min and max e
    vector<int> scc_min_e(comp_id, INT_MAX);
    vector<int> scc_max_e(comp_id, INT_MIN);
    for (int e : sorted_easts) {
        int id = scc[e];
        int idx = east_idx[e];
        scc_min_e[id] = min(scc_min_e[id], idx);
        scc_max_e[id] = max(scc_max_e[id], idx);
    }
    // Compute max_north and min_south iteratively
    vector<int> max_north(comp_id, INT_MIN);
    vector<int> min_south(comp_id, INT_MAX);
    vector<int> remaining = outdeg;
    queue<int> que;
    for (int id = 0; id < comp_id; id++) {
        if (remaining[id] == 0) {
            que.push(id);
        }
    }
    while (!que.empty()) {
        int id = que.front();
        que.pop();
        // compute max_north
        int res_max = scc_max_e[id];
        for (int child : dag[id]) {
            res_max = max(res_max, max_north[child]);
        }
        max_north[id] = res_max;
        // compute min_south
        int res_min = scc_min_e[id];
        for (int child : dag[id]) {
            res_min = min(res_min, min_south[child]);
        }
        min_south[id] = res_min;
        // update parents
        for (int par : rev_dag[id]) {
            remaining[par]--;
            if (remaining[par] == 0) {
                que.push(par);
            }
        }
    }
    // Now output
    vector<int> sorted_wests = wests;
    sort(sorted_wests.begin(), sorted_wests.end(), [&](int a, int b) {
        return pos[a].second > pos[b].second;
    });
    for (int w : sorted_wests) {
        if (scc[w] == -1) {
            cout << 0 << '\n';
            continue;
        }
        int id = scc[w];
        int mx = max_north[id];
        int mn = min_south[id];
        if (mx == INT_MIN || mn == INT_MAX || mx < mn) {
            cout << 0 << '\n';
        } else {
            cout << (mx - mn + 1) << '\n';
        }
    }
    return 0;
}