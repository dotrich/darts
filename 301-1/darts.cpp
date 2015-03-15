#include <iostream>
#include <ctime>

#include "darts.h"

using std::cout;

Darts::~Darts()
{
	delete [] thrCount;
	thrCount = nullptr;
}

uint32_t Darts::getScore()
{
	return score_;
}

uint16_t Darts::getNoPlayers()
{
	return plyrNo_;
}

uint32_t Darts::getNoGames()
{
	return gameNo_;
}

void Darts::setScore(uint32_t s)
{
	score_ = s;
}

void Darts::setNoPlayers(uint16_t p)
{
	plyrNo_ = p;
}

void Darts::setNoGames(uint32_t g)
{
	gameNo_ = g;
}

uint32_t clamp(uint32_t cur, uint32_t min, uint32_t max)
{
	cur = cur < min ? min : cur;
	cur = cur > max ? max : cur;

	return cur;
}

uint16_t Darts::single(uint16_t p) // non-bullseye darts
{
	uint16_t randNo = rand()%100;
	uint16_t item = 0;
	uint16_t board[22] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20, 1};

	if (randNo<80) // 80% chance to hit target
		return p;
	else // 20% chance to miss, 10% to hit left or right (on board array)
	{		
		do
		{
			item++;
		} while (board[item] != p);

		if (rand()%2 == 0)
			return board[item - 1]; // hits left of target
		else
			return board[item + 1]; // hits right of target
	}
}

uint16_t Darts::bull(uint16_t p) // bullseye darts
{
	// throw for the bull with accuracy p% (20 < p < 85)
	
	int r = rand()%100;
	
	if (r < (p - 20))
		return 50;
	else if (r < 85)	
		return 25;
	else
		return 1 + rand()%20;
}

uint16_t Darts::game(uint16_t p) // handles one game
{
	state style = FAST;
	uint32_t tmpScore = score_;
	uint16_t darts = 0;
	uint32_t temp = 0;

	do 
	{
		if (tmpScore >= 100) // decides state
			style = FAST;
		else if (tmpScore >= 70)
			style = SLOW;
		else if (tmpScore > 50)
			style = FOCUS;
		else
			style = OUT;

		switch(style) // performs dart throws
		{
			case FAST :
				tmpScore -= bull(p);
				break;
			case SLOW :
				tmpScore -= single(20);
				break;
			case FOCUS :
				temp = tmpScore - single(tmpScore - 50);

				if (temp >= 50)
					tmpScore = temp;
				break;
			case OUT :
				if (bull(p) == 50)
					tmpScore = 0;
		}

		darts++;
	} while (tmpScore > 0);
	
	return darts;
}

