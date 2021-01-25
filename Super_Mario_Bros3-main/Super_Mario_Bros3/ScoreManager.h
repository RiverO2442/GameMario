#pragma once
class ScoreManager
{
	float x, y;
	int score;
public:

	ScoreManager(float x, float y, int score)
	{
		this->x = x;
		this->y = y;
		this->score = score;
	}
	float getScorePoisitionX()
	{
		return x;
	}
	float getScorePoisitionY()
	{
		return y;
	}
	int getScore()
	{
		return score;
	}
};

