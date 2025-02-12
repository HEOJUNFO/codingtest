#include <bits/stdc++.h>
using namespace std;
 
// --- Global constants and modular arithmetic ---
 
const long long MOD = 1000000007;
// In our treap we only need to compare “room numbers” (which come from reverse–simulation)
// and since query type‑3 asks for x up to 1e9 we can clamp our numbers.
const long long INF_CLAMP = (1LL << 40); // a safe clamp (about 1e12)
 
// --- Transform type and composition ---
// A transform represents the effect of a block of arrival events (processed in reverse)
// as:  x -> (x - S) / (2^p)
// For a finite op (1 k, k>=1) we have (p,S) = (0,k)
// For an infinite op (1 0) we have (p,S) = (1,0)
 
struct Transform {
    int p;       // number of infinite ops in the block
    long long S; // finite shift (clamped if too big)
};
 
// Identity transform f(x)=x.
inline Transform identityTransform(){
    Transform ret;
    ret.p = 0;
    ret.S = 0;
    return ret;
}
 
// Compute 2^p but if p >= 31, return a value >1e9.
inline long long twoPow(int p) {
    if(p >= 31) return (1LL << 31);
    return (1LL << p);
}
 
// Compose transforms: first F then G gives H with
// H(x) = G( F(x) ) = ( x - (S_F + 2^(p_F)*S_G) ) / (2^(p_F+p_G)).
inline Transform combineTransform(const Transform &F, const Transform &G){
    Transform ret;
    ret.p = F.p + G.p;
    long long mul = twoPow(F.p);
    if(mul >= INF_CLAMP || G.S >= INF_CLAMP/mul)
        ret.S = INF_CLAMP;
    else {
        long long addVal = mul * G.S;
        ret.S = F.S + addVal;
        if(ret.S >= INF_CLAMP) ret.S = INF_CLAMP;
    }
    return ret;
}
 
// Check if a transform is "applicable" to x (i.e. if none of the ops in the block capture the guest).
// If applicable, update x to f(x) = (x - S)/2^(p). (When p>=31 we require x==S.)
inline bool applyTransform(const Transform &tr, long long &x){
    if(x < tr.S) return false;
    if(tr.p == 0){
        x = x - tr.S;
        return true;
    } else {
        if(tr.p >= 31){
            if(x != tr.S) return false;
            x = 0;
            return true;
        } else {
            long long modBase = twoPow(tr.p);
            long long diff = x - tr.S;
            if(diff % modBase != 0) return false;
            x = diff / modBase;
            return true;
        }
    }
}
 
// --- Treap for reverse simulation (type 3 queries) ---
// In simulation order the most recent arrival event is at position 0.
// (For each type 1 query we “insert” at the beginning.)
 
struct Node {
    int priority, sz;
    // The op's transform value:
    // For a finite op (1 k with k>=1): (op_p, op_S) = (0, k)
    // For an infinite op (1 0): (op_p, op_S) = (1, 0)
    int op_p;
    long long op_S;
    int group;  // group number for this event.
    // Aggregated transform for the subtree (in in–order, i.e. simulation order).
    int agg_p;
    long long agg_S;
    Node *l, *r;
    Node(int op_p, long long op_S, int group) : op_p(op_p), op_S(op_S), group(group) {
        priority = rand();
        sz = 1;
        agg_p = op_p;
        agg_S = op_S;
        l = r = nullptr;
    }
};
 
int getSize(Node* root){
    return root ? root->sz : 0;
}
 
// Recalculate size and aggregated transform.
void recalc(Node* root){
    if(!root) return;
    root->sz = 1 + getSize(root->l) + getSize(root->r);
    Transform Ltr = identityTransform();
    if(root->l){
        Ltr.p = root->l->agg_p;
        Ltr.S = root->l->agg_S;
    }
    Transform cur;
    cur.p = root->op_p;
    cur.S = root->op_S;
    Transform temp = combineTransform(Ltr, cur);
    Transform Rtr = identityTransform();
    if(root->r){
        Rtr.p = root->r->agg_p;
        Rtr.S = root->r->agg_S;
    }
    Transform tot = combineTransform(temp, Rtr);
    root->agg_p = tot.p;
    root->agg_S = tot.S;
}
 
// Split treap into [0, key-1] and [key, end] (by implicit index).
void split(Node* root, int key, Node*& left, Node*& right){
    if(!root){
        left = right = nullptr;
        return;
    }
    int leftSize = getSize(root->l);
    if(key <= leftSize){
        split(root->l, key, left, root->l);
        right = root;
        recalc(right);
    } else {
        split(root->r, key - leftSize - 1, root->r, right);
        left = root;
        recalc(left);
    }
}
 
// Merge two treaps.
Node* merge(Node* left, Node* right){
    if(!left || !right) return left ? left : right;
    if(left->priority < right->priority){
        left->r = merge(left->r, right);
        recalc(left);
        return left;
    } else {
        right->l = merge(left, right->l);
        recalc(right);
        return right;
    }
}
 
// Insert a node at position pos.
Node* insert(Node* root, int pos, Node* node){
    Node *L, *R;
    split(root, pos, L, R);
    return merge(merge(L, node), R);
}
 
// Insert at beginning (position 0)
void insertFront(Node*& root, Node* node){
    root = insert(root, 0, node);
}
 
// Get node at index pos (0-indexed in simulation order).
Node* getNode(Node* root, int pos){
    if(!root) return nullptr;
    int leftSize = getSize(root->l);
    if(pos < leftSize) return getNode(root->l, pos);
    else if(pos == leftSize) return root;
    else return getNode(root->r, pos - leftSize - 1);
}
 
