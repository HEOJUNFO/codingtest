# [level 5] 중력 작용 - 77887 

[문제 링크](https://school.programmers.co.kr/learn/courses/30/lessons/77887?language=cpp) 

### 성능 요약

메모리: 61.5 MB, 시간: 285.72 ms

### 구분

코딩테스트 연습 > 월간 코드 챌린지 시즌2

### 채점결과

정확성: 100.0<br/>합계: 100.0 / 100.0

### 제출 일자

2024년 12월 19일 11:27:42

### 문제 설명

<p>1부터 n까지 번호가 하나씩 붙은 n개의 노드를 갖는 트리가 주어집니다. 각 노드에는 값이 하나씩 들어 있으며, 이 트리의 루트 노드는 1번 노드입니다. 당신은 이 트리에 대해 다음과 같은 쿼리 두 종류를 처리하면 됩니다.</p>

<ul>
<li>1번 쿼리: 정수 u가 주어집니다. u번 노드의 서브 트리의 모든 노드의 값의 합을 구해야 합니다.</li>
<li>2번 쿼리: 정수 u, w가 주어집니다. u번 노드의 값을 삭제한 뒤, u번 노드의 부모 노드의 값을 u번 노드로 복사합니다., u번 노드의 부모 노드에 대해 같은 작업을 반복하며 루트노드까지 거슬러 올라갑니다. 마지막으로 루트 노드의 값을 w로 바꿉니다.</li>
</ul>

<p>트리의 노드 초기값이 담긴 정수 배열 <code>values</code>, 트리의 연결 상태가 담긴 2차원 정수 배열 <code>edges</code>, 쿼리들이 담긴 2차원 정수 배열 <code>queries</code>가 주어집니다. 쿼리들을 순서대로 처리할 때, 각 1번 쿼리에 대한 답을 수행 순서대로 배열에 담아  return 하도록 solution 함수를 완성해주세요.</p>

<hr>

<h5>제한사항</h5>

<ul>
<li>1 ≤ <code>values</code>의 길이 ≤ 100,000

<ul>
<li><code>values</code>의 길이는 트리의 노드 개수를 의미합니다.</li>
<li><code>values[i]</code>는 i+1번 노드의 초기 값을 의미합니다.</li>
</ul></li>
<li><code>edges</code>의 길이 = <code>values</code>의 길이 - 1

<ul>
<li><code>edges</code>의 각 행은 <code>[v1, v2]</code> 2개의 정수로 이루어져 있으며, 이는 v1번 노드와 v2번 노드가 연결되어 있음을 의미합니다.</li>
<li>주어진 그래프는 항상 1번 노드가 루트인 트리 형태입니다.</li>
</ul></li>
<li>1 ≤ <code>queries</code>의 길이 ≤ 100,000

<ul>
<li>queries의 각 행은 단일 쿼리를 의미하며, <code>[u, w]</code> 2개의 정수로 이루어져 있습니다.</li>
<li>1 ≤ u ≤ values의 길이</li>
<li>-1 ≤ w ≤ 10<sup>9</sup></li>
<li>w가 -1일 경우, 이 쿼리는 1번 쿼리이며, 그렇지 않을 경우 이 쿼리는 2번 쿼리입니다.</li>
</ul></li>
</ul>

<hr>

<h5>입출력 예</h5>
<table class="table">
        <thead><tr>
<th>values</th>
<th>edges</th>
<th>queries</th>
<th>result</th>
</tr>
</thead>
        <tbody><tr>
<td>[1,10,100,1000,10000]</td>
<td>[[1,2],[1,3],[2,4],[2,5]]</td>
<td>[[1,-1],[2,-1],[3,-1],[4,-1],[5,-1],[4,1000],[1,-1],[2,-1],[3,-1],[4,-1],[5,-1],[2,1],[1,-1],[2,-1],[3,-1],[4,-1],[5,-1]]</td>
<td>[11111,11010,100,1000,10000,11111,10011,100,10,10000,11111,11010,100,10,10000]</td>
</tr>
</tbody>
      </table>
<hr>

<h5>입출력 예 설명</h5>

<p>입출력 예 #1</p>

<ul>
<li>주어진 예시는 1번 쿼리 15개와 2번 쿼리 2개로 이루어져 있습니다.</li>
<li>다음 그림은 두 2번 쿼리에 의해 트리의 노드 값들이 바뀌는 과정을 나타낸 것입니다.</li>
</ul>

<p><img src="https://grepp-programmers.s3.ap-northeast-2.amazonaws.com/files/production/7f20b8f9-ee9e-40e2-a905-d69624a1513b/gravity_on_tree_example.png" title="" alt="gravity_on_tree_example.png"></p>


> 출처: 프로그래머스 코딩 테스트 연습, https://school.programmers.co.kr/learn/challenges