void Darts::playGame()
{
	std::string errorMsg = "";

	if (score_ < 101 || score_ > 30001)
	{
		errorMsg = "setScore > 100 || setScore < 30002";
		throw std::length_error(errorMsg);
		score_ = clamp(score_, 101, 30001);
	}
	
	if (plyrNo_ < 1 || plyrNo_ > 1000)
	{
		errorMsg = "setNoPlayers > 0 || setNoPlayers < 1001";
		throw std::length_error(errorMsg);
		plyrNo_ = (uint16_t)clamp(plyrNo_, 1, 1000);
	}
	
	if (gameNo_ < 1 || gameNo_ > 100000000)
	{
		errorMsg = "setNoGames > 0 || setNoGames < 100000001";
		throw std::length_error(errorMsg);
		gameNo_ = clamp(gameNo_, 1, 100000000);
	}

	const uint32_t maxDiv = (uint32_t)score_ / 5;

	// for outputing games with thrown darts e.g games with darts >0 were thrown
	uint32_t min = 0; 
	uint32_t max = maxDiv;

	// stores games organised into the number of darts thrown (eg 7 games had 42 darts thrown, 21 had 3 thrown etc)
	thrCount = new uint32_t[max];

	for (uint32_t x = 0; x <= max; x++)
		thrCount[x] = 0;

	uint32_t totDarts = 0;
	uint16_t darts = 0;
	uint16_t trash = 0;

	uint16_t plyrNo = 1;
	char *plyrName = "Unknown";

	srand( (uint32_t)time(NULL) );
	trash = rand()%100; // throws away first random value for better divergence
	
	cout << "No of Players:   " << plyrNo_ << "\n"
			<< "No of Games:     " << gameNo_ << "\n"
			<< "Starting Score:  " << score_ << "\n";
			 
	for (uint16_t x = 0; x < plyrNo_; x++)
	{
		for (uint32_t y = 0; y < gameNo_; y++)
		{				
			darts = game(rand()%75 + 70);
			totDarts += darts;

			thrCount[darts]++;
		}

		// gets first index with non-zero value
		for (uint32_t y = 0; y < max; y++)
		{				
			if (thrCount[y] != 0)
				break;
			min++;
		}

		// gets last index with non-zero value
		for (uint32_t y = max; y > 0; y--)
		{
			if (thrCount[y] != 0)
				break;
			max--;
		}

		// current player information
		cout << "\nPlayer: " << plyrName << plyrNo << " - Darts Thrown -\n\n"
			 << "Average Darts Thrown: " << (float)totDarts / gameNo_ << "\n";

		// displays games organised into the number of darts thrown
		for (uint32_t y = min; y < max + 1; y++)
			cout << "Games - " << y << ": " << thrCount[y] << "\n";

		if (plyrNo_ > 1) // refresh values for next player
		{
			totDarts = 0;
			darts = 0;
			plyrNo++;

			min = 0;
			max = maxDiv;

			for (uint32_t z = 0; z < max; z++)
				thrCount[z] = 0;
		}
	}
}

//int throw_treble(int d, int p) {
//
//	//  return result of throwing for treble d with accuracy p%  (o<90)
//	
//	// Board neighbours ignoring slot zero
//	int bd[2][21]={{0,20,15,17,18,12,13,19,16,14,6,8,9,4,11,10,7,2,1,3,5},
//		       {0,18,17,19,13,20,10,16,11,12,15,14,5,6,9,2,8,3,4,7,1}};
//	
//	int r = rand()%100;
//	
//	if(r<p)
//		return 3*d;
//	else if(r<90)
//		return d;
//	else if(r<93)
//		return 3*bd[0][d];
//	else if (r<96)
//		return 3*bd[1][d];
//	else if(r<98)
//		return bd[0][d];
//	else 
//		return bd[1][d];
//}

//int throw_double(int d) {
//	
//	// return result of throwing for double d with accuracy 80%
//	
//	// board neighbours ignoring slot zero
//	int bd[2][21]={
//		{0,20,15,17,18,12,13,19,16,14,6,8,9,4,11,10,7,2,1,3,5},
//		{0,18,17,19,13,20,10,16,11,12,15,14,5,6,9,2,8,3,4,7,1} };
//	int r = rand()%100;
//	
//	if (r < 80)
//		return 2 * d;
//	else if (r < 85)
//		return 0;
//	else if (r < 90)
//		return d;
//	else if (r < 93)
//		return 2*bd[0][d];
//	else if (r < 96)
//		return 2*bd[1][d];
//	else if (r < 98)
//		return bd[0][d];
//	else 
//		return bd[1][d];
//}

//int throw_single(int d) {
//	
//	// return result of throwing for single d with accuracy 88% (or 80% for the outer)
//	
//	// board neighbours ignoring slot zero
//	int bd[2][21]={
//		{0,20,15,17,18,12,13,19,16,14,6,8,9,4,11,10,7,2,1,3,5},
//		{0,18,17,19,13,20,10,16,11,12,15,14,5,6,9,2,8,3,4,7,1} };
//	int r = rand()%100;
//	
//	if (d == 25) // outer 80%
//	{		
//		if (r < 80)
//			return 25;
//		else if (r < 90)
//			return 50;
//		else
//			return 1 + rand()%20;
//	}
//	else // 1 to 20 single
//	{
//		if (r < 88)
//			return d;
//		else if (r < 92)
//			return bd[0][d];
//		else if (r < 96)
//			return bd[1][d];
//		else if (r < 98)
//			return 3 * d;
//		else 
//			return 2 * d;
//	}
//}