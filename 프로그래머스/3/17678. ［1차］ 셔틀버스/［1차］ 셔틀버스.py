def solution(n, t, m, timetable):
    # 시간 -> 분 변환
    def time_to_minutes(time_str):
        h, mm = map(int, time_str.split(':'))
        return h * 60 + mm

    # 분 -> 시간 변환
    def minutes_to_time(minutes):
        return f"{minutes//60:02d}:{minutes%60:02d}"

    # 크루 도착 시간을 분 단위로 변환하고 정렬
    crew_times = sorted([time_to_minutes(tt) for tt in timetable])

    # 셔틀버스 출발시간 (분 단위) 리스트
    start_time = 540  # 09:00
    shuttle_times = [start_time + i * t for i in range(n)]

    # 각 셔틀별로 최대 m명의 승객을 태운다.
    idx = 0  # 현재 crew_times를 탐색할 인덱스
    for i in range(n):
        shuttle_time = shuttle_times[i]
        # 해당 셔틀 시간에 탈 수 있는 사람(=셔틀 시간 <= crew 도착 시간)이 있는지 확인
        # 최대 m명 태운다.
        count = 0
        while count < m and idx < len(crew_times) and crew_times[idx] <= shuttle_time:
            idx += 1
            count += 1

    # 여기서 i번째 셔틀에 탑승한 사람이 count명
    # idx는 i번째 셔틀까지 태운 총 인원의 수이다.
    # 마지막에 태운 셔틀은 shuttle_times[-1]
    # 마지막 셔틀에 탄 사람의 수는 count

    # 마지막 셔틀에 탄 사람 수가 m명 미만이면 해당 셔틀 시간에 도착해도 탑승 가능
    if count < m:
        # 마지막 셔틀 도착 시간에 맞춰 도착하면 됨
        return minutes_to_time(shuttle_times[-1])
    else:
        # 마지막 셔틀이 꽉 찼다면 마지막으로 탄 사람보다 1분 일찍 도착해야 한다.
        # 마지막으로 탄 사람은 crew_times[idx-1]
        return minutes_to_time(crew_times[idx-1] - 1)