// Given a current x (room number), we “simulate” the reverse–process over a block of ops (stored in the treap).
// This function returns (and updates x) the number of consecutive events (starting from the beginning of simulation order)
// that are “fully applicable” (i.e. they do not capture the guest).
int countApplicable(Node* root, long long &x){
    if(!root) return 0;
    int cnt = 0;
    // First, process the left subtree (which comes earlier in simulation order).
    if(root->l){
        Transform Ltr = identityTransform();
        Ltr.p = root->l->agg_p;
        Ltr.S = root->l->agg_S;
        long long saved = x;
        if(applyTransform(Ltr, x))
            cnt += getSize(root->l);
        else {
            return countApplicable(root->l, x);
        }
    }
    // Now process the current node.
    Transform cur;
    cur.p = root->op_p;
    cur.S = root->op_S;
    if(applyTransform(cur, x)){
        cnt++;
    } else {
        return cnt;
    }
    // Finally, process the right subtree.
    cnt += countApplicable(root->r, x);
    return cnt;
}
 
// --- Global transformation for type 2 queries ---
// At grand opening every room is occupied by group 0; the global transformation is identity: (A=1,B=0).
// Then, for each type 1 query:
//   - For a finite arrival “1 k” (with k>=1): existing guests shift: x -> x+k, so global (A,B) becomes (A, B+k).
//   - For an infinite arrival “1 0”: existing guests get doubled: x -> 2*x, so (A,B) becomes (2*A, 2*B).
 
long long globA = 1, globB = 0;
 
// For each group we record the global transformation that was in effect at the moment of its creation.
// (For new groups the op itself “inserts” guests into empty rooms so that they do NOT get the effect of the current op.)
struct GroupInfo {
    long long A; // creation transform A
    long long B; // creation transform B
    bool infinite; // if true, the group was created by an infinite op (its initial assignment is 1,2,3,... odd => arithmetic progression with a=1,d=2)
                  // if false, its initial assignment is a finite block: arithmetic progression with a=0,d=1.
};
 
vector<GroupInfo> groupInfo; // indexed by group number
int curGroup = 1; // group0 already exists (grand opening)
 
// Modular exponentiation (for computing inverses mod MOD).
long long modExp(long long base, long long exp, long long mod){
    long long result = 1 % mod;
    base %= mod;
    while(exp > 0){
        if(exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
 
// --- Main function ---
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int Q;
    cin >> Q;
    groupInfo.resize(Q+5);
    // Group 0 (grand opening): creation transform is identity.
    groupInfo[0].A = 1;
    groupInfo[0].B = 0;
    groupInfo[0].infinite = false; // treat group 0 as coming from grand opening.
 
    // treap for reverse simulation (for type 3 queries) initially empty.
    Node* treapRoot = nullptr;
 
    // Process queries one–by–one.
    for (int qi = 0; qi < Q; qi++){
        int t;
        cin >> t;
        if(t == 1){
            // Type 1 query: arrival event.
            long long k;
            cin >> k;
            if(k > 0){
                // Finite arrival.
                // First, update global transformation for existing guests: x -> x + k.
                globB = (globB + k) % MOD;
                // New group: record its creation transform as the (updated) global transform.
                groupInfo[curGroup].A = globA;
                groupInfo[curGroup].B = globB;
                groupInfo[curGroup].infinite = false;
                // In the reverse–simulation treap, this op gives transform (0,k).
                Node* newNode = new Node(0, k, curGroup);
                insertFront(treapRoot, newNode);
                curGroup++;
            } else {
                // Infinite arrival (k == 0).
                globA = (globA * 2) % MOD;
                globB = (globB * 2) % MOD;
                groupInfo[curGroup].A = globA;
                groupInfo[curGroup].B = globB;
                groupInfo[curGroup].infinite = true;
                // In the treap, infinite op is represented as (1,0).
                Node* newNode = new Node(1, 0, curGroup);
                insertFront(treapRoot, newNode);
                curGroup++;
            }
        } else if(t == 2){
            // Type 2 query: "2 g x"
            int g; long long x;
            cin >> g >> x;
            // Let current global transform be (globA, globB) and group g was created when the global transform was (A_create, B_create).
            // Then the effective transformation on group g is:
            //   A_eff = globA * inv(A_create) mod MOD,
            //   B_eff = (globB - B_create * A_eff) mod MOD.
            long long Aeff = (globA * modExp(groupInfo[g].A, MOD-2, MOD)) % MOD;
            long long Beff = (globB - (groupInfo[g].B * Aeff) % MOD + MOD) % MOD;
            // New guests:
            //  - If group was created by a finite op, they were assigned rooms 0,1,2,… (i.e. arithmetic progression with a=0,d=1)
            //  - If group was created by an infinite op, they were assigned odd numbers (i.e. arithmetic progression with a=1,d=2)
            long long a0, d;
            if(groupInfo[g].infinite){
                a0 = 1;
                d = 2;
            } else {
                a0 = 0;
                d = 1;
            }
            // The x-th smallest room in the group (x is 1-indexed) is: a0 + (x-1)*d.
            long long initVal = (a0 + ((x-1) % MOD) * d) % MOD;
            long long ans = (Beff + (Aeff * initVal) % MOD) % MOD;
            cout << ans % MOD << "\n";
        } else if(t == 3){
            // Type 3 query: "3 x" -> which group occupies room x?
            long long room;
            cin >> room;
            long long curVal = room;
            int applied = countApplicable(treapRoot, curVal);
            // If all arrival events (stored in the treap) are “applied” then no event captured the guest,
            // so the guest is from group 0.
            if(applied == getSize(treapRoot))
                cout << 0 << "\n";
            else {
                Node* capt = getNode(treapRoot, applied);
                cout << capt->group << "\n";
            }
        }
    }
    return 0;
}
