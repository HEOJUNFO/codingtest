#include <bits/stdc++.h>
using namespace std;

struct Participant {
    string name;
    long long score;
    int isHidden;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N; cin >> N;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 모든 줄을 이어붙여 하나의 JSON 문자열로 만든다.
    string json;
    for (int i = 0; i < N; i++) {
        string line;
        getline(cin, line);
        json += line;
    }

    // 정규 표현식을 이용해 {"name":"이름","score":점수,"isHidden":비공개} 패턴을 추출한다.
    // 이름: ([a-z]+) 처럼 간단히 할 수도 있으나 문제 조건 상 영어 소문자 알파벳 1~10자
    // 여기서는 단순히 ([a-z]+)로 처리
    // 점수: (\d+)
    // 비공개: ([0-1]) 0 또는 1
    // 패턴 예: {"name":"aym","score":9815,"isHidden":0}
    // 참고: 문자열 내부에 ", 가 포함되니 \" 형태로 이스케이프
    std::regex pattern("\\{\"name\":\"([a-z]+)\",\"score\":(\\d+),\"isHidden\":(\\d)\\}");
    std::smatch match;

    vector<Participant> participants;

    string::const_iterator searchStart(json.cbegin());
    while (regex_search(searchStart, json.cend(), match, pattern)) {
        Participant p;
        p.name = match[1].str();
        p.score = stoll(match[2].str());
        p.isHidden = stoi(match[3].str());
        participants.push_back(p);
        searchStart = match.suffix().first;
    }

    // 정렬: 점수 내림차순, 점수가 같으면 이름 오름차순(사전순)
    sort(participants.begin(), participants.end(), [](const Participant &a, const Participant &b) {
        if (a.score == b.score) return a.name < b.name;
        return a.score > b.score;
    });

    // 순위 계산
    // 점수가 같으면 같은 순위를 매기고, 다음 다른 점수가 나올 때
    // (지금까지 배치된 사람 수 + 1)로 순위를 갱신
    vector<tuple<int, string, long long, int>> ranking;
    int current_rank = 1;
    long long prev_score = -1;
    int count_same = 0;
    int processed = 0; // 지금까지 처리한 인원 수

    for (auto &p : participants) {
        processed++;
        if (p.score == prev_score) {
            count_same++;
        } else {
            current_rank += count_same;
            count_same = 1; 
        }
        prev_score = p.score;
        ranking.push_back({current_rank, p.name, p.score, p.isHidden});
    }

    // 출력 (isHidden == 0 인 참가자만)
    for (auto &r : ranking) {
        int rank; string name; long long score; int hidden;
        tie(rank, name, score, hidden) = r;
        if (hidden == 0) {
            cout << rank << " " << name << " " << score << "\n";
        }
    }

    return 0;
}
