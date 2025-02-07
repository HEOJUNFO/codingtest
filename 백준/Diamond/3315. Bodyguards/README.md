# [Diamond V] Bodyguards - 3315 

[문제 링크](https://www.acmicpc.net/problem/3315) 

### 성능 요약

메모리: 26956 KB, 시간: 180 ms

### 분류

자료 구조, 그리디 알고리즘, 수학, 누적 합

### 제출 일자

2025년 2월 8일 00:31:23

### 문제 설명

<p>Have you ever met the members of the ten European royal houses, the Argentinian football team including their coach Diego Maradona, or all of the Turing and Fields medal prize winners? We have invited many celebrities from all over the world to the CEOI 2010 closing ceremony. Unfortunately, very few of them responded to our invitation, and those who did, politely rejected. Nevertheless, do not forget to bring your camera to the ceremony, one never knows who might turn up!</p>

<p>As you can imagine, the security of the guests is of utmost importance. The problem is how to seat their bodyguards in the audience so that maximal security is guaranteed.</p>

<p>The auditorium contains many seats, arranged into a large grid. Based on the safety regulations a security expert has determined necessary bodyguard counts for each row and each column of the auditorium.</p>

<p>You are given the required number of bodyguards for each row and column of the auditorium. This information is given in a compressed form as explained below. Determine whether it is possible to place the bodyguards in such a way that in each row and each column we would have exactly the required number of bodyguards.</p>

<p>Assume that the auditorium is initially empty, i.e., you may seat bodyguards wherever you wish. Each seat may only be occupied by a single bodyguard.</p>

### 입력 

 <p>The input begins with the description of the rows. The first line of the input contains one positive integer R: the number of groups of rows. R lines follow. Each of these lines contains 2 positive integers: the required number of bodyguards in each row of the group and the number of rows that form the group.</p>

<p>followed by the description of column groups. The next line contains one positive integer C: the number of groups of columns. C lines follow. Each of these lines contains 2 positive integers: the required number of bodyguards in each column of the group and the number of columns that form the group.</p>

### 출력 

 <p>Output a single line with the number "1" if the constraints are satisfiable and the number "0" otherwise (quotes for clarity).</p>

