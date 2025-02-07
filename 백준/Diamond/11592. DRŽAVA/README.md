# [Diamond II] DRŽAVA - 11592 

[문제 링크](https://www.acmicpc.net/problem/11592) 

### 성능 요약

메모리: 63960 KB, 시간: 592 ms

### 분류

이분 탐색, 자료 구조, 델로네 삼각분할, 분리 집합, 다이나믹 프로그래밍, 기하학, 그래프 이론, 수학, 최소 스패닝 트리, 정수론, 보로노이 다이어그램

### 제출 일자

2025년 2월 7일 13:27:49

### 문제 설명

<p>A distant country has N cities in it. The elections were just held in this country, so the new prime minister was elected. Currently, there is not a single road in this country, so the prime minister decided to modernize the country by connecting some cities with two-way highways and form counties. Two cities will be located in the same county if it is possible to get to one city from the other using the newly built roads. Each city will be located in exactly one county. Each conty consists of one or more cities.</p>

<p>The cities are represented as points in a two-dimensional coordinate system. The road between two cities is represented as a line segment connecting two points where the cities are located. The length of the road is equal to the length of the line segment in kilometers.</p>

<p>The country is currently suffering from recession, so the prime minister decided that, because of the lack of budget, they will not build roads longer than D kilometers. Additionally, the prime minister is happy about the small things, so he will be happy if, in at least one county, there exists a nonempty subset of cities (it can include all cities in the county) where the total sum of residents is divisible by K. For instance, if K = 4 and there is a county with cities that have 3, 5, 7 residents respectively, the prime minister will be happy because the sum of residents in the first two cities is equal to 8.</p>

<p>Help the prime minister in cutting the costs by determining the minimal D such that the prime minister can build roads and be happy about the small things at the same time.</p>

### 입력 

 <p>The first line of input contains the integers N and K (1 ≤ N ≤ 50 000, 1 ≤ K ≤ 30). Each of the following N lines contains three integers x<sub>i</sub>, y<sub>i</sub>, k<sub>i</sub> (0 ≤ x<sub>i</sub>, y<sub>i</sub>, k<sub>i</sub> ≤ 100 000 000), that represent the x coordinate of the city, the y coordinate and the number of residents in that city, respectively. There will not be two cities with the same coordinates in the input data. Additionally, there will not be a single city with the number of residents divisible by K.</p>

### 출력 

 <p>The first and only line of output must contain the minimal D such that it is possible to build roads with the condition that the prime minister is happy. Output D rounded to 3 decimal places. The input data will be such that there is always a solution.</p>

