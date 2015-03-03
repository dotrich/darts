#ifndef _DARTS_H
#define _DARTS_H

#include <string>
#include <cstdint>

int clamp(uint32_t cur, uint32_t min, uint32_t max);

class Darts
{
private:
	enum state {SLOW, FAST, FOCUS, OUT};
	uint32_t score_; // starting score
	uint16_t plyrNo_; // no of players
	uint32_t gameNo_; // no of games played

	uint32_t* thrCount;
	
	int single(uint8_t); // non-bullseye darts
	int bull(uint8_t); // bullseye darts	
	int game(uint8_t); // handles one game at a time

public:
	int getScore();
	int getNoPlayers();
	int getNoGames();

	std::string* plyrName[];
	
	Darts::Darts() :
		score_(101),
		plyrNo_(1),
		gameNo_(1000),

		thrCount(NULL)
	{	
	}

	void setScore(uint32_t);
	void setNoPlayers(uint16_t);
	void setNoGames(uint32_t);

	~Darts();

	void playGame(); // main function for playing
};

#endif