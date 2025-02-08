#include <bits/stdc++.h>
using namespace std;
 
/*
  Explanation:
 
  – There are N knights in total. The already‐arrived knights (N–1 of them) are given
    in order in the array K.
 
  – The late knight has skill R (distinct from all K values) and must be inserted into the
    initial lineup at some position P (0 ≤ P ≤ N–1). When inserted the order becomes:
      [ K[0], K[1], …, K[P–1], R, K[P], …, K[N–2] ].
 
  – The tournament consists of C rounds. In round r (0‑indexed) the interval [S[r], E[r]]
    of the current lineup is chosen. All knights in that interval “compete” and the knight
    with the highest skill wins (it is known that all skills are distinct). The winner returns
    to the lineup (its “slot” is the beginning of the interval) while all the other knights in
    the interval are eliminated.
 
  – If the late knight is inside the chosen interval then he competes.
    If his skill is the maximum (i.e. equals R) he wins the round (and we increment his win‐count)
    and his new “position” becomes S[r]. If he is in the interval but does not win then he is
    eliminated immediately and does not compete further.
 
  – If the late knight is not in the round’s interval the round is simulated for the other knights.
 
  – For each candidate insertion position P (from 0 to N–1) we simulate all rounds (or stop early if
    the late knight is eliminated) and count how many rounds he wins.
 
  – Finally we output the position P that yields the maximum win count (and if several P yield the same,
    the smallest such P is output).
 
  (This direct simulation uses a vector to represent the current lineup. In each round the
  segment [S, E] is scanned to find the maximum and then the new lineup is built. This is acceptable
  for small N and C – that is, for the subtask version.)
*/
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, C, R;
    cin >> N >> C >> R;
    // Read the skills of the N-1 knights already in line.
    vector<int> K(N-1);
    for (int i = 0; i < N-1; i++){
        cin >> K[i];
    }
    // Read the C rounds; each round is given by S and E.
    vector<int> S(C), E(C);
    for (int i = 0; i < C; i++){
        cin >> S[i] >> E[i];
    }
 
    int bestP = 0, bestWins = -1;
    // Try every possible insertion position for the late knight.
    for (int P = 0; P < N; P++){
        // Build the initial lineup:
        // positions 0..P–1 come from K[0..P–1],
        // position P is the late knight (with skill R),
        // positions P+1..N–1 come from K[P..N–2].
        vector<int> lineup;
        lineup.reserve(N);
        for (int i = 0; i < P; i++){
            lineup.push_back(K[i]);
        }
        lineup.push_back(R);
        for (int i = P; i < N-1; i++){
            lineup.push_back(K[i]);
        }
 
        int wins = 0;
        bool alive = true; // whether the late knight is still in the tournament
        // Process the rounds sequentially.
        for (int r = 0; r < C; r++){
            int L = lineup.size(); // current number of knights in the lineup
            int start = S[r], end = E[r];
            // It is given that 0 ≤ start < end < L.
 
            // Find the maximum value and its index in lineup[start ... end].
            int maxVal = -1, maxIdx = -1;
            for (int i = start; i <= end; i++){
                if(lineup[i] > maxVal){
                    maxVal = lineup[i];
                    maxIdx = i;
                }
            }
 
            // Check whether the late knight (skill R) is in this round’s interval.
            bool lateInRange = false;
            for (int i = start; i <= end; i++){
                if(lineup[i] == R){
                    lateInRange = true;
                    break;
                }
            }
 
            if(lateInRange){
                // Late knight competes.
                if(maxVal == R){
                    // He wins this round.
                    wins++;
                    // After the round the lineup is updated:
                    // positions 0..start–1 remain,
                    // then the winner (late knight) is placed at position start,
                    // then all knights after position end remain.
                    vector<int> newLineup;
                    newLineup.reserve(L - (end - start));
                    for (int i = 0; i < start; i++){
                        newLineup.push_back(lineup[i]);
                    }
                    newLineup.push_back(R);
                    for (int i = end+1; i < L; i++){
                        newLineup.push_back(lineup[i]);
                    }
                    lineup = move(newLineup);
                } else {
                    // Late knight participated but did not win: he is eliminated.
                    alive = false;
                    break;
                }
            } else {
                // Late knight is not in the interval;
                // update the lineup by replacing the subarray with its winner.
                vector<int> newLineup;
                newLineup.reserve(L - (end - start));
                for (int i = 0; i < start; i++){
                    newLineup.push_back(lineup[i]);
                }
                newLineup.push_back(lineup[maxIdx]); // winner of the round (from pre‐arrived knights)
                for (int i = end+1; i < L; i++){
                    newLineup.push_back(lineup[i]);
                }
                lineup = move(newLineup);
            }
        }
        // Record the result: even if the late knight was eliminated mid‐tournament,
        // his win count is what he achieved before elimination.
        if(wins > bestWins || (wins == bestWins && P < bestP)){
            bestWins = wins;
            bestP = P;
        }
    }
 
    cout << bestP << "\n";
    return 0;
}
