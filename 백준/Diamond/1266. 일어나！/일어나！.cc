#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <cmath>
#include <cassert>
using namespace std;

#define fastio cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false)
typedef double ld;
typedef pair<ld, ld> Point;

const ld EPSILON = 1e-6;
const int START = 0;
const int INTERSECT = 1;
const int END = 2;

Point operator*(const Point &p, const ld &d) { return { p.first * d, p.second * d}; } 
Point operator-(const Point &p1, const Point &p2) { return Point(p1.first - p2.first, p1.second - p2.second); }
Point operator+(const Point &p1, const Point &p2) { return Point(p1.first + p2.first, p1.second + p2.second); }
ld operator*(const Point &p1, const Point &p2) { return p1.first * p2.second - p1.second * p2.first; }

int K;
ld cur;
bool after_intersection;

bool is_zero(ld d) {
	return fabsl(d) < EPSILON;
}

struct Segment{ 
	Point s;
	Point e;
	int idx;
	ld slope;

	Segment(Point _s, Point _e, int _idx) {
		idx = _idx;
		s = Point(K * _s.first - _s.second, _s.first + K * _s.second);
		e = Point(K * _e.first - _e.second, _e.first + K * _e.second);

		if (e < s) swap(s, e);
		slope = (e.second - s.second) / (e.first - s.first);
	}

	ld eval() const {
		return slope * (cur- s.first) + s.second;
	}

	bool operator<(const Segment& rhs) const {//위아래 순서
		ld ev1 = eval();
		ld ev2 = rhs.eval();
		
		if (!is_zero(ev1 - ev2)) return ev1 < ev2;
		if (slope != rhs.slope) return after_intersection ? slope < rhs.slope : rhs.slope < slope ;
		return idx < rhs.idx;
	}

	Point operator*(const Segment& rhs) const {//교점 구하기
		ld det = (e - s) * (rhs.e - rhs.s);

		if (!det) {
			if (e == rhs.s) return e;
			return s;
		}

		return s + (e - s) * ((rhs.s - s) * (rhs.e - rhs.s)/det);
	}
};

struct Event {
	ld x;
	ld y;
	int type;
	int idx1;
	int idx2;

	Event(Point p, int _type, int _idx) : x(p.first), y(p.second), type(_type), idx1(_idx), idx2(_idx) {}
	Event(Point p, int _type, int _idx1, int _idx2) : x(p.first), y(p.second), type(_type), idx1(_idx1), idx2(_idx2) {}

	bool operator<(const Event &e) const {
		return tie(x, y, type) > tie(e.x, e.y, e.type);
	}
};

int ccw(Point& a, Point& b, Point& c) {
	ld ret = (b - a) * (c - b);
	return is_zero(ret)? 0 : ret > 0 ? 1 : -1; 
}

bool intersect(const Segment& seg1, const Segment& seg2) {
	Point p1 = seg1.s, p2 = seg1.e;
	Point p3 = seg2.s, p4 = seg2.e;

	int p1p2 = ccw(p1, p2, p3) * ccw(p1, p2, p4); 
	int p3p4 = ccw(p3, p4, p1) * ccw(p3, p4, p2); 

	if (p1p2 == 0 && p3p4 == 0) {
		if (p1 > p2) swap(p1, p2); 
		if (p3 > p4) swap(p3, p4);
		
		return (p2 >= p3 && p4 >= p1);
	}

	return p1p2 <= 0 && p3p4 <= 0;
}

vector<Segment> segments;
priority_queue<Event> events;//Q
multiset<Segment> T;
set<pair<int, int>> ans;//서로 교차하는 선분쌍 집합

void push(Point &intersection, int idx1, int idx2) {
	if (idx1 < idx2) swap(idx1, idx2);
	if (ans.find({idx1, idx2}) != ans.end()) return;//이미 Q에 들어갔던 교차점인 경우는 다시 보지 않는다.

	ans.insert({idx1, idx2}); 
	Event e = Event(intersection, INTERSECT, idx1, idx2);
	events.push(e);
}

void bentley_ottmann() {
	for (auto &[s, e, idx, slope] : segments) {
		events.push(Event(s, START, idx));
		events.push(Event(e, END, idx));
	}

	while (!events.empty()) {
		auto [x, y, type, idx1, idx2] = events.top(); 
		events.pop();
		cur = x;

		if (type == START) {//INSERT
			auto seg = segments[idx1];
			auto it = T.insert(seg);
			auto above = next(it);
			auto below = prev(it);

			if (above != T.end() && intersect(*above, *it)) {
				Point intersection = (*above) * (*it);
				push(intersection, it->idx, above->idx);
			}

			if (it != T.begin() && intersect(*below, *it)) {
				Point intersection = (*below) * (*it);
				push(intersection, it->idx, below->idx);
			}
		}
		else if (type == END) {//ERASE
			auto seg = segments[idx1];

			auto it = T.lower_bound(seg);

			auto above = next(it);
			auto below = prev(it);

			if (it != T.begin() && above != T.end() && intersect(*below, *above)) {
				Point intersection = (*below) * (*above);
				push(intersection, above->idx, below->idx);
			}

			T.erase(it);
		}
		else {//INTERSECT
			auto seg1 = segments[idx1];
			auto seg2 = segments[idx2];
			
			if (is_zero(x - seg1.e.first) || is_zero(x - seg2.e.first)) continue;//끝점을 만난 경우

			T.erase(seg1); T.erase(seg2);

			//교차 이후 순서 바꾸기
			after_intersection = true;

			if (seg2.slope < seg1.slope) swap(seg1, seg2);
			
			auto upper = T.insert(seg2);
			auto lower = T.insert(seg1);

			auto above = next(upper);
			auto below = prev(lower);

			if (above != T.end() && intersect(*upper, *above)) {
				Point intersection = (*upper) * (*above);
				push(intersection, upper->idx, above->idx);
			}

			if (lower != T.begin() && intersect(*lower, *below)) {
				Point intersection = (*lower) * (*below);
				push(intersection, lower->idx, below->idx);
			}

			after_intersection = false;
		}
	}
}

int main() {
	fastio;

	int N;
	cin >> N;

	vector<pair<Point, Point>> endpoints;
	set<int> slopes;

	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;

		endpoints.emplace_back(Point(x1, y1), Point(x2, y2));

		if (x1 != x2) slopes.insert((y2 - y1) / (x2 - x1));
	}

	while (slopes.find(K) != slopes.end()) K++;

	for (int i = 0; i < N; i++) segments.emplace_back(endpoints[i].first, endpoints[i].second, i);

	bentley_ottmann();

	cout << ans.size();
}