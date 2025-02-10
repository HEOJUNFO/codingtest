# [Diamond IV] Pyramid Base 2 - 11552 

[문제 링크](https://www.acmicpc.net/problem/11552) 

### 성능 요약

메모리: 66180 KB, 시간: 1372 ms

### 분류

자료 구조, 느리게 갱신되는 세그먼트 트리, 매개 변수 탐색, 세그먼트 트리, 스위핑

### 제출 일자

2025년 2월 10일 21:42:13

### 문제 설명

<p>You have been asked to find the largest affordable location for constructing a new pyramid.  In order to help you decide, you have been provided with a survey of the available land which has been conveniently divided into an M by N grid of square cells. The base of the pyramid must be a square with sides parallel to those of the grid.</p>

<p>The survey has identified a set of P possibly overlapping obstacles, which are described as rectangles in the grid with sides parallel to those of the grid. In order to build the pyramid, all the cells covered by its base must be cleared of any obstacles. Removing the ith obstacle has a cost Ci. Whenever an obstacle is removed, it must be removed completely, that is, you cannot remove only part of an obstacle. Also, please note that removing an obstacle does not affect any other obstacles that overlap it.</p>

<p>Write a program that, given the dimensions M and N of the survey, the description of the P obstacles, the cost of removing each of the obstacles, and the budget B you have, finds the maximum possible side length of the base of the pyramid such that the total cost of removing obstacles does not exceed B.</p>

### 입력 

 <p>Your program must read from the standard input the following data:</p>

<ul>
	<li>Line 1 contains two integers separated by a single space that represent M and N respectively. (1 ≤ M, N ≤ 1,000,000)</li>
	<li>Line 2 contains the integer B, the maximum cost you can afford (i.e., your budget). (0 < B ≤ 2,000,000,000)</li>
	<li>Line 3 contains the integer P, the number of obstacles found in the survey. (1 ≤ P ≤ 30,000)</li>
	<li>Each of the next P lines describes an obstacle. The ith of these lines describes the ith obstacle. Each line consists of 5 integers: X<sub>i1</sub>, Y<sub>i1</sub>, X<sub>i2</sub>, Y<sub>i2</sub>, and C<sub>i</sub> separated by single spaces. They represent respectively the coordinates of the bottommost leftmost cell of the obstacle, the coordinates of the topmost rightmost cell of the obstacle, and the cost of removing the obstacle.    The bottommost leftmost cell on the grid has coordinates (1, 1) and the topmost rightmost cell has coordinates (M, N). (1 ≤ X<sub>i1</sub> ≤ X<sub>i2</sub> ≤ M, 1 ≤ Y<sub>i1</sub> ≤ Y<sub>i2</sub> ≤ N, 1 ≤ C<sub>i</sub> ≤ 7,000)</li>
</ul>

### 출력 

 <p>Your program must write to the standard output a single line containing one integer, the maximum possible side length of the base of the pyramid that can be prepared. If it is not possible to build any pyramid, your program should output the number 0.</p>

