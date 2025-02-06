# [Diamond IV] Conspiracy - 8202 

[문제 링크](https://www.acmicpc.net/problem/8202) 

### 성능 요약

메모리: 5380 KB, 시간: 1888 ms

### 분류

그래프 이론, 수학

### 제출 일자

2025년 2월 6일 11:59:43

### 문제 설명

<p>Hostile Bitotia launched a sneak attack on Byteotia and occupied a significant part of its territory. The King of Byteotia, Byteasar, intends to organise resistance movement in the occupied area. Byteasar naturally started with selecting the people who will form the skeleton of the movement. They are to be partitioned into two groups: the conspirators who will operate directly in the occupied territory, and the support group that will operate inside free Byteotia.</p>

<p>There is however one issue - the partition has to satisfy the following conditions:</p>

<ul>
	<li>Every pair of people from the support group have to know each other - this will make the whole group cooperative and efficient.</li>
	<li>The conspirators must not know each other.</li>
	<li>None of the groups may be empty, i.e., there has to be at least one conspirator and at least one person in the support group.</li>
</ul>

<p>Byteasar wonders how many ways there are of partitioning selected people into the two groups. And most of all, whether such partition is possible at all. As he has absolutely no idea how to approach this problem, he asks you for help.</p>

### 입력 

 <p>The first line of the standard input holds one integer n (2 ≤ n ≤ 5,000), denoting the number of people engaged in forming the resistance movement. These people are numbered from 1 to n(for the sake of conspiracy!). The n lines that follow describe who knows who in the group. The i-th of these lines describes the acquaintances of the person  with a sequence of integers separated by single spaces. The first of those numbers, k<sub>i</sub> (0 ≤ k<sub>i</sub> ≤ n-1), denotes the number of acquaintances of the person i. Next in the line there are k<sub>i</sub> integers a<sub>i,1</sub>,a<sub>i,2</sub>,…,a<sub>i,k<sub>i</sub></sub> - the numbers of i’s acquaintances. The numbers a<sub>ij</sub> are given in increasing order and satisfy 1 ≤ a<sub>ij</sub> ≤ n, a<sub>ij</sub> ≠ i. You may assume that if x occurs in the sequence a<sub>i</sub>(i.e., among i’s acquaintances), then also i occurs in the sequence a<sub>x</sub> (i.e., among x’s acquaintances).</p>

### 출력 

 <p>In the first and only line of the standard output your program should print out one integer: the number of ways to partition selected people into the conspirators and the support group. If there is no partition satisfying aforementioned conditions, then 0 is obviously the right answer.</p>

