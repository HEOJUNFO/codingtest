#include <stdio.h>
#include <math.h>

int solution(int h1, int m1, int s1, int h2, int m2, int s2) {
    int start_t = h1*3600 + m1*60 + s1;
    int end_t = h2*3600 + m2*60 + s2;

    // HS: t = (43200/719)*k
    // k 범위 구하기
    double hsA = (double)start_t * 719.0 / 43200.0;
    double hsB = (double)end_t * 719.0 / 43200.0;
    int min_k_hs = (int)ceil(hsA);
    int max_k_hs = (int)floor(hsB);
    int HS_count = (max_k_hs >= min_k_hs) ? (max_k_hs - min_k_hs + 1) : 0;

    // MS: t = (3600/59)*k
    double msA = (double)start_t * 59.0 / 3600.0;
    double msB = (double)end_t * 59.0 / 3600.0;
    int min_k_ms = (int)ceil(msA);
    int max_k_ms = (int)floor(msB);
    int MS_count = (max_k_ms >= min_k_ms) ? (max_k_ms - min_k_ms + 1) : 0;

    // triple alignment: t=0, t=43200
    int triple_count = 0;
    if (start_t <= 0 && 0 <= end_t) triple_count++;
    if (start_t <= 43200 && 43200 <= end_t) triple_count++;

    int answer = HS_count + MS_count - triple_count;

    return answer;
}

int main(){
    // 예제 테스트
    printf("%d\n", solution(0,5,30,0,7,0));    //2
    printf("%d\n", solution(12,0,0,12,0,30)); //1
    printf("%d\n", solution(0,6,1,0,6,6));    //0
    printf("%d\n", solution(11,59,30,12,0,0));//1
    printf("%d\n", solution(11,58,59,11,59,0));//1
    printf("%d\n", solution(1,5,5,1,5,6));    //2
    printf("%d\n", solution(0,0,0,23,59,59)); //2852
    return 0;
}
