# [Diamond IV] HAPPINESS - 11783 

[문제 링크](https://www.acmicpc.net/problem/11783) 

### 성능 요약

메모리: 62692 KB, 시간: 480 ms

### 분류

자료 구조, 세그먼트 트리

### 제출 일자

2025년 2월 11일 09:08:30

### 문제 설명

<p>Monetary system in X-land is a bit strange. There are banknotes with values of all integer numbers from 1 to M. There is another strange rule in the shops of X-land – the customer can never receive change, but also cannot leave a tip – in other words the customer must always pay the exact value of his purchases. If he does not have the exact sum for his purchase, then he cannot buy. Imagine what inconvenience this creates for the customers. </p>

<p>Niya is a girl from X-land. Like all other persons, she constantly fights against the rules described above. She always knows her set of banknotes – let’s assume their values are a<sub>1</sub>, a<sub>2</sub>,…,a<sub>N</sub>. All those values are between 1 and M and she may possess more than one banknote of a kind. Also the sequence of values a<sub>1</sub>, a<sub>2</sub>,…….,a<sub>N</sub>, is not ordered in any way. Niya feels happy, when entering a shop, she may buy any combination of goods with total price equal to any number between 1 and the total sum of her banknotes a<sub>1</sub> + a<sub>2</sub> + ... + a<sub>N</sub>. In that case, when she is shopping, she must only consider her total amount of money without making complicated calculations of whether she can buy (or not) with her banknotes. </p>

<p>Remark: Let us sort a<sub>1</sub>, a<sub>2</sub>,…….,a<sub>N</sub> in ascending order. Let us denote S<sub>i</sub> = 1+a<sub>1</sub>+ a<sub>2</sub>+…….+a<sub>i</sub>. Necessary and sufficient condition to be able to represent each number between 1 and a<sub>1</sub>+ a<sub>2</sub>+…….+a<sub>N</sub> as a sum of elements from the multiset a<sub>1</sub>, a<sub>2</sub>,…….,a<sub>N</sub>, is that the following inequality S<sub>i</sub> ≥ a<sub>i+1</sub> held true for each i>1 and a<sub>1</sub> = 1. </p>

<p>As expected, Niya’s set of banknotes is changing after each purchase and also after each wage she receives – that’s why her happiness is variable. You can help the girl with a program. Your program will receive as an input the initial set of Niya’s banknotes and all the events that happen – purchases and wages. The program should be able to determine if Niya is happy in the beginning and after each event.</p>

<p>We should note that Niya feels happy also when she doesn’t have any money – then she just skips shopping and goes jogging.</p>

<p>Task</p>

<p>Write functions init() and is_happy(), which will be compiled with jury’s grader. These functions should serve to determine Niya’s happiness at the beginning and after each event. The functions will receive as parameters the starting set of banknotes and the sets of banknotes that are removed from the set (on purchases) and added to the set (on receiving wage).</p>

<p>Implementation details</p>

<p>You should submit to the grading system a file happiness.cpp, which contains functions:</p>

<pre>bool init(int coinsCount, long long maxCoinSize, long long coins[]).
bool is_happy(int event, int coinsCount, long long coins[]).</pre>

<p>Parameters description:</p>

<ul>
	<li>coinsCount – number of banknotes that are received (starting set or wage) or discarded (shopping).</li>
	<li>maxCoinSize – maximum value of one banknote. coins[] – array, in which in random order are given values of the banknotes (index starts from 0).</li>
	<li>event – event’s type :
	<ul>
		<li>−1 – Shopping;</li>
		<li>1 – Receiving wage.</li>
	</ul>
	</li>
</ul>

<p>The function init is called once by the grader at the beginning to set the starting set of Niya’s banknotes and then grader calls Q times function is_happy with event = −1 (shopping) or event = 1 (wage). After each call the called function should return true, if Niya feels happy with her current set of banknotes or false if she doesn’t.</p>

<p>File happiness.cpp should NOT contain function main(), but can contain other declarations and functions, necessary for correct working of functions init and is_happy. Your program should contain #include "happiness.h" in the beginning</p>

### 입력 

 <p>Single positive integers N and M are given on the first row – initial count of Niya’s banknotes and maximum value of one banknote.</p>

<p>N positive integers are given on the second row, separated by spaces – values of banknotes in the initial set.</p>

<p>Non-negative integer Q is given on the third row – event’s count.</p>

<p>On each of the next Q rows one event is described – first, a value for the event is given: −1 (shopping) or 1 (receiving of a wage). After that a positive integer K is given – number of banknotes that are removed or added to Niya’s set. Last K integers are given, separated by intervals – values for the banknotes which are removed or added.</p>

### 출력 

 <p>On the standard output the program will print Q+1 lines with 0 or 1 – "happiness" statuses of Niya at the beginning and after each event.</p>

