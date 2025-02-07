# [Diamond II] C-algae - 7995 

[문제 링크](https://www.acmicpc.net/problem/7995) 

### 성능 요약

메모리: 6732 KB, 시간: 624 ms

### 분류

자료 구조, 그래프 이론, 해싱, 해시를 사용한 집합과 맵, 수학

### 제출 일자

2025년 2월 7일 11:44:34

### 문제 설명

<p>C-algae is the Byteotians' favourite dish of their national cuisine. C-algae have a very specific structure. An algae consisting of a single cell is a c-algae. Two c-algae K1 and K2, can be combined in either one of the following ways: </p>

<p>by choosing all the cells from both K1 and K2, and all the connections from both K1 and K2,</p>

<p><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/7995/1.png" style="height:107px; width:100px"></p>

<p>by choosing all the cells from both K1 and K2, all the connections from both K1 and K2, and setting further connections: each cell from K1 is connected to every cell from K2.</p>

<p><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/7995/2.png" style="height:100px; width:100px"></p>

<p>As a result we obtain a new c-algae K.</p>

<p>Unfortunately the hostile country of Bitotia has recently started selling algae imitating c-algae. These look so alike that it is hard to tell the difference between a false one and a genuine c-algae. This is the reason why the Byteotian government has asked you to write a programme that would allow verification if a given algae is a c-algae.</p>

<p>Write a programme that:</p>

<ul>
	<li>reads the descriptions of algae from the standard input,</li>
	<li>checks which of them are proper c-algae,</li>
	<li>writes the answer to the standard output.</li>
</ul>

### 입력 

 <p>In the first line of the standard input a single integer k is written, 1 ≤ k ≤ 10, it denotes the number of algae to be examined. Descriptions of k algae are written in the following lines. Each single description is of the following form: in the first line there are two integers written, separated by a single space, n and m, 1 ≤ n ≤ 10,000, 0 ≤ m ≤ 100,000. They denote the number of cells and the number of connections respectively. The cells are numbered from 1 to n. In the following m lines the connections are described - each by two integers a, b, separated by a single space (a≠b, 1 ≤ a,b ≤ n), indicating that the cells a and b are connected. Each connection is specified once.</p>

### 출력 

 <p>k lines should be written to the standard output. In the i’th line one word should be written:</p>

<ul>
	<li>TAK - (i.e. yes in Polish) - if the i’th algae is a proper c-algae,</li>
	<li>NIE - (i.e. no in Polish) - otherwise.</li>
</ul>

