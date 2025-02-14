# [Diamond V] Monotonicity 2 - 8195 

[문제 링크](https://www.acmicpc.net/problem/8195) 

### 성능 요약

메모리: 62372 KB, 시간: 276 ms

### 분류

자료 구조, 다이나믹 프로그래밍, 세그먼트 트리

### 제출 일자

2025년 2월 14일 21:54:44

### 문제 설명

<p>For an integer sequence a<sub>1</sub>,a<sub>2</sub>,…,a<sub>n</sub> we define its monotonicity scheme as the sequence s<sub>1</sub>,s<sub>2</sub>,…,s<sub>n-1</sub> of symbols <, > or -. The symbol s<sub>i</sub> represents the relation between a<sub>i</sub> and a<sub>i+1</sub>. For example, the monotonicity scheme of the sequence 2,4,3,3,5,3 is <, >, =, <, >.</p>

<p>We say that an integer sequence b<sub>1</sub>,b<sub>2</sub>,…,b<sub>n+1</sub> with monotonicity scheme s<sub>1</sub>,s<sub>2</sub>,…,s<sub>n</sub>, realizes another monotonicity scheme s’<sub>1</sub>,s’<sub>2</sub>,…,s’<sub>k</sub> if for every i=1,2,…,n it holds that s<sub>i</sub>=s'<sub>((i-1) mod k) + 1</sub>. In other words, the sequence s<sub>1</sub>,s<sub>2</sub>,…,s<sub>n</sub> can be obtained by repeating the sequence  s’<sub>1</sub>,s’<sub>2</sub>,…,s’<sub>k</sub> and removing appropriate suffix from that repetition. For example, the sequence 2,4,3,3,5,3 realizes each and every one of the following schemes:</p>

<ul>
	<li><,>,=</li>
	<li><,>,=,<,></li>
	<li><,>,=,<,>,<,<,=</li>
	<li><,>,=,<,>,=,>,></li>
</ul>

<p>as well as many others.</p>

<p>An integer sequence a<sub>1</sub>,a<sub>2</sub>,…,a<sub>n</sub> and a monotonicity scheme s<sub>1</sub>,s<sub>2</sub>,…,s<sub>k</sub> are given. Your task is to find the longest subsequence a<sub>i<sub>1</sub></sub>,a<sub>i<sub>2</sub></sub>,…,a<sub>i<sub>m</sub></sub> (1 ≤ i<sub>1</sub> < i<sub>2</sub> < … < i<sub>m</sub> ≤ n) of the former that realizes the latter.</p>

### 입력 

 <p>The first line of the standard input holds two integers n and k (1 ≤ n ≤ 500,000, 1 ≤ k ≤ 500,000), separated by a single space, denoting the lengths of the sequences (a<sub>i</sub>) and monotonicity scheme (s<sub>j</sub>) respectively.</p>

<p>The second input line gives the sequence (a<sub>i</sub>), i.e, it holds n integers ai separated by single spaces (1 ≤ a<sub>i</sub> ≤ 1,000,000).</p>

<p>Finally, the third lines gives the monotonicity scheme (s<sub>j</sub>), i.e., it holds k symbols sj of the form <, > or = separated by single spaces.</p>

### 출력 

 <p>In the first line of the standard output your program should print out a single integer m, the maximum length of a subsequence of a<sub>1</sub>,a<sub>2</sub>,…,a<sub>n</sub> that realizes the scheme s<sub>1</sub>,s<sub>2</sub>,…,s<sub>k</sub>.</p>

<p>In the second line it should print out any such subsequence a<sub>i<sub>1</sub></sub>,a<sub>i<sub>2</sub></sub>,…,a<sub>i<sub>m</sub></sub>, separating its elements by single spaces.</p>

