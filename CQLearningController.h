#pragma once
#include "cdisccontroller.h"
#include "CParams.h"
#include "CDiscCollisionObject.h"
#include <cmath>

typedef unsigned int uint;
class CQLearningController :
	public CDiscController
{
private:
	uint _grid_size_x;
	uint _grid_size_y;

	std::vector<std::vector<std::vector<double> >> theGrid;
	std::vector<bool> deadlist;//update Q-table when player dies
	void allDead();

	//discount and learning rate
	double disc = 0.8;
	double lrate = 1.0;

public:
	CQLearningController(HWND hwndMain);
	virtual void InitializeLearningAlgorithm(void);
	double R(uint x, uint y, uint sweeper_no); //move
	virtual bool Update(void);
	virtual ~CQLearningController(void);
};

