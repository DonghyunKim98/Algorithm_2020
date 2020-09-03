#include <bits/stdc++.h>
//19237번 문제
using namespace std;
const int MAX = 20 + 1;
typedef struct {
	int ypos, xpos;
}point;
typedef struct {
	int smell_idx, smell_left;
}smell;
vector<pair<point, int>> shark(MAX);
int MAP[MAX][MAX], shark_cnt_MAP[MAX][MAX];
vector<int> eliminated_shark_idx;
smell smell_MAP[MAX][MAX];
vector<vector<int>> shark_priority[MAX * MAX];
int N, M, K;
//위,아래,왼쪽,오른쪽
int dy[] = { 0,-1,1,0,0 };
int dx[] = { 0,0,0,-1,1 };
int cnt;

void make_smell() {
	for (int i = 1; i <= M; i++) {
		//해당 차례가 끝나는 턴에 냄새를 없애줄 것이므로.
		smell_MAP[shark[i].first.ypos][shark[i].first.xpos] = { i, K };
	}
}

void move() {
	make_smell();
	for (int idx = 1; idx <= M; idx++) {
		//제거된 상어인지 확인
		bool is_eliminated = false;
		for (auto u : eliminated_shark_idx) {
			if (idx == u) {
				is_eliminated = true;
				break;
			}
		}
		if (is_eliminated) continue;

		int py = shark[idx].first.ypos, px = shark[idx].first.xpos, p_dir = shark[idx].second;
		int ny = 0, nx = 0, n_dir = 0;
		//우선순위에 맞게 일단 아무 냄새가 없는 곳을 향한다.
		bool is_no_smell = false;
		for (int k = 0; k < 4; k++) {
			n_dir = shark_priority[idx][p_dir][k];
			ny = py + dy[n_dir], nx = px + dx[n_dir];
			if (ny <= 0 || N < ny || nx <= 0 || N < nx) continue;
			if (smell_MAP[ny][nx].smell_left == 0) {
				is_no_smell = true; break;
			}
		}
		if (is_no_smell) {
			shark_cnt_MAP[py][px] = 0, shark_cnt_MAP[ny][nx]++;
			shark[idx].first.ypos = ny, shark[idx].first.xpos = nx, shark[idx].second = n_dir;
			continue;
		}

		//자신의 냄새가 있는 칸의 방향으로 잡는다.
		for (int k = 0; k < 4; k++) {
			n_dir = shark_priority[idx][p_dir][k];
			ny = py + dy[n_dir], nx = px + dx[n_dir];
			if (ny <= 0 || N < ny || nx <= 0 || N < nx) continue;
			if (smell_MAP[ny][nx].smell_idx == idx) {
				break;
			}
		}
		shark_cnt_MAP[py][px] = 0, shark_cnt_MAP[ny][nx]++;
		shark[idx].first.ypos = ny, shark[idx].first.xpos = nx, shark[idx].second = n_dir;
	}
}

void eliminate() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (shark_cnt_MAP[i][j] == 0 || shark_cnt_MAP[i][j] == 1) continue;
			int min_idx = M + 1;
			for (int k = 1; k <= M; k++) {
				//해당위치에 있는 상어 일때
				if (shark[k].first.ypos == i && shark[k].first.xpos == j) {
					//idx가 최솟값이 아니라면
					if (k > min_idx) {
						eliminated_shark_idx.push_back(k);
						cnt--;
					}
					else min_idx = k;
				}
			}
		}
	}
}

void smell_down() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (!smell_MAP[i][j].smell_left) continue;
			smell_MAP[i][j].smell_left--;
			//냄새가 완전히 사라졌다면 상어의 idx값도 지워준다.
			if (smell_MAP[i][j].smell_left == 0) smell_MAP[i][j].smell_idx = 0;
		}
	}
}

void input() {
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> MAP[i][j];
			if (MAP[i][j]) {
				shark[MAP[i][j]] = { {i,j},0 };
				shark_cnt_MAP[i][j] = 1;
			}
		}
	}
	int temp_dir;
	for (int i = 1; i <= M; i++) {
		cin >> temp_dir;
		shark[i].second = temp_dir;
	}

	for (int i = 1; i <= M; i++) {
		//0번쨰 idx채워넣기
		shark_priority[i].push_back({ 0 });
		for (int j = 1; j <= 4; j++) {
			vector<int> temp_vc;
			for (int k = 1; k <= 4; k++) {
				int temp;
				cin >> temp;
				temp_vc.push_back(temp);
			}
			shark_priority[i].push_back(temp_vc);
		}
	}
}


void solution() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	input();
	int ans = 0;
	cnt = M;
	while (ans <= 1000) {
		ans++;
		move();
		eliminate();
		smell_down();
		if (cnt == 1) break;
	}
	if (ans > 1000) cout << -1;
	else cout << ans;
}