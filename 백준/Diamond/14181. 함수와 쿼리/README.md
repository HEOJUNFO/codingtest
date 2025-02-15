# [Diamond III] 함수와 쿼리 - 14181 

[문제 링크](https://www.acmicpc.net/problem/14181) 

### 성능 요약

메모리: 94948 KB, 시간: 1028 ms

### 분류

볼록 껍질을 이용한 최적화, 자료 구조, 세그먼트 트리

### 제출 일자

2025년 2월 15일 23:21:43

### 문제 설명

<p>다음과 같은 함수가 있다.</p>

<ul>
	<li>f(1, j) = a[j], 1 ≤ j ≤ n</li>
	<li>f(i, j) = min(f(i-1, j), f(i-1, j-1)) + a[j], 2 ≤ i ≤ n, i ≤ j ≤ n</li>
</ul>

<p>여기서 a는 길이가 n인 배열이다.</p>

<p>배열 a의 값과 쿼리 x<sub>i</sub>, y<sub>i</sub>가 주어졌을 때, f(x<sub>i</sub>, y<sub>i</sub>)값을 출력하는 프로그램을 작성하시오.</p>

### 입력 

 <p>첫째 줄에 배열 a의 크기 n (1 ≤ n ≤ 10<sup>5</sup>)가 주어지고, 둘째 줄에 배열 a[1], a[2], ..., a[n]이 주어진다. (0 ≤ a[i] ≤ 10<sup>4</sup>)</p>

<p>다음  줄에는 쿼리의 개수 m (1 ≤ m ≤ 10<sup>5</sup>)가 주어지고, 다음 m개의 줄에는 쿼리 x<sub>i</sub>, y<sub>i</sub>가 주어진다. (1 ≤ x<sub>i</sub> ≤ y<sub>i</sub> ≤ n)</p>

### 출력 

 <p>각각의 쿼리마다 f(x<sub>i</sub>, y<sub>i</sub>)를 한 줄에 하나씩 순서대로 출력한다.</p>

