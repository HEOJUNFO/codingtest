# [Diamond IV] Traffic - 3311 

[문제 링크](https://www.acmicpc.net/problem/3311) 

### 성능 요약

메모리: 104132 KB, 시간: 1100 ms

### 분류

그래프 이론, 그래프 탐색, 너비 우선 탐색, 깊이 우선 탐색, 방향 비순환 그래프, 강한 연결 요소, 평면 그래프

### 제출 일자

2025년 7월 22일 11:14:11

### 문제 설명

<p>The center of Gdynia is located on an island in the middle of the Kacza river. Every morning thousands of cars drive through this island from the residential districts on the western bank of the river (using bridge connections to junctions on the western side of the island) to the industrial areas on the eastern bank (using bridge connections from junctions on the eastern side of the island).</p>

<p>The island resembles a rectangle, whose sides are parallel to the cardinal directions. Hence, we view it as an A × B rectangle in a Cartesian coordinate system, whose opposite corners are in points (0, 0) and (A, B).</p>

<p>On the island, there are n junctions numbered from 1 to n. The junction number i has coordinates (x<sub>i</sub>, y<sub>i</sub>). If a junction has coordinates of the form (0, y), it lies on the western side of the island. Similarly, junctions with the coordinates (A, y) lie on the eastern side. Junctions are connected by streets. Each street is a line segment connecting two junctions. Streets can be either unidirectional or bidirectional. No two streets may have a common point (except for, possibly, a common end in a junction). There are are no bridges or tunnels. You should not assume anything else about the shape of the road network. In particular, there can be streets going along the river bank or junctions with no incoming or outgoing streets.</p>

<p>Because of the growing traffic density, the city mayor has hired you to check whether the current road network on the island is sufficient. He asked you to write a program which determines how many junctions on the eastern side of the island are reachable from each junction on the western side.</p>

### 입력 

 <p>The first line of the standard input contains four integers n, m, A and B (1 ≤ n ≤ 300 000, 0 ≤ m ≤ 900 000, 1 ≤ A, B ≤ 10<sup>9</sup>). They denote the number of junctions in the center of Gdynia, the number of streets and dimensions of the island, respectively.</p>

<p>In each of the following n lines there are two integers x<sub>i</sub>, y<sub>i</sub> (0 ≤ x<sub>i</sub> ≤ A, 0 ≤ y<sub>i</sub> ≤ B) describing the coordinates of the junction number i. No two junctions can have the same coordinates.</p>

<p>The next m lines describe the streets. Each street is represented in a single line by three integers c<sub>i</sub>, d<sub>i</sub>, k<sub>i</sub> (1 ≤ c<sub>i</sub>, d<sub>i</sub> ≤ n, c<sub>i</sub> ≠ d<sub>i</sub>, k<sub>i</sub> ∈ {1, 2}). Their meaning is that junctions c<sub>i</sub> and d<sub>i</sub> are connected with a street. If k<sub>i</sub> = 1, then this is a unidirectional street from c<sub>i</sub> to d<sub>i</sub>. Otherwise, the street can be driven in both directions. Each unordered pair {c<sub>i</sub>, d<sub>i</sub>} can appear in the input at most once.</p>

<p>You can assume that there is at least one junction on the western side of the island from which it is possible to reach some junction on the eastern side of the island.</p>

### 출력 

 <p>Your program should write to the standard output one line for each junction on the western side of the island. This line should contain the number of junctions on the eastern side that are reachable from that junction. The output should be ordered according to decreasing y-coordinates of the junctions.</p>

