# [Diamond I] Dynamic Diameter - 17642 

[문제 링크](https://www.acmicpc.net/problem/17642) 

### 성능 요약

메모리: 52208 KB, 시간: 3348 ms

### 분류

자료 구조, 트리, 세그먼트 트리, 분할 정복, 느리게 갱신되는 세그먼트 트리, 오일러 경로 테크닉, 센트로이드 분할

### 제출 일자

2025년 7월 23일 13:51:28

### 문제 설명

<p>You are given a weighted undirected tree on n vertices and a list of q updates. Each update changes the weight of one edge. The task is to output the diameter of the tree after each update.</p>

<p>(The distance between two vertices is the sum of the weights on the unique simple path that connects them. The diameter is the largest of all those distances.)</p>

### 입력 

 <p>The first line contains three space-separated integers n, q and w (2 ≤ n ≤ 100, 000, 1 ≤ q ≤ 100, 000, 1 ≤ w ≤ 20, 000, 000, 000, 000) – the number of vertices in the tree, the number of updates and the limit on the weights of edges. The vertices are numbered 1 through n.</p>

<p>Next, n − 1 lines describing the initial tree follow. The i-th of these lines contains three space-separated integers a<sub>i</sub>, b<sub>i</sub>, c<sub>i</sub> (1 ≤ a<sub>i</sub>, b<sub>i</sub> ≤ n, 0 ≤ c<sub>i</sub> < w) meaning that initially, there is an edge between vertices a<sub>i</sub> and b<sub>i</sub> with weight c<sub>i</sub>. It is guaranteed that these n − 1 lines describe a tree.</p>

<p>Finally, q lines describing queries follow. The j-th of these lines contains two space-separated integers d<sub>j</sub>, e<sub>j</sub> (0 ≤ d<sub>j</sub> < n − 1, 0 ≤ e<sub>j</sub> < w). These two integers are then transformed according to the following scheme:</p>

<ul>
	<li>d'<sub>j</sub> = (d<sub>j</sub> + <em>last</em>) mod (n − 1)</li>
	<li>e'<sub>j</sub> = (e<sub>j</sub> + <em>last</em>) mod w</li>
</ul>

<p>where <em>last</em> is the result of the last query (initially <em>last</em> = 0). Tuple (d'<sub>j</sub> , e'<sub>j</sub>) represents a query which takes the d'<sub>j</sub> + 1-th edge from the input and sets its weight to e'<sub>j</sub>.</p>

### 출력 

 <p>Output q lines. For each i, line i should contain the diameter of the tree after the i-th update.</p>

