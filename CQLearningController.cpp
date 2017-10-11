/**
         (                                      
   (     )\ )                                   
 ( )\   (()/(   (    ) (        (        (  (   
 )((_)   /(_)) ))\( /( )(   (   )\  (    )\))(  
((_)_   (_))  /((_)(_)|()\  )\ |(_) )\ )((_))\  
 / _ \  | |  (_))((_)_ ((_)_(_/((_)_(_/( (()(_) 
| (_) | | |__/ -_) _` | '_| ' \)) | ' \)) _` |  
 \__\_\ |____\___\__,_|_| |_||_||_|_||_|\__, |  
                                        |___/   

Refer to Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
for a detailed discussion on Q Learning
*/
#include "CQLearningController.h"


CQLearningController::CQLearningController(HWND hwndMain):
	CDiscController(hwndMain),
	_grid_size_x(CParams::WindowWidth / CParams::iGridCellDim + 1),
	_grid_size_y(CParams::WindowHeight / CParams::iGridCellDim + 1)
{
}
/**
 The update method should allocate a Q table for each sweeper (this can
 be allocated in one shot - use an offset to store the tables one after the other)

 You can also use a boost multiarray if you wish
*/
void CQLearningController::InitializeLearningAlgorithm(void)
{
	//TODO
	vector<CDiscMinesweeper*> sweepers = m_vecSweepers; //allSweepers
	vector<CDiscCollisionObject*> objects = m_vecObjects; //objects

	for (int x = 0; x < _grid_size_x; x++)
	{
		std::vector<std::vector<double > > row;
		for (int y = 0; y < _grid_size_y; y++)
		{
			row.push_back({ 0.0, 0.0, 0.0, 0.0 });
		}
		theGrid.push_back(row);
	}
	allDead();
}
/**
 The immediate reward function. This computes a reward upon achieving the goal state of
 collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and 
 of course for hitting supermines/rocks!
*/
double CQLearningController::R(uint x,uint y, uint sweeperNo){ //sweeper_no
	//TODO: roll your own here!
	double reward = 0.0; //initilaize reward here
	int hit = (m_vecSweepers[sweeperNo])->CheckForObject(m_vecObjects, CParams::dMineScale);

	if (hit >= 0)
	{
		switch (m_vecObjects[hit]->getType()) {
		case CDiscCollisionObject::Mine:
		{
			if (!m_vecObjects[hit]->isDead())
			{
				return 100.0;
			}
			break;
		}
		case CDiscCollisionObject::Rock:
		{
			return -100.0;
			break;
		}
		case CDiscCollisionObject::SuperMine:
		{
			return -100.0;
			break;
		}
		}
	}

	return reward;
	//return 0;
}
/**
The update method. Main loop body of our Q Learning implementation
See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
*/
bool CQLearningController::Update(void)
{
	//m_vecSweepers is the array of minesweepers
	//everything you need will be m_[something] ;)
	uint cDead = std::count_if(m_vecSweepers.begin(),
							   m_vecSweepers.end(),
						       [](CDiscMinesweeper * s)->bool{
								return s->isDead();
							   });
	if (cDead == CParams::iNumSweepers){
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
	}

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		//1:::Observe the current state:
		//TODO
		//2:::Select action with highest historic return:
		//TODO
		//now call the parents update, so all the sweepers fulfill their chosen action
	}
	
	CDiscController::Update(); //call the parent's class update. Do not delete this.
	
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		//3:::Observe new state:
		//TODO
		//4:::Update _Q_s_a accordingly:
		//TODO
	}
	return true;
}

CQLearningController::~CQLearningController(void)
{
	//TODO: dealloc stuff here if you need to	
}

