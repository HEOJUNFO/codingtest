# [Diamond IV] Communication Jamming - 17744 

[문제 링크](https://www.acmicpc.net/problem/17744) 

### 성능 요약

메모리: 120244 KB, 시간: 600 ms

### 분류

자료 구조, 분리 집합, 세그먼트 트리

### 제출 일자

2025년 2월 13일 09:31:13

### 문제 설명

<p>JOI 国は平面上に存在する．N 個の村があり，村には 1 から N までの番号がついている．村 i は座標 (i, 0) に存在する点とみなす．現在，JOI 国では村を接続する通信回線を整備しようとしている．障害に備え，通 信回線は二系統整備される予定である．これらを系統 1 と系統 2 と呼ぶ．</p>

<p>系統 k には，ハブが Mk 個存在し，回線が N + M<sub>k</sub> − 1 本ある．系統 k のハブには 1 から M<sub>k</sub> までの番号 がついており，ハブ j は座標 (X<sub>kj</sub>, Y<sub>kj</sub>) に存在する点とみなす．系統 k の各回線は，村と系統 k のハブ，ま たは系統 k のハブ同士を接続する．各回線は，両端を結ぶ線分とみなす．任意の 2 つの回線は端点同士以 外で共有点を持たないようになっていることが保証されている．系統 1 のハブ j の y 座標 Y<sub>1j</sub> は 0 より大 きい．また，系統 2 のハブ j の y 座標 Y<sub>2j</sub> は 0 より小さい．</p>

<p>ある 2 つの地点が通信できるとは，それらの地点が回線により間接的に接続されていることとする．す なわち，回線に沿った移動を繰り返して，片方の地点からもう片方の地点へ移動できるならば，その 2 つ の地点は通信ができる．系統 1 の回線のみを考えても，系統 2 の回線のみを考えても，任意の 2 つの村及 びハブは通信可能である．</p>

<p>下図は通信回線の例である．灰色の円は系統 1 のハブ，黒い円は系統 2 のハブ，白い円は村を表す．</p>

<table class="table table-bordered" style="width: 100%;">
	<tbody>
		<tr>
			<td style="width: 50%; text-align: center;"><img alt="" src="https://upload.acmicpc.net/f892295f-4caf-4847-a179-a514c46dd978/-/preview/" style="width: 137px; height: 231px;"></td>
			<td style="width: 50%; text-align: center;"><img alt="" src="https://upload.acmicpc.net/a2e64d74-71c8-4a78-bdad-edcd945befc3/-/preview/" style="width: 198px; height: 235px;"></td>
		</tr>
		<tr>
			<td style="width: 50%; text-align: center;">図 1: 通信回線の例 1</td>
			<td style="width: 50%; text-align: center;">図 2: 通信回線の例 2</td>
		</tr>
	</tbody>
</table>

<p>計画を検討するにあたって，外部からの攻撃で，どの程度攻撃を受けても通信が可能であるかを調べた い．外部からの攻撃は，2 つの数 A, B (A ≥ 0, B ≤ 0) により表現され，y 座標が A より大きい全てのハブと y 座標が B より小さい全てのハブを破壊するものと想定している．ハブが破壊されると，そこを経由した 通信は行えなくなる．</p>

<p>村や各系統の情報が与えられる．また，Q 個のクエリが与えられる．各クエリ q は 1 つの整数 A<sub>q</sub> で表 され，y 座標が A<sub>q</sub> より大きい全てのハブが破壊される場合を意味する．各クエリに対し，加えて y 座標が いくつ未満のハブまでであれば破壊されても全村間の通信が可能であるかを答えよ．すなわち，整数 Bq で あって，y 座標が A<sub>q</sub> より大きい全てのハブと y 座標が B<sub>q</sub> より小さい全てのハブを破壊しても全村間の通 信が可能であるような最大の B<sub>q</sub> (B<sub>q</sub> ≤ 0) を答えよ．</p>

### 입력 

 <p>標準入力から以下の入力を読み込め．</p>

<ul>
	<li>1 行目には 4 つの整数 N, M<sub>1</sub>, M<sub>2</sub>, Q が空白を区切りとして書かれている．</li>
	<li>続く M<sub>1</sub> + (N + M<sub>1</sub> − 1) 行には，系統 1 の情報が書かれている．
	<ul>
		<li>最初の M1 行の i 行目 (1 ≤ i ≤ M<sub>1</sub>) には，2 つの整数 X<sub>1i</sub>, Y<sub>1i</sub> が書かれている．</li>
		<li>続く N + M<sub>1</sub> − 1 行の i 行目 (1 ≤ i ≤ N + M<sub>1</sub> − 1) には，回線 i の情報を表す 3 つの整数 T<sub>1i</sub>, C<sub>1i</sub>, D<sub>1i</sub> が書かれている (T<sub>1i</sub> = 1, 2)．
		<ul>
			<li>T<sub>1i</sub> が 1 のとき，回線 i は村 C<sub>1i</sub> とハブ D<sub>1i</sub> を接続する (1 ≤ C<sub>1i</sub> ≤ N かつ 1 ≤ D<sub>1i</sub> ≤ M<sub>1</sub>)．</li>
			<li>T<sub>1i</sub> が 2 のとき，回線 i はハブ C<sub>1i</sub> とハブ D<sub>1i</sub> を接続する (1 ≤ C<sub>1i</sub>, D<sub>1i</sub> ≤ M<sub>1</sub> かつ C<sub>1i</sub> ≠ D<sub>1i</sub>)．</li>
		</ul>
		</li>
		<li>続く M<sub>2</sub> + (N + M<sub>2</sub> − 1) 行には，系統 2 の情報が書かれている．
		<ul>
			<li>最初の M<sub>2</sub> 行の i 行目 (1 ≤ i ≤ M<sub>2</sub>) には，2 つの整数 X<sub>2i</sub>, Y<sub>2i</sub> が書かれている．</li>
			<li>続く N + M<sub>2</sub> − 1 行の i 行目 (1 ≤ i ≤ N + M<sub>2</sub> − 1) には，回線 i の情報を表す 3 つの整数 T<sub>2i</sub>, C<sub>2i</sub>, D<sub>2i</sub> が書かれている (T<sub>2i</sub> = 1, 2)．
			<ul>
				<li>T<sub>2i</sub> が 1 のとき，回線 i は村 C<sub>2i</sub> とハブ D<sub>2i</sub> を接続する (1 ≤ C<sub>2i</sub> ≤ N かつ 1 ≤ D<sub>2i</sub> ≤ M<sub>2</sub>)．</li>
				<li>T<sub>2i</sub> が 2 のとき，回線 i はハブ C<sub>2i</sub> とハブ D<sub>2i</sub> を接続する (1 ≤ C<sub>2i</sub> , D<sub>2i</sub> ≤ M<sub>2</sub> かつ C<sub>2i</sub> ≠ D<sub>2i</sub>)．</li>
			</ul>
			</li>
		</ul>
		</li>
	</ul>
	</li>
	<li>続く Q 行の i 行目 (1 ≤ i ≤ Q) には 1 つの整数 A<sub>i</sub> が書かれている．</li>
</ul>

### 출력 

 <p>標準出力に Q 行出力せよ．i 行目 (1 ≤ i ≤ Q) には，クエリ i への答えを表す整数 B<sub>i</sub> を出力せよ． 答えが 0 の場合，-0 と出力してはならない．</p>

