#ifndef FIND_PATH_H
#define FIND_PATH_H

#include "stlastar.h"

const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 16;

int GetMap(int x,int y);

class MapSearchNode {
public:
	unsigned int x;	 // the (x,y) positions of the node
	unsigned int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py ) { x=px; y=py; }

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );
};

#endif
