# [Gold V] Slots of Fun - 4261 

[문제 링크](https://www.acmicpc.net/problem/4261) 

### 성능 요약

메모리: 34536 KB, 시간: 36 ms

### 분류

브루트포스 알고리즘, 기하학

### 제출 일자

2024년 12월 16일 11:21:22

### 문제 설명

<p>The International Betting Machine company has just issued a new type of slot machine. The machine display consists of a set of identical circles placed in a triangular shape. An example with four rows is shown below. When the player pulls the lever, the machine places a random letter in the center of each circle. The machine pays off whenever any set of identical letters form the vertices of an equilateral triangle. In the example below, the letters ‘a’ and ‘c’ satisfy this condition.</p>

<p><img alt="" src="" style="height:148px; width:175px"></p>

<p>In order to prevent too many payoffs, the electronics in the machine ensures that no more than 3 of any letter will appear in any display configuration.</p>

<p>IBM is manufacturing several models of this machine, with varying number of rows in the display, and they are having trouble writing code to identify winning configurations. Your job is to write that code.</p>

### 입력 

 <p>Input will consist of multiple problem instances. Each instance will start with an integer n indicating the number of rows in the display. The next line will contain n(n + 1)/2 letters of the alphabet (all lowercase) which are to be stored in the display row-wise, starting from the top. For example, the display above would be specified as</p>

<pre>4
abccddadca</pre>

<p>The value of n will be between 1 and 12, inclusive. A line with a single 0 will terminate input.</p>

### 출력 

 <p>For each problem instance, output all letters which form equilateral triangles on a single line, in alphabetical order. If no such letters exist, output “LOOOOOOOOSER!”.</p>

