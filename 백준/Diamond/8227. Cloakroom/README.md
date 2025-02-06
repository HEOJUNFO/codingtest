# [Diamond IV] Cloakroom - 8227 

[문제 링크](https://www.acmicpc.net/problem/8227) 

### 성능 요약

메모리: 53332 KB, 시간: 620 ms

### 분류

다이나믹 프로그래밍, 배낭 문제, 오프라인 쿼리

### 제출 일자

2025년 2월 6일 11:43:47

### 문제 설명

<p>The annual rich citizen's reunion is taking place in Byteotia. They meet to boast about their incomes, Lebytene's shoes and other luxuries. Naturally, not all these objects of pride are carried into the banquet - coats, jackets, umbrellas and such are left in the cloakroom, and picked up upon leave.</p>

<p>Unfortunately for the well off, a gang of Byteotian thieves plans to break into the cloakroom and steal part of the items stored therein. At this very moment the gang's leader is examining the plans of the robbery put forward by other gang members (they are a democratic lot!). A plan typically looks as follows: the thieves break into the cloakroom at time m<sub>j</sub>, take items worth exactly k<sub>j</sub> and escape, where the whole heist takes them s<sub>j</sub> time. The gang leader would first of all like to know which of these plans are feasible and which are not. A plan is feasible if at time m<sub>j</sub> it is possible to collect items of total value k<sub>j</sub> in such a way that up to the very m<sub>j</sub>+s<sub>j</sub> moment no one shows up to retrieve any of the stolen goods (in such event, they would notify security, and the robbery would fail). In particular, if at time m<sub>j</sub> it is impossible to select items of exact total worth k<sub>j</sub>, then the plan is infeasible and consequently rejected. Knowing the drop off and retrieval times for each item, determine which plans are feasible and which are not. We assume that an item left in the cloakroom the moment a robbery starts can already be stolen (see the example).</p>

### 입력 

 <p>In the first line of the standard input there is a single integer n (1 ≤ n ≤ 1,000) denoting the number of items that will be left in the cloakroom. Those items are described in the n lines that follow. Each of those lines consists of three integers c<sub>i</sub>, a<sub>i</sub>, and b<sub>i</sub> (1 ≤ c<sub>i</sub> ≤ 1,000, 1 ≤ a<sub>i </sub>< b<sub>i </sub>≤ 10<sup>9</sup>), separated by single spaces, that denote respectively: the item's value, the moment it is left in the cloakroom, and the moment it will be retrieved by the owner.</p>

<p>The next line holds an integer p (1 ≤ p ≤ 1,000,000), the number of plans the gang came up with. Each is specified in a separate line by three integers, m<sub>j</sub>, k<sub>j</sub> and s<sub>j </sub>(1 ≤ m<sub>j</sub> ≤ 10<sup>9</sup>, 1 ≤ k<sub>j</sub> ≤ 100,000, 0 ≤ s<sub>j</sub> ≤ 10<sup>9</sup> ), separated by single spaces, that denote respectively: the moment the thieves would enter the cloakroom, the value of goods they would like to steal, and the time the robbery would take them.</p>

<p>In test worth 16% of the total points p ≤ 10 holds in addition.</p>

<p>In some other tests, also worth 16% of the points, all the items have the same a<sub>i</sub> values.</p>

<p>In yet other tests, worth 24% of the points, all the queries share the same s<sub>j</sub> value.</p>

### 출력 

 <p>For each plan put forward by the gang determine if it is feasible, i.e., whether it is possible to steal items of total worth exactly k<sub>j</sub> and escape before anyone asks for their belongings. If the plan is feasible, your program should print the word TAK (Polish for yes) on the standard output, otherwise it should print NIE (Polish for no).</p>

