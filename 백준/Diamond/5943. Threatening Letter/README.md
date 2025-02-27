# [Diamond IV] Threatening Letter - 5943 

[문제 링크](https://www.acmicpc.net/problem/5943) 

### 성능 요약

메모리: 13244 KB, 시간: 4 ms

### 분류

자료 구조, 다이나믹 프로그래밍, 세그먼트 트리, 문자열, 접미사 배열과 LCP 배열

### 제출 일자

2025년 2월 8일 22:58:27

### 문제 설명

<p>FJ has had a terrible fight with his neighbor and wants to send him a nasty letter, but wants to remain anonymous. As so many before him have done, he plans to cut out printed letters and paste them onto a sheet of paper. He has an infinite number of the most recent issue of the Moo York Times that has N (1 <= N <= 50,000) uppercase letters laid out in a long string (though read in as a series of shorter strings). Likewise, he has a message he'd like to compose that is a single long string of letters but that is read in as a set of shorter strings.</p>

<p>Being lazy, he wants to make the smallest possible number of cuts. FJ has a really great set of scissors that enables him to remove any single-line snippet from the Moo York Times with one cut. He notices that he can cut entire words or phrases with a single cut, thus reducing his total number of cuts.</p>

<p>What is the minimum amount of cuts he has to make to construct his letter of M (1 <= M <= 50,000) letters?</p>

<p>It is guaranteed that it is possible for FJ to complete his task.</p>

<p>Consider a 38 letter Moo York Times:</p>

<pre>        THEQUICKBROWNFOXDO
        GJUMPSOVERTHELAZYDOG</pre>

<p>from which FJ wants to construct a 9 letter message:</p>

<pre>        FOXDOG
        DOG</pre>

<p>These input lines represent a pair of strings:</p>

<pre>        THEQUICKBROWNFOXDOGJUMPSOVERTHELAZYDOG
        FOXDOGDOG</pre>

<p>Since "FOXDOG" exists in the newspaper, FJ can cut this piece out and then get the last "DOG" by cutting out either instance of the word "DOG".</p>

<p>Thus, he requires but two cuts.</p>

### 입력 

 <ul>
	<li>Line 1: Two space-separated integers: N and M</li>
	<li>Lines 2..?: N letters laid out on several input lines; this is the text of the one copy of the Moo York Times. Each line will have no more than 80 characters.</li>
	<li>Lines ?..?: M letters that are the text of FJ's letter. Each line will have no more than 80 characters.</li>
</ul>

<p> </p>

### 출력 

 <ul>
	<li>Line 1: The minimum number of cuts FJ has to make to create his message</li>
</ul>

<p> </p>

