# [Diamond IV] Rounding - 13569 

[문제 링크](https://www.acmicpc.net/problem/13569) 

### 성능 요약

메모리: 3616 KB, 시간: 16 ms

### 분류

서큘레이션, 최대 유량

### 제출 일자

2025년 2월 6일 10:04:16

### 문제 설명

<p>Professor Park conducts and records the N data values from his experiment every day. The data values are recorded in real numbers, which rounded up to the first decimal place. The data of M days are shown on a two dimensional table with each row sum and each column sum. Every experimental data values including row sums and column sums, however, must be in integers to be published. Each number can be up or down to the nearest integer so that the sum of the rounded elements in each row (column) equals the row (column) sum. More formally, if x is a real number which represents an experimental data value or a row sum or a column sum, you can replace x by ⌊x⌋ or ⌈x⌉. If this operation, i.e. feasible rounding, is possible, the new table is called feasibly rounded table.</p>

<p>For examples, refer to the following tables:</p>

<table class="table" style="width:100%">
	<tbody>
		<tr>
			<td style="text-align:center"><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/13569/1.png" style="height:113px; width:249px"></td>
			<td style="text-align:center"><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/13569/2.png" style="height:114px; width:251px"></td>
			<td style="text-align:center"><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/13569/3.png" style="height:92px; width:253px"></td>
			<td style="text-align:center"><img alt="" src="https://onlinejudgeimages.s3-ap-northeast-1.amazonaws.com/problem/13569/4.png" style="height:93px; width:253px"></td>
		</tr>
		<tr>
			<td style="text-align:center">(a) Original Table</td>
			<td style="text-align:center">(b) A feasibly rounded table</td>
			<td style="text-align:center">(a) Original Table</td>
			<td style="text-align:center">(b) A feasibly rounded table</td>
		</tr>
		<tr>
			<td colspan="2" style="text-align:center">Figure 1. First example of the feasible rounding</td>
			<td colspan="2" style="text-align:center">Figure 2. Second example of the feasible rounding.</td>
		</tr>
	</tbody>
</table>

<p>Given an original table, write a program that finds a feasibly rounded table.</p>

### 입력 

 <p>Your program is to read from standard input. The first line of the input contains two integers M (2 ≤ M ≤ 200) which represents the total experiment days and N (2 ≤ N ≤ 200) which represents the experimental data values for each day. In the following M lines, all experimental data values and row sums are given in real numbers to first decimal place. The i-th line consists of N real numbers which represent the experimental data values of the i-th day (1 ≤ i ≤ M) and the i-th row sum. The next line consists of N real numbers which represent column sums. The experimental data values are between 0.0 to 1,000.0, inclusively.</p>

### 출력 

 <p>Your program is to write to standard output. Print a feasibly rounded table including each row sum and column sum. It is known that the feasibly rounded table can be obtained always.</p>

