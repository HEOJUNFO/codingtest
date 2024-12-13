#include <string>
#include <vector>
using namespace std;

struct TrieNode {
    int count;
    TrieNode* children[26];
    TrieNode() : count(0) {
        for (int i = 0; i < 26; i++) children[i] = nullptr;
    }
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx]) {
                cur->children[idx] = new TrieNode();
            }
            cur = cur->children[idx];
            cur->count++;
        }
    }

    int getUniquePrefixLength(const string& word) {
        TrieNode* cur = root;
        int length = 0;
        for (char c : word) {
            int idx = c - 'a';
            cur = cur->children[idx];
            length++;
            if (cur->count == 1) {
                // 현재 prefix는 유일
                return length;
            }
        }
        // 끝까지 유일해지지 않으면 전체 길이
        return length;
    }

private:
    TrieNode* root;
};

int solution(vector<string> words) {
    Trie trie;
    for (auto &w : words) {
        trie.insert(w);
    }

    int answer = 0;
    for (auto &w : words) {
        answer += trie.getUniquePrefixLength(w);
    }
    return answer;
}
