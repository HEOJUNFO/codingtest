# [Diamond IV] Newspapers - 12830 

[문제 링크](https://www.acmicpc.net/problem/12830) 

### 성능 요약

메모리: 7592 KB, 시간: 1484 ms

### 분류

이분 탐색, 센트로이드, 센트로이드 분할, 자료 구조, 분할 정복, 매개 변수 탐색, 세그먼트 트리, 트리

### 제출 일자

2025년 2월 11일 09:43:48

### 문제 설명

<p>In the beautiful Polish city called Mirkow, there are n intersections which are connected by n − 1 bidirectional streets. It is possible to travel between every pair of intersections using these streets.</p>

<p>Mirek works in Mirkow as a paper boy and he delivers newspapers to the villagers. For every street it is known how many people live there (it is also the number of newspapers he should deliver on that street). Each day he chooses two intersections and visits every house on the shortest path between them. Mirek’s daily salary is proportional to the average number of newspapers delivered on a single road (the number of delivered newspapers divided by the number of traversed roads).</p>

<p>At first, Mirek tried to be smart by delivering newspapers only on the road with the highest number of habitants, but the boss found out what his strategy was and tried to stop it, because too many people were not getting their newspapers. The boss gave Mirek an additional constraint: he had to choose a route containing not less than k roads.</p>

<p>Help Mirek and find the optimal path for him.</p>

<p>Given the description of Mirkow, find a path containing k streets or more with the highest number of delivered newspapers per road.</p>

### 입력 

 <p>On the first line of input there are two integers n and k (1 ≤ n ≤ 50 000, 1 ≤ k ≤ n−1) – the number of intersections in Mirkow and the minimal length of Mirek’s path. On the next n − 1 lines there are descriptions of streets. Each description consists of three integers a, b and c (1 ≤ a < b ≤ n, 0 ≤ c ≤ 10<sup>6</sup>) – they mean that there is a street connecting intersections a and b and on this road there live c people.</p>

<p>You can assume that there exists a path of length k.</p>

### 출력 

 <p>Output one number – the average number of delivered newspapers on a single road in the optimal route. The output will be considered correct if the difference between it and the correct answer is less than 10−6.</p>

