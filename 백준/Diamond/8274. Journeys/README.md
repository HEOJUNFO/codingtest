# [Diamond IV] Journeys - 8274 

[문제 링크](https://www.acmicpc.net/problem/8274) 

### 성능 요약

메모리: 108560 KB, 시간: 572 ms

### 분류

자료 구조, 데이크스트라, 그래프 이론, 느리게 갱신되는 세그먼트 트리, 세그먼트 트리, 최단 경로

### 제출 일자

2025년 2월 10일 17:08:29

### 문제 설명

<p>There are <em>n</em> towns in Byteotia, numbered 1 through <em>n</em>. Highways are built in the country not too often; if they are built, however, they are built in bigger batches. Thus <em>m</em> batches have been built, the <em>i</em>-th batch consists of all highways connecting towns with numbers in [<em>a<sub>i</sub></em>,<em>b<sub>i</sub></em>] with towns with numbers in [<em>c<sub>i</sub></em>,<em>d<sub>i</sub></em>]. The highways cross only at the towns, but they may lead through tunnels or flyovers. Using the highway network one can travel between any pair of towns in Byteotia. Riding a single highway costs exactly 1 bytean dollar.</p>

<p>Byteasar spent the last several years abroad. This year he has decided to return to his home country and buy a house in the capital of the country, Bitcity. He is planning to visit all his old acquaintances that now live in many different towns of Byteotia. He would like to know what is the cheapest route from Bitcity to any other town in Byteotia (using only the highway network). Help him find that out.</p>

### 입력 

 <p>The first line of the standard input contains three integers <em>n</em>, <em>m</em> and <em>p</em> (2 ≤ <em>n</em> ≤ 500 000, 1 ≤ <em>m</em> ≤ 100 000, 1 ≤ <em>p</em> ≤ <em>n</em>) that represent the number of towns in Byteotia, the number of batches of highways that have been built and the number of the town where Byteasar lives (that is, Bitcity). The following <em>m</em> lines contain a description of the batches, one per line. Each such description consists of four integers <em>a<sub>i</sub></em>, <em>b<sub>i</sub></em>, <em>c<sub>i</sub></em>, <em>d<sub>i</sub></em> (1 ≤ <em>a<sub>i</sub></em> ≤ <em>b<sub>i</sub></em> ≤ <em>n</em>, 1 ≤ <em>c<sub>i</sub></em> ≤ <em>d<sub>i</sub></em> ≤ <em>n</em>, [<em>a<sub>i</sub></em>,<em>b<sub>i</sub></em>] ∩ [<em>c<sub>i</sub></em>,<em>d<sub>i</sub></em>] = ∅). It means that from each of the towns <em>a<sub>i</sub></em>, ..., <em>b<sub>i</sub></em> leads a bidirectional highway to each of the towns <em>c<sub>i</sub></em>, ..., <em>d<sub>i</sub></em>. Each highway appears in at most one batch.</p>

### 출력 

 <p>Your program should write <em>n</em> lines to the standard output. The <em>i</em>-th line should contain the minimal cost, in bytean dollars, of travelling from Bitcity to the <em>i</em>-th town of Byteotia. The <em>p</em>-th of these lines should contain the number 0.</p>

