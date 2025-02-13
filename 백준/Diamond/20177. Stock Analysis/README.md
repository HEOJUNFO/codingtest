# [Diamond IV] Stock Analysis - 20177 

[문제 링크](https://www.acmicpc.net/problem/20177) 

### 성능 요약

메모리: 170760 KB, 시간: 1828 ms

### 분류

자료 구조, 다차원 세그먼트 트리, 오프라인 쿼리, 세그먼트 트리

### 제출 일자

2025년 2월 13일 17:21:46

### 문제 설명

<p>SY Company wants to analyze a stock. The fluctuation value, which is the difference in stock prices for two consecutive days, is the most frequently used data for the time-series analysis of the stock. It is important to utilize the largest sum of the contiguous fluctuation values. However, using the largest contiguous sum as a key indicator could be risky. As an alternative, the company utilizes the largest contiguous sum that is not greater than a predetermined value <em>U</em> in a specified period [<em>S</em>, <em>E</em>] from <em>S</em> to <em>E</em>. The company wants to process such queries as fast as possible, where a query is defined as a predetermined value <em>U</em> and a period [<em>S</em>, <em>E</em>].</p>

<p>Given a collection of <em>n</em> recent fluctuation values for some stock and <em>m</em> queries {(<em>S</em><sub>1</sub>, <em>E</em><sub>1</sub>, <em>U</em><sub>1</sub> ),… , (<em>S<sub>m</sub></em>, <em>E<sub>m</sub></em>, <em>U<sub>m</sub></em>)}, write a program to find the largest sum of contiguous fluctuation values that is less than or equal to <em>U<sub>i</sub></em> in the period [<em>S<sub>i</sub></em>, <em>E<sub>i</sub></em>]for each query (<em>S<sub>i</sub></em>, <em>E<sub>i</sub></em>, <em>U<sub>i</sub></em>).</p>

### 입력 

 <p>Your program is to read from standard input. The input starts with a line containing two integers, <em>n</em> and <em>m</em>, representing the number of fluctuation values and the number of queries respectively, where 1 ≤ <em>n</em> ≤ 2,000 and 1 ≤ <em>m</em> ≤ 200,000. The next line contains <em>n</em> integers representing <em>n</em> fluctuation values, which are numbered in chronological order from 1 to <em>n</em>. Each of the following <em>m</em> lines represents a query that consists of three integers, <em>S<sub>i</sub></em>, <em>E<sub>i</sub></em>, and <em>U<sub>i</sub></em>, where [<em>S<sub>i</sub></em>, <em>E<sub>i</sub></em>] is the period from <em>S<sub>i</sub></em> to <em>E<sub>i</sub></em> over which the fluctuation values should be considered and <em>U<sub>i</sub></em> is the value that the contiguous sum should not exceed. Note that all fluctuation values are between −10<sup>9</sup> and 10<sup>9</sup> , 1 ≤ <em>S<sub>i</sub></em> ≤ <em>E<sub>i</sub></em> ≤ <em>n</em> and −10<sup>14</sup> ≤ <em>U<sub>i</sub></em> ≤ 10<sup>14</sup> for <em>i</em> = 1, … , <em>m</em>.</p>

### 출력 

 <p>Your program is to write to standard output. Print exactly <em>m</em> lines. The <em>i</em>-th line should contain the largest sum of contiguous fluctuation values that does not exceed <em>U<sub>i</sub></em> and in the period [<em>S<sub>i</sub></em>, <em>E<sub>i</sub></em>] for the <em>i</em>-th query. Note that every contiguous sum is the sum of one or more consecutive fluctuation values. When it is impossible to find such the sum, the program should print <code>NONE</code>.</p>

