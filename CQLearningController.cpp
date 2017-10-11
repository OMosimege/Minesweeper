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
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		if (m_vecSweepers[sw]->isDead()) continue;
		SVector2D<int> current_state = m_vecSweepers[sw]->Position();
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
	
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		//3:::Observe new state:
		//TODO

		else if (m_vecSweepers[sw]->isDead()) { deadlist[sw] = true; }
		SVector2D<int> prevPosition = m_vecSweepers[sw]->PrevPosition();
		SVector2D<int> currentPosition = m_vecSweepers[sw]->Position();//currPosition
		prevPosition /= 10;
		currentPosition /= 10;
		int action = (int)m_vecSweepers[sw]->getRotation();
		//4:::Update _Q_s_a accordingly:
		//TODO
		freopen("CONOUT$", "w", stdout);
		//cout << "OLD VALUE" << qTables[prevPosition.x][prevPosition.y][action] << endl;
		theGrid[prevPosition.x][prevPosition.y][action] +=
			(lrate * (R(currPosition.x, currentPosition.y, sw) +
				(disc * bestValue(theGrid[currPosition.x][currentPosition.y])) -
				theGrid[prevPosition.x][prevPosition.y][action]));
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

