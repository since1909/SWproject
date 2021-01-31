#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> S;
vector<int> subset1;
vector<int> subset2;

void isPossible(int N, int sum) {
	if (sum % 2 != 0)
	{ 
		cout << "cases: 0\n";
		return;
	}
	else sum = sum / 2;
	vector<vector<unsigned long long int>> P(N + 1, vector<unsigned long long int>(sum + 1, 0));
	int i, j;
	for (i = 0; i <= N; i++)
		P[i][0] = 1;
	for (j = 1; j <= sum; j++)

		P[0][j] = 0;

	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= sum; j++)
		{
			P[i][j] = P[i - 1][j];
			if (S[i-1] <= j)
			{
				P[i][j] += P[i - 1][j - S[i-1]];
			}
		}
	}

	int a = N;
	int b = sum;
	while (a > 0 && b >= 0)
	{
		if (P[a - 1][b] != 0)
		{
			a--;
			subset2.push_back(S[a]);
		}
		else if (P[a - 1][b - S[a - 1]] != 0)
		{
			a--;
			b -= S[a];
			subset1.push_back(S[a]);
		}
	}

	if (P[N][sum] / 2 > ULONG_MAX) {
		cout << "NUMEROUS\n";
		return;
	}
	else if (P[N][sum] / 2 == 0)
	{
		cout << P[N][sum] / 2 << "\n";
		return;
	}
	else {
		cout << P[N][sum] / 2 << "\n";
		cout << "{";
		for (int i = 0; i < subset1.size(); i++)
		{
			cout << subset1[i];
			if (i < subset1.size() - 1)
				cout << ",";
		}
		cout << "},";
		cout << "{";
		for (int i = 0; i < subset2.size(); i++)
		{
			cout << subset2[i];
			if (i < subset2.size() - 1)
				cout << ",";
		}

		cout << "}" << "\n";
	}
}


int main() {
	while (1) {
		string str;
		int N;
		int sum = 0;
		cin >> str; //������ ������ ����
		
		if (!str.compare("EOI"))	break;
		else if (stoi(str) < 1 || stoi(str) > 1000) break;
		else N = stoi(str);
		for (int i = 0; i < N; i++)
		{
			int tmp;
			cin >> tmp;
			if (tmp >= 1500 || tmp <= 0)
			{
				i--;
				continue;
			}
			S.push_back(tmp);
			sum += S[i];
		}

		isPossible(N, sum);
		S.clear();
		subset1.clear();
		subset2.clear();
	}	
	return 0;
}

