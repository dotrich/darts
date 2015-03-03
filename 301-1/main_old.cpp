#include <iostream>
#include <ctime>
using namespace std;

class Darts301 {
	/*
	int percent;
	int score;
	enum state { fast, slow, focus, out };
	int darts, total_darts;
	int count;
	*/

	int bull(int percent) {	//return 50 or 1 to 20

	if ((rand() % 100) < percent)
		return 50;
	else			// miss
		return 1 + rand() % 20;
	}
	
	int throw_single(int single) { // return single or a neighbour
		int board[22] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20, 1 };
		int r = rand() % 100;
		int i;
		
		if (r<80)	// hit	
			return single;
		else {		// miss
			i = 0;
			do { //to find neighbours search for single in board 
				i++;
			} while (board[i] != single);
						
			if (rand() % 2 == 0) // now return left or right neighbour, each equally likely
				return board[i - 1];
			else
				return board[i + 1];
		}
	}

	int play_301(int s) {
		state player = fast;
		int score = 301;
		int darts = 0;
		int temp = 0;

		int bull(int skill);
		int throw_single(int single);

		do {
			if (score >= 100)
				player = fast;
			else if (score >= 70)
				player = slow;
			else if (score > 50)
				player = focus;
			else
				player = out;

			switch (player) {
			case fast:	score -= bull(s);
				break;

			case slow:	score -= throw_single(20);
				break;

			case focus:	temp = score - throw_single(score - 50);
				if (temp >= 50)
					score = temp;
				break;

			case out:	if (bull(s) == 50)
				score = 0;;
			}
			darts++;
		} while (score >0);

		return darts;
	}

public:
	int game(int N) {
		enum state { fast, slow, focus, out };
		int total_darts = 0;
		int counts[14] = { 0 };
		int temp = 0;

		srand(time(NULL));

		for (int i = 0; i<N; i++) {
			temp = play_301(70);
			total_darts += temp;
			if (temp <13)
				counts[temp]++;
			else
				counts[13]++;
		}

		cout << "Average no. of darts = " << (float)total_darts / N << endl << endl;

		cout << "Seven dart games  " << counts[7] << endl;
		cout << "Eight dart games  " << counts[8] << endl;
		cout << "Nine dart games   " << counts[9] << endl;
		cout << "Ten dart games    " << counts[10] << endl;
		cout << "Eleven dart games " << counts[11] << endl;
		cout << "Twelve dart games " << counts[12] << endl;
		cout << "Longer games      " << counts[13] << endl;

		return 0;
	}
};

int main() {
	Darts301.game(10000);
}

/*
int main() {
	int total_darts = 0;
	int counts[14] = { 0 };
	int temp;

	srand(time(NULL));

	for (int i = 0; i<N; i++) {
		temp = play_301(70);
		total_darts += temp;
		if (temp <13)
			counts[temp]++;
		else
			counts[13]++;
	}

	cout << "Average no. of darts = " << (float)total_darts / N << endl << endl;

	cout << "Seven dart games  " << counts[7] << endl;
	cout << "Eight dart games  " << counts[8] << endl;
	cout << "Nine dart games   " << counts[9] << endl;
	cout << "Ten dart games    " << counts[10] << endl;
	cout << "Eleven dart games " << counts[11] << endl;
	cout << "Twelve dart games " << counts[12] << endl;
	cout << "Longer games      " << counts[13] << endl;

	return 0;
}


int bull(int percent) {	//return 50 or 1 to 20

	if ((rand() % 100) < percent)

		return 50;
	else			// miss
		return 1 + rand() % 20;
}


int throw_single(int single) {
	// return single or a neighbour

	int  board[22] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20, 1 };

	int r = rand() % 100;
	int i;


	if (r<80)	// hit	
		return single;
	else {		// miss
		//to find neighbours search for single in board 
		i = 0;
		do {
			i++;
		} while (board[i] != single);

		// now return left or right neighbour, each equally likely

		if (rand() % 2 == 0)
			return board[i - 1];
		else
			return board[i + 1];
	}
}


int play_301(int s) {
	state player = fast;
	int score = 301;
	int darts = 0;
	int temp = 0;

	int bull(int skill);
	int throw_single(int single);

	do {
		if (score >= 100)
			player = fast;
		else if (score >= 70)
			player = slow;
		else if (score > 50)
			player = focus;
		else
			player = out;

		switch (player) {
		case fast:	score -= bull(s);
			break;

		case slow:	score -= throw_single(20);
			break;

		case focus:	temp = score - throw_single(score - 50);
			if (temp >= 50)
				score = temp;
			break;

		case out:	if (bull(s) == 50)
			score = 0;;
		}
		darts++;
	} while (score >0);

	return darts;
}
*/