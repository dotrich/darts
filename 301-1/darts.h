#ifndef _DARTS_H
#define _DARTS_H

#include <cstdint>

uint32_t clamp(uint32_t cur, uint32_t min, uint32_t max);

class Darts
{
private:
	enum state {SLOW, FAST, FOCUS, OUT};
	uint32_t score_; // starting score
	uint16_t plyrNo_; // no of players
	uint32_t gameNo_; // no of games played

	uint32_t* thrCount;
	
	uint16_t single(uint16_t); // non-bullseye darts
	uint16_t bull(uint16_t); // bullseye darts	
	uint16_t game(uint16_t); // handles one game at a time

public:
	uint32_t getScore();
	uint16_t getNoPlayers();
	uint32_t getNoGames();
	
	Darts::Darts() :
		score_(101),
		plyrNo_(1),
		gameNo_(1000),

		thrCount(nullptr)
	{	
	}

	void setScore(uint32_t);
	void setNoPlayers(uint16_t);
	void setNoGames(uint32_t);

	~Darts();

	void playGame(); // main function for playing
};

#endif