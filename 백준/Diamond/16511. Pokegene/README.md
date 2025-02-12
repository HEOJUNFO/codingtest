# [Diamond IV] Pokegene - 16511 

[문제 링크](https://www.acmicpc.net/problem/16511) 

### 성능 요약

메모리: 89156 KB, 시간: 1812 ms

### 분류

자료 구조, 다이나믹 프로그래밍, 트리에서의 다이나믹 프로그래밍, 최소 공통 조상, 세그먼트 트리, 문자열, 트리, 트리 압축, 트라이

### 제출 일자

2025년 2월 12일 09:03:53

### 문제 설명

<p>Professor Oak, the friendly neighbourhood Pokémon Professor, has been getting a lot of questions about Pokémon ancestors lately.</p>

<p>One of the things a lot of Pokémon trainers are curious about is finding common ancestors of Pokémon in their Pokédex. In particular, each Pokémon Trainer wants to know about the number of Pokémon that are ancestors of exactly L of their Pokémon. The value of L, too, is decided by the trainer asking the question.</p>

<p>In Pokémon world, given the genome strings of Pokémon A and Pokémon B, Pokémon A is considered an ancestor of Pokémon B if and only if the genome string of Pokémon A is a prefix of Pokémon B’s genome string. Also, all non-empty genome strings correspond to a real Pokémon in the Pokémon world.</p>

<p>Given Professor Oak’s database of Pokémon genomes, help him answer the trainers’ queries.</p>

### 입력 

 <p>The first line of input will contain the integer N, (1 ≤ N ≤ 200 000), the number of Pokémon in Professor Oak’s database, and the integer Q, (1 ≤ Q ≤ 200 000), the number of Pokémon trainers that have questions for Professor Oak. The following N lines contain a string, each denoting a Pokémon genome string from Professor Oak’s database. All genome strings are distinct and consist of lowercase English letters.</p>

<p>Then 2 · Q lines follow. Two lines describe each Pokémon trainer’s query. The first line of each query contains two integers: K, (1 ≤ K ≤ N), the number of Pokémon they own, and L, (1 ≤ L ≤ K), the value described in the statement. The second line contains K distinct space-separated integers. An integer x in the list of K integers indicates that this trainer owns the xth Pokémon from Professor Oak’s database, where the Pokémon Professor Oak owns are numbered from 1 to N.</p>

<p>The sum of the lengths of the genome strings in input does not exceed 200 000 characters and the sum of K over all queries does not exceed 1 000 000.</p>

### 출력 

 <p>For each Pokémon trainer’s query, print the number of Pokémon that are ancestors of exactly L of the K Pokémon they own. Assume that all non-empty genome strings correspond to a Pokémon. Note that L is defined in the query, and does not necessarily remain the same for all queries.</p>

