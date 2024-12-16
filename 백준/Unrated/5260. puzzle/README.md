# [Unrated] puzzle - 5260 

[문제 링크](https://www.acmicpc.net/problem/5260) 

### 성능 요약

메모리: 35560 KB, 시간: 444 ms

### 분류

분류 없음

### 제출 일자

2024년 12월 16일 12:16:41

### 문제 설명

<p>Little P is very fond of the chips produced by Company X because they are crisp, very tasty and with each package he now increases his chances of winning the big prize at the end-of-year lottery. To take part in this lottery, all he has to do is collect the pieces of puzzle found in the packages. Whenever he finds four pieces that can be assembled to form a square (the square representing the assembled puzzle), he is to stick them to a sheet of paper, label them “Chips forever” and send them to Company X.</p>

<p>The puzzle pieces are made from cardboard, both sides having the same colour, and can be used in 8 distinct ways (in their current position or rotated by 90<sup>o</sup>, 180<sup>o</sup>, 270<sup>o</sup> on both sides).</p>

<p>The initial pieces were n by n squares. From each piece we chose 2 adjacent sides and we cut out some small squares (1 by 1). We cut at least one small square from each chosen side, but also leave at least one small square on each chosen side.</p>

<p>When assembled together they form a 2n–1 by 2n–1 square.</p>

<p>Each piece is given a number from 1 to 4. All the elements for a given piece have the same number, except for the cut out squares that are 0.</p>

<p>This is the last puzzle Little P was able to assemble:</p>

<p style="text-align: center;"><img alt="" src="" style="width: 415px; height: 73px;"></p>

<p>He put these pieces together to form a 7 by 7 square as illustrated in the figure below.</p>

<p style="text-align: center;"><img alt="" src="" style="width: 170px; height: 124px;"></p>

<p>Little P needs a program to help him assemble the puzzle.</p>

<p>You are given the four n by n puzzle pieces in order. They can be rotated and mirrored.</p>

<p>Your task is to assemble these four pieces to form a 2n-1 by 2n-1 square so that the pieces fit perfectly without overlapping and without any spaces between the pieces.</p>

### 입력 

 <p>The input contains the integer n on the first line, representing the size of the puzzle pieces. The next lines describe the 4 puzzle pieces in order.</p>

<p>A puzzle piece is represented by n lines. Each line contains n digits separated by one space.</p>

<p>A blank line separates the puzzle pieces.</p>

### 출력 

 <p>The output will contain 2n-1 lines with 2n-1 digits separated by one space that will represent an assembled puzzle.</p>

