# [Gold I] Earn Big - 22600 

[문제 링크](https://www.acmicpc.net/problem/22600) 

### 성능 요약

메모리: 35560 KB, 시간: 7740 ms

### 분류

다이나믹 프로그래밍, 수학, 확률론

### 제출 일자

2024년 12월 16일 11:01:56

### 문제 설명

<p>A group of <var>N</var> people is trying to challenge the following game to earn big money.</p>

<p>First, <var>N</var> participants are isolated from each other. From this point, they are not allowed to contact each other, or to leave any information for other participants. The game organizer leads each participant, one by one, to a room with <var>N</var> boxes. The boxes are all closed at the beginning of the game, and the game organizer closes all the boxes whenever a participant enters the room. Each box contains a slip of paper on which a name of a distinct participant is written. The order of the boxes do not change during the game. Each participant is allowed to open up to <var>M</var> boxes. If every participant is able to open a box that contains a paper of his/her name, then the group wins the game, and everybody in the group earns big money. If anyone is failed to open a box that contains a paper of his/her name, then the group fails in the game, and nobody in the group gets money.</p>

<p>Obviously, if every participant picks up boxes randomly, the winning probability will be <var>(M/N)<sup>N</sup></var>. However, there is a far more better solution.</p>

<p>Before discussing the solution, let us define some concepts. Let <var>P = {p<sub>1</sub>, p<sub>2</sub>, ..., p<sub>N</sub>}</var> be a set of the participants, and <var>B = {b<sub>1</sub>, b<sub>2</sub>, ..., b<sub>N</sub>}</var> be a set of the boxes. Let us define <var>f</var>, a mapping from <var>B</var> to <var>P</var>, such that <var>f(b)</var> is a participant whose name is written on a paper in a box <var>b</var>.</p>

<p>Here, consider a participant <var>p<sub>i</sub></var> picks up the boxes in the following manner:</p>

<ol>
	<li>Let <var>x := i</var>.</li>
	<li>If <var>p<sub>i</sub></var> has already opened <var>M</var> boxes, then exit as a failure.</li>
	<li><var>p<sub>i</sub></var> opens <var>b<sub>x</sub></var>.
	<ol>
		<li>If <var>f(b<sub>x</sub>) = p<sub>i</sub></var>, then exit as a success.</li>
		<li>If <var>f(b<sub>x</sub>) = p<sub>j</sub></var> (<var>i != j</var>), then let <var>x := j</var>, and go to 2.</li>
	</ol>
	</li>
</ol>

<p>Assuming every participant follows the algorithm above, the result of the game depends only on the initial order of the boxes (i.e. the definition of <var>f</var>). Let us define <var>g</var> to be a mapping from <var>P</var> to <var>B</var>, such that <var>g(p<sub>i</sub>) = b<sub>i</sub></var>. The participants win the game if and only if, for every <var>i ∈ {1, 2, ..., N}</var>, there exists <var>k(≤M)</var> such that <var>(f○g)<sup>k</sup> (p<sub>i</sub>) = p<sub>i</sub></var>.</p>

<p>Your task is to write a program that calculates the winning probability of this game. You can assume that the boxes are placed randomly.</p>

### 입력 

 <p>The input consists of one line. It contains two integers <var>N</var> and <var>M</var> (1 ≤ <var>M</var> ≤ <var>N</var> ≤ 1,000) in this order, delimited by a space.</p>

### 출력 

 <p>For given <var>N</var> and <var>M</var>, your program should print the winning probability of the game. The output value should be printed with eight digits after the decimal point, and should not contain an error greater than <var>10<sup>-8</sup></var>.</p>

