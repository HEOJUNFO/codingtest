#include <bits/stdc++.h>
using namespace std;
 
struct Exam {
    int T, P;
    double ratio;
};
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<Exam> exams(N);
    for (int i=0; i<N; i++){
        cin >> exams[i].T >> exams[i].P;
        exams[i].ratio = (double)exams[i].T / exams[i].P;
    }
    // sort by T/P (if equal, sort by T then by P)
    sort(exams.begin(), exams.end(), [](const Exam &a, const Exam &b){
        if(fabs(a.ratio - b.ratio) > 1e-12)
            return a.ratio < b.ratio;
        if(a.T != b.T) return a.T < b.T;
        return a.P < b.P;
    });
 
    // Precompute total T, total P and prefix sums.
    long long totalT = 0, totalP = 0;
    vector<long long> prefT(N+1,0), prefP(N+1,0);
    for (int i=0; i<N; i++){
        totalT += exams[i].T;
        totalP += exams[i].P;
        prefT[i+1] = prefT[i] + exams[i].T;
        prefP[i+1] = prefP[i] + exams[i].P;
    }
 
    vector<int> valid; // list of D's for which an alternative removal exists.
 
    // For each D from 1 to N-1:
    for (int D = 1; D < N; D++){
        long long remT = prefT[D], remP = prefP[D];
        long long keepT = totalT - remT, keepP = totalP - remP;
        double Rteacher = (double)keepT / keepP; // teacher's grade
        double U = (double)keepT, V = (double)keepP;
 
        bool canImprove = false;
        // To avoid worst-case double loop we sample a few indices from each side.
        int sampleSize = 50;
        int remSize = D, keepSize = N - D;
 
        vector<int> remIndices;
        for (int j = 0; j < min(remSize, sampleSize); j++){
            remIndices.push_back(j);
        }
        for (int j = max(0, remSize - sampleSize); j < remSize; j++){
            remIndices.push_back(j);
        }
 
        vector<int> keepIndices;
        for (int i = D; i < D + min(keepSize, sampleSize); i++){
            keepIndices.push_back(i);
        }
        for (int i = max(D, N - sampleSize); i < N; i++){
            keepIndices.push_back(i);
        }
 
        sort(remIndices.begin(), remIndices.end());
        remIndices.erase(unique(remIndices.begin(), remIndices.end()), remIndices.end());
        sort(keepIndices.begin(), keepIndices.end());
        keepIndices.erase(unique(keepIndices.begin(), keepIndices.end()), keepIndices.end());
 
        // Check over the sampled pairs.
        for (int j : remIndices){
            for (int i : keepIndices){
                int dP = exams[i].P - exams[j].P;
                int dT = exams[i].T - exams[j].T;
                // if dP==0, then if dT<0 swapping lowers removed T (hence improves kept grade)
                if(dP == 0){
                    if(dT < 0){ canImprove = true; break; }
                    else continue;
                }
                double candidate = (double)dT / dP;
                if(dP > 0){
                    if(candidate < Rteacher - 1e-12){ canImprove = true; break; }
                } else { // dP < 0; then inequality reverses
                    if(candidate > Rteacher + 1e-12){ canImprove = true; break; }
                }
            }
            if(canImprove) break;
        }
 
        if(canImprove)
            valid.push_back(D);
    }
 
    sort(valid.begin(), valid.end());
    cout << valid.size() << "\n";
    for(auto d : valid)
        cout << d << "\n";
 
    return 0;
}
