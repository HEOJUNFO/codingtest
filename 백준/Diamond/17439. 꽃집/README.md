# [Diamond I] 꽃집 - 17439 

[문제 링크](https://www.acmicpc.net/problem/17439) 

### 성능 요약

메모리: 3524 KB, 시간: 960 ms

### 분류

다이나믹 프로그래밍, 이분 탐색, Aliens 트릭, 단조 큐를 이용한 최적화

### 제출 일자

2025년 7월 24일 09:37:36

### 문제 설명

<p>수학이니 과학이니, 컴퓨터니 사람이니, ……. 복잡하고 슬픈 세상에 신물이 난 남서는 새해가 되고 추위가 가실 즈음, 도시를 떠나 조용한 마을에 꽃집을 열었다.</p>

<p>남서는 처음엔 <span style="font-style: italic;">N</span>송이의 꽃을 가격순으로 일렬로 늘어놓았다. 그런데 개점 소식을 듣고 꽃집에 놀러 온 남서의 친구 끄드흐가 남서에게 이렇게 조언했다.</p>

<p>"꽃이 <span style="font-style: italic;">N</span>송이나 되니까 둘러보기가 너무 힘들어. 꽃다발로 묶는 게 어때?"</p>

<p>생각해보니 그랬다. <span style="font-style: italic;">N</span>송이는 너무 많지 않은가! 그래서 남서는 꽃을 꽃다발로 묶어 정리하기로 했다. 남서는 몇 가지 조건을 세웠다.</p>

<ul>
	<li>각각의 꽃을 꼭 한 번씩 사용한다. 따라서 모든 꽃은 반드시 단 하나의 꽃다발에 속한다.</li>
	<li>꽃다발을 만들 땐 가격순으로 진열된 현재 상태에서 인접한 꽃끼리만 묶는다. 즉, 각 꽃다발을 구성하는 꽃의 원래 위치는 연속한 구간을 이루어야 한다.</li>
	<li>꽃다발의 가격은, 그 꽃다발에 묶인 꽃의 가격의 합에, 묶인 꽃의 개수를 곱한 값이다. (그 이유는, 이해하긴 어렵지만 영업 비밀이라고 한다.)</li>
	<li>꽃다발은 총 <span style="font-style: italic;">K</span>개 이하로 만든다. (손님들이 보기 편하게 진열하기 위함이라고 한다.)</li>
</ul>

<p>이제 꽃다발을 어떻게 만들지 고민하던 남서는, 모든 꽃다발의 가격의 합이 꽃다발을 만드는 방법에 따라 달라질 수 있다는 사실을 깨달았다. 남서는 착하기 때문에 모든 꽃다발의 가격의 합이 최소가 되도록 꽃다발을 만들려고 한다.</p>

<p><span style="font-style: italic;">N</span>개의 꽃의 가격이 진열된 순서대로 주어질 때, <span style="font-style: italic;">K</span>개 이하의 꽃다발을 만들어 모든 꽃다발의 가격의 합을 최소화했을 때의 그 합을 구하는 프로그램을 작성하시오.</p>

### 입력 

 <p>첫 번째 줄에 양의 정수 <span style="font-style: italic;">N</span>과 <span style="font-style: italic;">K</span>가 주어진다. (1 ≤ <span style="font-style: italic;">N</span>, <span style="font-style: italic;">K</span> ≤ 50,000)</p>

<p>두 번째 줄에 꽃의 가격을 의미하는 <span style="font-style: italic;">N</span>개의 양의 정수 <span style="font-style: italic;">v</span><sub><span style="font-style: italic;">i</span></sub>가 띄어쓰기를 사이에 두고 주어진다. (1 ≤ <span style="font-style: italic;">v</span><sub><span style="font-style: italic;">i</span></sub> ≤ 50,000)</p>

<p>주어지는 수열 <span style="font-style: italic;">v</span><sub><span style="font-style: italic;">i</span></sub>는 단조 증가 수열임이 보장된다. 즉, 1 ≤ <span style="font-style: italic;">j</span> ≤ <span style="font-style: italic;">N</span>-1를 만족하는 모든 <span style="font-style: italic;">j</span>에 대해, <span style="font-style: italic;">v</span><sub><span style="font-style: italic;">j</span></sub> ≤ <span style="font-style: italic;">v</span><sub><span style="font-style: italic;">j</span>+1</sub>임이 보장된다.</p>

### 출력 

 <p>첫 번째 줄에, <span style="font-style: italic;">K</span>개 이하의 꽃다발을 만들 때 모든 꽃다발의 가격의 합의 최솟값을 출력한다.</p>

