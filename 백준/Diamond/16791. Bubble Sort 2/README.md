# [Diamond IV] Bubble Sort 2 - 16791 

[문제 링크](https://www.acmicpc.net/problem/16791) 

### 성능 요약

메모리: 54700 KB, 시간: 3712 ms

### 분류

값 / 좌표 압축, 자료 구조, 세그먼트 트리

### 제출 일자

2025년 2월 12일 09:26:06

### 문제 설명

<p>Bubble sort is an algorithm to sort a sequence. Let’s say we are going to sort a sequence A<sub>0</sub>, A<sub>1</sub>, . . . , A<sub>N−1</sub> of length N in non-decreasing order. Bubble sort swaps two adjacent numbers when they are not in the correct order. Swaps are done by repeatedly passing through the sequence. Precisely speaking, in a pass, we swap A<sub>i</sub> and A<sub>i+1</sub> if A<sub>i</sub> > A<sub>i+1</sub>, for i = 0, 1, . . . , N − 2 in this order. It is known that any sequence can be sorted in non-decreasing order by some passes. For a sequence A, we define the number of passes by bubble sort as the number of passes needed to sort A using the above algorithm.</p>

<p>JOI-kun has a sequence A of length N. He is going to process Q queries of modifying values of A. To be specific, in the (j + 1)-th query (0 ≤ j ≤ Q − 1), the value of A<sub>X<sub>j</sub></sub> is changed into V<sub>j</sub>.</p>

<p>JOI-kun wants to know the number of passes by bubble sort for the sequence after processing each query.</p>

### 입력 

 <ul>
	<li>line 1: N Q</li>
	<li>line 2: A<sub>0</sub> A<sub>1</sub> . . . A<sub>N−1</sub></li>
	<li>line 3 + j (0 ≤ j ≤ Q − 1): X<sub>j</sub> V<sub>j</sub></li>
</ul>

### 출력 

 <ul>
	<li>line 1 + j (0 ≤ j ≤ Q − 1): the number of passes by bubble sort for the sequence right after processing (j + 1)-th query.</li>
</ul>

