# [Diamond IV] Candy Rain - 17338 

[문제 링크](https://www.acmicpc.net/problem/17338) 

### 성능 요약

메모리: 30268 KB, 시간: 1472 ms

### 분류

자료 구조, 다차원 세그먼트 트리, 세그먼트 트리

### 제출 일자

2025년 2월 12일 12:55:00

### 문제 설명

<p>There is a beautiful fairy tale: Once upon a time in a far away land by the edge of the world, there existed a "Candy Kingdom". This nation is filled with skyscrapers; meanwhile, the little trails, flowers, and grass are all made out of candy. What's more magical is that the sky is filled with rainbow colored candy clouds floating about. Very often, heavy candy rain will pour onto the ground. The red colors are strawberry candy, the yellow colors are lemon, the green colors are mint, the black colors are chocolate, and so on. The children of Candy Kingdom will frequently hold open pouches of all sizes to catch the candy pieces that fall from the sky, bringing them home for their friends to enjoy.</p>

<p>Having a very big sweet tooth, little Z longs to be able to visit this fairy tale kingdom. As the old saying goes, what you think about in the day, you will dream about during the night. So one night, little Z dreamed that he has arrived in Candy Kingdom. He discovered that at any given moment, all of the clouds in the sky will have different colors. These different colored clouds continuously drop candies of corresponding colors. What's even more interesting is that all of the clouds are in constant back and forth motion. There's no harm in imagining that there are borders to the sky, which is why the clouds just happen to be moving back and forth between the two borders of the sky. During each unit of time, a cloud moves one unit of distance either to the left or to the right. <b>When a cloud hits the left border of the sky, it will change its direction and start moving to the right; when a cloud completely moves past the right border of the sky, it will change its direction and start moving to the left.</b></p>

<p>We may as well think of the sky as a Cartesian coordinate plane, where clouds are represented by line segments (which may degenerate to points):</p>

<p><img alt="" src="https://upload.acmicpc.net/b4547c95-bea6-4b5b-a089-a918bc0e7f60/-/preview/"><br>
 </p>

<p>In the figure above, we set the left border of the sky to be 0 and the right border to be <var>len</var>. The figure contains a total of 5 clouds. The cloud labeled 1 is just changing its direction to move rightwards. The cloud labeled 2 is just changing its direction to move leftwards. The vertical coordinates of the clouds may be ignored, for the clouds will never affect each other during their movement process.</p>

<p>Little Z noticed that new clouds will continuously form in the sky (during some time, at some starting position, moving in some direction), and some clouds after moving for a certain time will disappear from the sky. While clouds are in motion, candy will continuously fall from them. Little Z decided to bring many bags to catch the falling candy. Bags have unlimited capacities, but the size of their openings are limited. For example at time <var>T</var>, little Z carries a bag which has an opening spanning the horizontal range [<var>L</var>, <var>R</var>]. If [<var>L</var>, <var>R</var>] contains a position <var>x</var> where candy falls, then the bag will be able to catch the candy. In extreme cases, the range of the bag's opening can be a point such as [0, 0] or [1, 1], but the bag will still be able to catch candy at the corresponding positions. It is generally known that the amount of candy one can catch is rather larger, so little Z would like to know for each time (the moment he pulls out a bag) <b>how many different colors of candy can his bag catch</b>? The falling times of the candy are negligible.</p>

### 입력 

 <p>The first line contains two positive integers <var>n</var> and <var>len</var>, representing the total number of events and the sky's "boundary", respectively.</p>

<p>For the following <var>n</var> lines, each line describes one event. <b>All events happen in the input order.</b> The first number of each line <var>k</var> (<var>k</var> = 1, 2, or 3) specifies the type of event that happens. There are three types of events: <code>Insert</code>, <code>Query</code>, and <code>Delete</code>. Their formats are as follows:</p>

<table class="table table-bordered">
	<tbody>
		<tr>
			<th>Event Type</th>
			<th>Input Format</th>
			<th>Explanation</th>
		</tr>
		<tr>
			<td><code>Insert</code><br>
			(A new cloud appears in the sky)</td>
			<td><code>1 <var>T<sub>i</sub></var> <var>C<sub>i</sub></var> <var>L<sub>i</sub></var> <var>R<sub>i</sub></var> <var>D<sub>i</sub></var></code></td>
			<td>At time <var>T<sub>i</sub></var>, a new cloud of color <var>C<sub>i</sub></var> spanning the range [<var>L<sub>i</sub></var>, <var>R<sub>i</sub></var>] will appear in the sky. The initial direction of the cloud may be leftwards (<var>D<sub>i</sub></var> = −1) or rightwards (<var>D<sub>i</sub></var> = 1). These parameters will satisfy 0 ≤ <var>L<sub>i</sub></var> ≤ <var>R<sub>i</sub></var> ≤ <var>len</var>, and <var>D<sub>i</sub></var> ∈ {−1, 1}.<br>
			<b>The data guarantees that at any given moment, the sky will not contain two clouds that are the same color.</b></td>
		</tr>
		<tr>
			<td><code>Query</code><br>
			(Determine how many different colors of candy a bag can catch)</td>
			<td><code>2 <var>T<sub>i</sub></var> <var>L<sub>i</sub></var> <var>R<sub>i</sub></var></code></td>
			<td>At time <var>T<sub>i</sub></var>, little Z uses a bag with an opening that spans the range [<var>L<sub>i</sub></var>, <var>R<sub>i</sub></var>] to catch candy. He would like to know how many different colors of them he can catch. The parameters will satisfy 0 ≤ <var>L<sub>i</sub></var> ≤ <var>R<sub>i</sub></var> ≤ <var>len</var>.</td>
		</tr>
		<tr>
			<td><code>Delete</code><br>
			(A cloud disappears from the sky)</td>
			<td><code>3 <var>T<sub>i</sub></var> <var>C<sub>i</sub></var></code></td>
			<td>At time <var>T<sub>i</sub></var>, the cloud colored <var>C<sub>i</sub></var> will disappear from the sky. The data guarantees that the sky will contain a cloud with color <var>C<sub>i</sub></var> at that time.</td>
		</tr>
	</tbody>
</table>

### 출력 

 Empty

