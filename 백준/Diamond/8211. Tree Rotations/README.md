# [Diamond IV] Tree Rotations - 8211 

[문제 링크](https://www.acmicpc.net/problem/8211) 

### 성능 요약

메모리: 27584 KB, 시간: 432 ms

### 분류

자료 구조, 세그먼트 트리, 작은 집합에서 큰 집합으로 합치는 테크닉, 트리

### 제출 일자

2025년 2월 10일 16:52:56

### 문제 설명

<p>Byteasar the gardener is growing a rare tree called Rotatus Informatikus. It has some interesting features:</p>

<ul>
	<li>The tree consists of straight branches, bifurcations and leaves. The trunk stemming from the ground is also a branch.</li>
	<li>Each branch ends with either a bifurcation or a leaf on its top end.</li>
	<li>Exactly two branches fork out from a bifurcation at the end of a branch - the left branch and the right branch.</li>
	<li>Each leaf of the tree is labelled with an integer from the range 1…n The labels of leaves are unique.</li>
	<li>With some gardening work, a so called rotation can be performed on any bifurcation, swapping the left and right branches that fork out of it.</li>
</ul>

<p>The corona of the tree is the sequence of integers obtained by reading the leaves' labels from left to right.</p>

<p>Byteasar is from the old town of Byteburg and, like all true Byteburgers, praises neatness and order. He wonders how neat can his tree become thanks to appropriate rotations. The neatness of a tree is measured by the number of inversions in its corona, i.e. the number of pairs (i,j), 1 ≤ i < j ≤ n such that a<sub>i</sub> > a<sub>j</sub> in the corona a<sub>1</sub>,a<sub>2</sub>,…,a<sub>n</sub>.</p>

<p><img alt="" src="https://onlinejudgeimages.s3.amazonaws.com/problem/8210/1.gif" style="height:123px; width:337px"></p>

<p>The original tree (on the left) with corona 3,1,2 has two inversions. A single rotation gives a tree (on the right) with corona 1,3,2, which has only one inversion. Each of these two trees has 5 branches.</p>

<p>Write a program that determines the minimum number of inversions in the corona of Byteasar's tree that can be obtained by rotations.</p>

### 입력 

 <p>In the first line of the standard input there is a single integer n (2 ≤ n ≤ 200,000) that denotes the number of leaves in Byteasar's tree. Next, the description of the tree follows. The tree is defined recursively:</p>

<ul>
	<li>if there is a leaf labelled with p (1 ≤ p ≤ n) at the end of the trunk (i.e., the branch from which the tree stems), then the tree's description consists of a single line containing a single integer p,</li>
	<li>if there is a bifurcation at the end of the trunk, then the tree's description consists of three parts:
	<ul>
		<li>the first line holds a single number 0,</li>
		<li>then the description of the left subtree follows (as if the left branch forking out of the bifurcation was its trunk),</li>
		<li>and finally the description of the right subtree follows (as if the right branch forking out of the bifurcation was its trunk).</li>
	</ul>
	</li>
</ul>

<p>In tests worth at least 30% of the points it additionally holds that n ≤ 5,000.</p>

### 출력 

 <p>In the first and only line of the standard output a single integer is to be printed: the minimum number of inversions in the corona of the input tree that can be obtained by a sequence of rotations.</p>

