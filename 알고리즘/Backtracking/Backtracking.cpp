#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define MAX 1000

int n;
int hole;
int index;
vector<int> stones;
vector<vector <int>> state;
vector<int> ans;
int min = MAX;
vector<vector<int>> min_ans;

void swap(int m, int n) {
	int temp = stones[m];
	stones[m] = stones[n];
	stones[n] = temp;
}


bool isPromising(int i) {
	//1. hole�� �������� ���� �ΰ��� �ڸ��� �ű� �� �ִ�.
	if (i < hole - 2 || i > hole + 2)
		return false;
	else if (i == hole)
		return false;
	else {
		//2. hole-2 ��°�� 2 �̸� ���� �Ұ�, 1 �϶� hole-1�� 1�̸� �̵� �Ұ�
		if (i == hole - 2) {
			if (stones[hole - 2] == 2 || stones[hole - 2] == stones[hole - 1])
				return false;
		}
		//3. hole -1 ��°�� 2�̸� ���� �Ұ�
		else if (i == hole - 1) {
			if (stones[hole - 1] == 2)
				return false;
		}
		//4. hole + 1 ��°�� 1�̸� ���� �Ұ�
		else if (i == hole + 1) {
			if (stones[hole + 1] == 1)
				return false;
		}
		//5. hole + 2 ��°�� 1 �̸� ���� �Ұ�, 2 �϶� hole + 1 �� 2 �̸� �̵� �Ұ�
		else if (i == hole + 2) {
			if (stones[hole + 2] == 1 || stones[hole + 2] == stones[hole + 1])
				return false;
		}
	}
	return true;
}

void move(int depth, int position) {
	//hole�� ��� ���� ���
	if (position == n) {
		int k;
		//������ ��� 2�� �ٲ����� �˻��Ѵ�
		for (k = 0; k < n; k++) {
			if (stones[k] != 2)
				break;
		}
		//��� �ٲ��ٸ�
		if (k == n) {
			//�� ���� �̵��� �����ϰ� �ּ����� Ȯ���Ѵ�
			min_ans.push_back(ans);
			if (min > ans.size())
				min = ans.size();
		}
	}
	
	for (index = 0; index < stones.size(); index++) {
		
		if (isPromising(index)) {
			state.push_back(stones);
			ans.push_back(index);
			swap(hole, index);
			hole = index;
			move(depth+1, hole);

			if (depth == 0 && position == 2 *n)
				return;

			//02.. or ..10 �� ���� ��쿡�� �� ĭ �� �ǳʶپ� �˻��Ѵ�
			
			else if (stones[stones.size() - 1] == 0 && stones[stones.size() - 2] == 1)
				index++;
		}
	}
	if (depth == 0) {
		for (int m = 0; m < min_ans.size(); m++) {
			if (min_ans[m].size() == min) {
				cout << min_ans[m].size();
				for (int n = 0; n < min; n++) {
					if (n % 20 == 0)
						cout << '\n';
					cout << min_ans[m][n] + 1 << " ";
				}
				cout << '\n';
				break;
			}
		}
		return;
	}
		
	stones = state[state.size()-1];
	for (int i = 0; i < stones.size(); i++) {
		if (stones[i] == 0)
			hole = i;
	}
	state.pop_back();
	index = ans[ans.size() - 1];
	ans.pop_back();
}


int main() {
	while (1) {
		string number;
		cin >> number;
		if (!number.compare("EOI"))
		{
			break;
		}
		else {
			n = stoi(number);
			if (n < 1 || n >12)
				break;
		}

		for (int i = 0; i < n; i++) {
			stones.push_back(1);
		}
		stones.push_back(0);
		for (int i = 0; i < n; i++) {
			stones.push_back(2);
		}

		hole = n;
		move(0, 0);

		stones.clear();
		state.clear();
		ans.clear();
		min_ans.clear();
		min = MAX;
	}
	return 0;
}