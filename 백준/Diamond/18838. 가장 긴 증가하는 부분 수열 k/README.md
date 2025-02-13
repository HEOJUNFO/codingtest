# [Diamond IV] 가장 긴 증가하는 부분 수열 k - 18838 

[문제 링크](https://www.acmicpc.net/problem/18838) 

### 성능 요약

메모리: 11924 KB, 시간: 64 ms

### 분류

자료 구조, 다이나믹 프로그래밍, 가장 긴 증가하는 부분 수열: O(n log n), 세그먼트 트리

### 제출 일자

2025년 2월 13일 11:16:40

### 문제 설명

<p>N개의 정수로 이루어진 수열 A<sub>1</sub>, A<sub>2</sub>, ..., A<sub>N</sub>에서, 가장 긴 증가하는 부분 수열(LIS)의 길이를 L이라고 하자. LIS는 하나 또는 그 이상 있을 수 있다. 모든 LIS를 사전 순으로 정렬했을 때, K번째 오는 수열을 구해보자.</p>

<p>두 LIS A<sub>i<sub>1</sub></sub>, A<sub>i<sub>2</sub></sub>, ..., A<sub>i<sub>L</sub></sub>와 A<sub>j<sub>1</sub></sub>, A<sub>j<sub>2</sub></sub>, ..., A<sub>j<sub>L</sub></sub>이 있을 때, i<sub>k</sub> ≠ j<sub>k</sub>를 만족하는 k가 하나라도 존재하면 다른 LIS이다.</p>

### 입력 

 <p>첫째 줄에 N과 K가 주어진다. 둘째 줄에 공백으로 구분된 A<sub>1</sub>, A<sub>2</sub>, ..., A<sub>N</sub>이 주어진다. </p>

### 출력 

 <p>K번째 LIS를 공백으로 구분해서 출력한다. K번째 LIS가 없을 때는 -1을 출력한다.</p>

