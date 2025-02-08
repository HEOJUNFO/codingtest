# [Diamond IV] The MP3 Player - 3316 

[문제 링크](https://www.acmicpc.net/problem/3316) 

### 성능 요약

메모리: 23692 KB, 시간: 624 ms

### 분류

자료 구조, 세그먼트 트리

### 제출 일자

2025년 2월 8일 22:22:53

### 문제 설명

<p>Georg's new MP3 player has many interesting features, one of them being the key lock. All the keys are locked after more than T seconds of inactivity. After the key lock is engaged, no key performs its original function, but if any key is pressed, the key lock is disengaged.</p>

<p>For example, assume that T = 5 and the player is currently locked. Georg presses the key A, waits for 3 seconds, presses the key B, waits for 5 seconds, presses C, waits for 6 seconds, and presses D. In this case only the keys B and C perform their regular functions. Note that the keys became locked between C and D was pressed.</p>

<p>Sound level of the MP3 player is controlled by the + and - keys, increasing and decreasing volume by 1 unit respectively. The sound level is an integer between 0 and V<sub>max</sub>. Pressing the + key at volume V<sub>max</sub> or pressing the - key at volume 0 leaves the volume unchanged.</p>

<p>Georg does not know the value of T. He wanted to find it by an experiment. Starting with a locked keyboard, he pressed a sequence of N + and - keys. At the end of the experiment Georg read the final volume from the player's display. Unfortunately, he forgot to note the volume before his first keypress. For the purpose of this task, the unknown initial volume will be denoted V<sub>1</sub> and the known final volume will be denoted V<sub>2</sub>.</p>

<p>You are given the value V<sub>2</sub> and a list of keystrokes in the order in which Georg made them. For each key, you are given the type of the key (+ or -) and the number of seconds from the beginning of the experiment to the moment when the key was pressed. The task is to find the largest possible integer value of T which is consistent with the outcome of the experiment.</p>

### 입력 

 <p>The first line of the input contains three space-separated integers N, V<sub>max</sub> and V<sub>2</sub> (0 ≤ V<sub>2</sub> ≤ V<sub>max</sub>). Each of the next N lines contains a description of one key in the sequence: a character + or -, a space and an integer C<sub>i</sub> (0 ≤ Ci ≤ 2·10<sup>9</sup>), the number of seconds from the beginning of the experiment. You may assume that the keypresses are in sorted order and that all times are distinct (i.e., C<sub>i</sub> < C<sub>i + 1</sub> for all 1 ≤ i < N).</p>

### 출력 

 <p>If T can be arbitrarily large, output a single line containing the word "infinity" (quotes for clarity).</p>

<p>Otherwise, output a single line containing two integers T and V<sub>1</sub> separated by a single space.</p>

<p>The values must be such that carrying out the experiment with locking time T starting at volume V<sub>1</sub> gives the final volume V<sub>2</sub>. If there are multiple possible answers, output the one with the largest T; if there are still multiple possible answers, output the one with the largest V<sub>1</sub>.</p>

<p>(Note that at least one solution always exists: for T = 0 none of the keys performs its action, so it suffices to take V<sub>1</sub> = V<sub>2</sub>.)</p>

