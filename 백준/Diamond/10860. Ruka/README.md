# [Diamond III] Ruka - 10860 

[문제 링크](https://www.acmicpc.net/problem/10860) 

### 성능 요약

메모리: 12184 KB, 시간: 1096 ms

### 분류

자료 구조, 세그먼트 트리

### 제출 일자

2025년 2월 15일 22:18:25

### 문제 설명

<p>Stjepan is programming a robot arm that can use a chalk to draw on a blackboard located in a standard coordinate plane (x coordinate increases to the right, y coordinate increases upwards).</p>

<p>The robot arm’s plan is an array of precisely N vectors (x<sub>1</sub>, y<sub>1</sub>), . . . , (x<sub>N</sub>, y<sub>N</sub>) where each x<sub>i</sub> and y<sub>i</sub> are even integers. The plan is executed by the robot arm starting from point (1, 1) and making N steps: in the i th step, the arm moves the chalk from the current point (x, y) straight to the point (x + x<sub>i</sub>, y + y<sub>i</sub>). Therefore, the robot arm is drawing a kind of a broken line in the coordinate plane, and the segments of that broken line are the given vectors.</p>

<p>While Stjepan is devising and changing his plan, sometimes he wants to know how many times the chalk will go over the coordinate axes. Write a programme that will simulate the process of changing the plan and that will give answers to Stjepan’s queries.</p>

<p style="text-align: center;"><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/10860/1.png" style="height:520px; width:501px"></p>

<p style="text-align: center;">The layout of the plan on each ‘Q‘ command in the second test case. The dotted line marks the segment that was most recently changed.</p>

<p>Let us assume that Stjepan wrote down his plan in a text file that consists of N lines – the ith line contains the vector (x<sub>i</sub>, y<sub>i</sub>). Initially, Stjepan’s cursor is located at the first line of the file. Your programme should simulate the following commands:</p>

<ul>
	<li>‘B‘ – the cursor moves to the previous line (if it’s already located at the first line, nothing happens).</li>
	<li>‘F‘ – the cursor moves to the next line (if it’s already located at the last line, nothing happens).</li>
	<li>‘C nx ny‘ – where nx and ny are even integers. The row of the file where the cursor is located at changes in a way that the current vector is replaced with the vector (nx, ny).</li>
	<li>‘Q‘ – you need to output how many times the dotted line which is described by the current plan went over the coordinate axes. If the dotted line goes through the starting point, then we count that as two times going over the coordinate axes.</li>
</ul>

### 입력 

 <p>The first line of input contains the integer N – the number of vectors in the plan. The i<sup>th</sup> of the following N lines contains two even integers x<sub>i</sub> and y<sub>i</sub> separated by a single space – the coordinates of the i<sup>th</sup> vector in the initial plan.</p>

<p>The following line contains the integer M – the number of commands which execution you need to simulate. Each of the following M lines contains a single command. A command is either one of the uppercase letters ‘B’, ‘F’ or ‘Q’ or an expression in the form ‘C nx ny’ where nx and ny are even integers described in the task.</p>

### 출력 

 <p>For each ‘Q‘ command from the input, you must output its result in a single line. The results need to be printed in the order which the commands appear in the input.</p>