/*
	#include "CQLearningController.h"


CQLearningController::CQLearningController(HWND hwndMain) :
	CDiscController(hwndMain),
	_grid_size_x(CParams::WindowWidth / CParams::iGridCellDim + 1),
	_grid_size_y(CParams::WindowHeight / CParams::iGridCellDim + 1)
{
}
/**
The update method should allocate a Q table for each sweeper (this can
be allocated in one shot - use an offset to store the tables one after the other)

You can also use a boost multiarray if you wish
*/
/*void CQLearningController::InitializeLearningAlgorithm(void)
{
	vector<CDiscMinesweeper*> allSweepers = m_vecSweepers;
	vector<CDiscCollisionObject*> allObjects = m_vecObjects;
	//TODO
	//create the Q-tables for all sweepers and store them all in qTables:
	//for (int i = 0; i < allSweepers.size(); i++)
	//{

	for (int x = 0; x < _grid_size_x; x++)
	{
		std::vector<std::vector<double > > row;
		for (int y = 0; y < _grid_size_y; y++)
		{
			row.push_back({ 0.0, 0.0, 0.0, 0.0 });
		}
		theGrid.push_back(row);
	}
	allDead();

	//}
}
/**
The immediate reward function. This computes a reward upon achieving the goal state of
collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and
of course for hitting supermines/rocks!
*/
/*double CQLearningController::R(uint x, uint y, uint sweeper_no) {
	//TODO: roll your own here!

	double reward = 0.0;
	int hit = (m_vecSweepers[sweeper_no])->CheckForObject(m_vecObjects, CParams::dMineScale);

	if (hit >= 0)
	{
		switch (m_vecObjects[hit]->getType()) {
		case CDiscCollisionObject::Mine:
		{
			if (!m_vecObjects[hit]->isDead())
			{
				return 100.0;
			}
			break;
		}
		case CDiscCollisionObject::Rock:
		{
			return -100.0;
			break;
		}
		case CDiscCollisionObject::SuperMine:
		{
			return -100.0;
			break;
		}
		}
	}

	return reward;
}
/**
The update method. Main loop body of our Q Learning implementation
See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
*/
/*int nextMove(vector<double> moves)
{
	int dir = 0; //the direction of the default best move- forwards
	double value = moves[0];
	//double highestValue = actionValues[0];
	//the four is for east,west, south and north
	for (int i = 0; i < 4; i++)
	{

	if (moves[i] > value) {
		//if there is a move with a value greater than ours
		dir = i;
		value = moves[i]; //this now becomes our new value
	}
}
//What if more than one move has the max value
//we dont want them to all end at the same place, so we have to randomly assgin them
vector<int> bestmoves;
for (int b = 0; b < 4; b++)
{
	//now add all the moves with the same outval
	if (moves[b] == value) {
		bestmoves.push_back(b);//add this move 
	}
}
//then from the list of moves pick one randomly, this serves as to spaz up exploration
int bestdir = bestmoves[RandInt(0, bestmoves.size() - 1)];
//now return the best direction, and its value
return bestdir;
}

//helper method that just takes the first part of bestOfFour method above
int bestValue(std::vector<double> actions) {
	double highestValue = actions[0];
	for (uint a = 1; a < 4; a++) {
		if (actions[a] > highestValue) {
			highestValue = actions[a];
		}
	}
	return highestValue;
}

void CQLearningController::allDead()
{
	deadlist.clear();
	for (int i = 0; i < m_NumSweepers; i++)
	{
		deadlist.push_back(false);
	}
}

bool CQLearningController::Update(void)
{
	//m_vecSweepers is the array of minesweepers
	//everything you need will be m_[something] ;)
	uint cDead = std::count_if(m_vecSweepers.begin(),
		m_vecSweepers.end(),
		[](CDiscMinesweeper * s)->bool {
		return s->isDead();
	});
	if (cDead == CParams::iNumSweepers) {
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
		allDead();
	}

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw) {
		if (m_vecSweepers[sw]->isDead()) continue;
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		/*SVector2D<int> current_state = m_vecSweepers[sw]->Position();
		current_state /= 10; //
							 //2:::Select action with highest historic return:
		int dir = nextMove(theGrid[current_state.x][current_state.y]);
		//doing the damn thing!
		m_vecSweepers[sw]->setRotation((ROTATION_DIRECTION)dir);
		//1:::Observe the current state:
		//TODO
		//2:::Select action with highest historic return:
		//TODO
		//now call the parents update, so all the sweepers fulfill their chosen action
	}

	CDiscController::Update(); //call the parent's class update. Do not delete this.

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw) {
		if (m_vecSweepers[sw]->isDead() && deadlist[sw]) { continue; }
		else if (m_vecSweepers[sw]->isDead()) { deadlist[sw] = true; }
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		//3:::Observe new state:
		//TODO
		SVector2D<int> prevPosition = m_vecSweepers[sw]->PrevPosition();
		SVector2D<int> currPosition = m_vecSweepers[sw]->Position();
		prevPosition /= 10;
		currPosition /= 10;
		int action = (int)m_vecSweepers[sw]->getRotation();

		//4:::Update _Q_s_a accordingly:
		//TODO
		freopen("CONOUT$", "w", stdout);
		//cout << "OLD VALUE" << qTables[prevPosition.x][prevPosition.y][action] << endl;
		theGrid[prevPosition.x][prevPosition.y][action] +=
			(lrate * (R(currPosition.x, currPosition.y, sw) +
				(disc * bestValue(theGrid[currPosition.x][currPosition.y])) -
				theGrid[prevPosition.x][prevPosition.y][action]));
		//cout << "NEW VALUE" << qTables[prevPosition.x][prevPosition.y][action] << endl;
		//cout << endl;
		//qTables[prevPosition.x][prevPosition.y][action] = (learnR * (R(currPosition.x, currPosition.y, sw) + (discountF * bestValue(qTables[currPosition.x][currPosition.y]))));
	}
	if (m_iTicks == CParams::iNumTicks)
	{
		allDead();
	}
	return true;
}

CQLearningController::~CQLearningController(void)
{
	//TODO: dealloc stuff here if you need to	
}

*/