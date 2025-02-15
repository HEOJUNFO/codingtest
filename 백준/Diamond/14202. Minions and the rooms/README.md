# [Diamond III] Minions and the rooms - 14202 

[문제 링크](https://www.acmicpc.net/problem/14202) 

### 성능 요약

메모리: 7092 KB, 시간: 332 ms

### 분류

조합론, 자료 구조, 고속 푸리에 변환, 느리게 갱신되는 세그먼트 트리, 수학, 세그먼트 트리

### 제출 일자

2025년 2월 15일 23:33:15

### 문제 설명

<p>N minions live in the basement of Gru’s house and are numbered from 1 to N. There are M rooms with infinite capacity in the basement and each room has two states: available or unavailable. Gru will reverse the state of consecutive rooms every day. Minions can only live in available rooms. And each available room should contain at least one Minion.</p>

<p>Gru is interested in how many different ways to arrange creatures in rooms. At the beginning, all rooms are available. In the following D days, Gru will ask you the number of ways after reversing. Two ways A and B are regarded as the same if for any room in A, there exists a room in B that the creatures in these two rooms have the same set of numbers. In other words, rooms are indistinguishable.</p>

### 입력 

 <p>The first line of the input contains an integer T (T ≤ 10), indicating the number of cases. For each test case:</p>

<p>The first line: three space separated integers N, M and D (1 ≤ M ≤ N,D ≤ 100000). Their meanings are described above.</p>

<p>The following D lines: two space separated integers L and R (1 ≤ L ≤ R ≤ M) denoting the consecutive rooms L, L + 1, ..., R which are reversed by the Gru on that day.</p>

### 출력 

 <p>For each query, output the number of different ways to arrange minions modulo 880803841 as it can be very large.</p>

