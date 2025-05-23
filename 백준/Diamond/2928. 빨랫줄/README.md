# [Diamond IV] 빨랫줄 - 2928 

[문제 링크](https://www.acmicpc.net/problem/2928) 

### 성능 요약

메모리: 68052 KB, 시간: 184 ms

### 분류

자료 구조, 느리게 갱신되는 세그먼트 트리, 수학, 오프라인 쿼리, 세그먼트 트리

### 제출 일자

2025년 2월 8일 12:52:59

### 문제 설명

<p>일요일을 맞이해 창영이는 집안의 모든 침대 시트를 빨고, 집 앞 잔디밭에서 말리고 있다. 갑자기, 엄청난 강풍이 몰아쳤고 모든 침대 시트는 땅바닥에 떨어지게 되었다.</p>

<p>잔디밭은 크기가 무한대인 정사각형 격자로 나타낼 수 있고, 각 단위 정사각형은 좌표 쌍으로 나타낼 수 있다. 침대 시트는 직사각형이며, 축에 평행하다. 시트는 서로 겹칠 수도 있다.</p>

<p>다시 빨래줄에 시트를 걸어놓기 위해 창영이는 막대를 (0,0)에 꽂았다. 그런데 갑자기 땅에서 기름이 나오기 시작했다. 창영이는 이런 예상하지 못한 상황에 당황하였고 결국 기절했다. 창영이가 기절한 사이에 기름은 퍼져나갔다.</p>

<p>시간은 기름이 퍼지기 시작한 순간부터 측정하기 시작한다. 가장 처음에는 단위 정사각형 (0,0)만 기름이 있다. 기름은 아래 그림에 나와있는 것 같이 1초에 한 정사각형씩 여덟 방향으로 이동한다. 기름이 단위 정사각형에 들어가게 되면, 즉시 그 정사각형을 가득 채우게 되고, 그 부분에 속해있는 시트에도 기름이 스며들게 된다.</p>

<p style="text-align: center;"><img alt="" src="" style="width: 604px; height: 174px;"></p>

<p>시간 0, 1, 2초의 모습이다.</p>

<p>입력으로 주어지는 시간이 지난 후에 기름이 물들은 시트의 면적을 구하는 프로그램을 작성하시오.</p>

### 입력 

 <p>첫째 줄에 침대 시트의 수 N (1 ≤ N ≤ 100,000)이 주어진다.</p>

<p>다음 N개 줄에는 침대 시트를 나타내는 좌표 x1, y1, x2, y2 (−1,000,000 ≤ x1 ≤ x2 ≤ 1,000,000, −1,000,000 ≤ y1 ≤ y2 ≤ 1,000,000)가 주어진다. (x1, y1), (x2, y2)는 침대 시트의 양 끝 대각선 좌표이다. 이 좌표는 좌표평면 위의 점이 아닌 각 단위 정사각형을 나타낸다. (0, 0)을 포함하는 침대 시트는 없다.</p>

<p>다음 줄에는 시간의 개수 M (1 ≤ M ≤ 100,000)이 주어진다.</p>

<p>다음 M개 줄에는 시간이 주어진다. 이 정수는 0과 1,000,000 사이의 정수이다. 오름차순으로 주어지며, 중복된 시간은 주어지지 않는다.</p>

### 출력 

 <p>각각의 시간에 대해서, 기름에 물들은 침대 시트 영역의 크기를 출력한다.</p>

