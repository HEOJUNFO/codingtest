# [Diamond V] Measuring Problem Difficulty - 6626 

[문제 링크](https://www.acmicpc.net/problem/6626) 

### 성능 요약

메모리: 8440 KB, 시간: 112 ms

### 분류

자료 구조, 분할 정복, 다이나믹 프로그래밍, 세그먼트 트리, 정렬, 스위핑

### 제출 일자

2025년 2월 14일 13:16:32

### 문제 설명

<p>If you are the lucky one to advance to the ACM-ICPC World Finals, one of the situations you will face is the world finals competition itself. Wait, isn’t that the main reason to go there?</p>

<p>In the beginning of each ACM-ICPC competition, there are two separate goals each team tries to accomplish:</p>

<ol>
	<li>among the given set of problems, find the easiest one,</li>
	<li>solve that problem as fast as possible.</li>
</ol>

<p>To evaluate the performance of all teams in detail, we want to test your abilities for each of these two goals separately. This problem deals with the former goal (finding the easiest problem), the latter one (solving it) is analyzed in another problem (easy).</p>

<p>The main trouble with comparing problem difficulty is that the opinions of different people may vary. To satisfy everyone, we need to find some consensus. Let’s start with determining all problems on which the opinions agree.</p>

<p>Your team is given a set of ICPC problems. Each team member sorts all of the problems in the order of their expected difficulty. Then we want to find all pairs of problems such that their relative order is the same according to all given orderings.</p>

### 입력 

 <p>The input contains several tasks. Each task starts by one line containing single integer N, 2 ≤ N ≤ 150 000, the number of problems to consider.</p>

<p>After that, there are three blocks, each of them describing opinion of one team member. (ICPC teams have three members, right?) Every block specifies an arbitrary permutation of N numbers 1 ...N representing the problems. You should know from the school that the word “permutation” means each of the numbers will appear exactly once in each block.</p>

<p>Each block starts on a new line. For presentation reasons, the numbers inside a block may be split among any number of lines. If there are more than one number per line, they will be separated by at least one space. Empty lines may occur before and after blocks. The last task is followed by a line containing zero.</p>

### 출력 

 <p>For each task, print one line containing a single integer number: the number of all pairs of problems, whose mutual order is the same in all three permutations.</p>

<p>Please note that the result may be as high as N·(N−1)/2 and may therefore exceed 2<sup>32</sup>.</p>

