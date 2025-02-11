# [Diamond IV] Points and Rectangles - 16336 

[문제 링크](https://www.acmicpc.net/problem/16336) 

### 성능 요약

메모리: 101120 KB, 시간: 1148 ms

### 분류

자료 구조, 분할 정복, 오프라인 쿼리, 세그먼트 트리

### 제출 일자

2025년 2월 11일 17:22:23

### 문제 설명

<p>You have an empty infinite two-dimensional plane and q queries. There are two types of queries:</p>

<ul>
	<li><1 x y> — add a point with the coordinates (x, y) to the plane.</li>
	<li><2 x<sub>1</sub> y<sub>1</sub> x<sub>2</sub> y<sub>2</sub>> — add a rectangle whose lower left corner has the coordinates (x<sub>1</sub>, y<sub>1</sub>) and the upper right — (x<sub>2</sub>, y<sub>2</sub>). The area of this rectangle can be zero and a rectangle can degenerate into a point.</li>
</ul>

<p>Rectangles and points may overlap, that is, there is not guarantee that the figures are distinct.</p>

<p>In addition, to fulfill these queries, after each of them, you need to print the number of pairs of rectangles and points, in which the point lies on the border or inside the rectangle.</p>

### 입력 

 <p>The first line contains one integer q (1 ≤ q ≤ 10<sup>5</sup>) — the number of queries.</p>

<p>Each of the following q lines contains one query:</p>

<ul>
	<li><1 x y> (1 ≤ x, y ≤ 10<sup>9</sup>) — add a point with the coordinates (x, y) to this plane.</li>
	<li><2 x<sub>1</sub> y<sub>1</sub> x<sub>2</sub> y<sub>2</sub>> (1 ≤ x<sub>1</sub> ≤ x<sub>2</sub> ≤ 10<sup>9</sup>, 1 ≤ y<sub>1</sub> ≤ y<sub>2</sub> ≤ 10<sup>9</sup>) — add a rectangle whose left lower corner has the coordinates (x<sub>1</sub>, y<sub>1</sub>), and the upper right — (x<sub>2</sub>, y<sub>2</sub>).</li>
</ul>

### 출력 

 <p>You need to print out q lines, the i-th line must contain one integer number — the number of pairs of rectangles and points, in which the point lies on the side or inside the rectangle.</p>

