# 세 개의 양수를 입력받는다
w1 = int(input().strip())
w2 = int(input().strip())
w3 = int(input().strip())

# 세 개의 무게를 리스트에 담은 뒤 정렬한다
weights = [w1, w2, w3]
weights.sort()

# 정렬된 리스트에서 중간값(엄마 곰의 그릇 무게)을 출력한다
print(weights[1])
