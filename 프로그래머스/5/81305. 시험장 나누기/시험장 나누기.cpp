#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

vector<int> info;
vector<int> G[100001];
bool vis[100001];
int indegree[100001];

int group_size;
int group;


int test(int node){
    priority_queue<int> pq;
    int sum = 0;
    int sum_subtree = 0;
    vis[node] = true;
    for(int nxt : G[node]){
        // cout << nxt << '\n';
        if(!vis[nxt]){
            sum_subtree = test(nxt);
            sum += sum_subtree;
            pq.push(sum_subtree);   
        }
    }

    while(!pq.empty() && sum + info[node] > group_size){ // current max size 
        group+=1;
        sum -= pq.top(); pq.pop();   
    }

    return sum + info[node];
}

int solution(int k, vector<int> num, vector<vector<int>> links) {

    int n = num.size();
    int l = -1;
    int r = 0;

    // G, indegree, l, r : initialize
    for(int i = 0;i<n;i++){
        info.push_back(num[i]);
        l = max(l, num[i]);
        r+= num[i];
        for(int j = 0;j<2;j++){
            if(links[i][j]!=-1){
                G[i].push_back(links[i][j]);
                // G[links[i][j]].push_back(i);
                indegree[links[i][j]]+=1;
            }
        }
    }    

    // root 
    int root = 0;
    for(int i = 0;i<n;i++){
        if(indegree[i]==0){
            root = i;
            break;
        }
    }

    int ans = r;
    while(l<r){
        for(int i = 0;i<n;i++){
            vis[i] = 0;
        }
        group = 0;

        group_size = (l+r)/2;
        test(root);
        // cout << l << " " << r << " " << group << '\n';
        if(group < k){
            r = group_size; // mid down -> group up
            ans = group_size;
        }else{
            l = group_size + 1; // mid up ->  group down 
        }
    }

    return ans;
}

