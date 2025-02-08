# [Diamond IV] Photo - 5824 

[문제 링크](https://www.acmicpc.net/problem/5824) 

### 성능 요약

메모리: 4376 KB, 시간: 20 ms

### 분류

벨만–포드, 자료 구조, 다이나믹 프로그래밍, 그래프 이론, 세그먼트 트리, 최단 경로

### 제출 일자

2025년 2월 8일 22:56:12

### 문제 설명

<p>Farmer John has decided to assemble a panoramic photo of a lineup of his N cows (1 <= N <= 200,000), which, as always, are conveniently numbered from 1..N.  Accordingly, he snapped M (1 <= M <= 100,000) photos, each covering a contiguous range of cows: photo i contains cows a_i through b_i inclusive.  The photos collectively may not necessarily cover every single cow.</p><p>After taking his photos, FJ notices a very interesting phenomenon: each photo he took contains exactly one cow with spots!  FJ was aware that he had some number of spotted cows in his herd, but he had never actually counted them.  Based on his photos, please determine the maximum possible number of spotted cows that could exist in his herd.  Output -1 if there is no possible assignment of spots to cows consistent with FJ's photographic results.</p>

### 입력 

 <ul><li>Line 1: Two integers N and M.</li><li>Lines 2..M+1: Line i+1 contains a_i and b_i.</li></ul>

### 출력 

 <ul><li>Line 1: The maximum possible number of spotted cows on FJ's farm, or -1 if there is no possible solution.</li></ul>

