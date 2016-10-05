#include "findpath.hpp"
#include <iostream>
#include <math.h>

#define DEBUG_LISTS 0
#define DEBUG_LIST_LENGTHS_ONLY 0

using namespace std;

// Global data

// The world map


int map[MAP_WIDTH * MAP_HEIGHT] = {
	
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	9,9,9,9,9,1,1,9,9,9,9,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,9,9,
	1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,9,9,9,9,9,9,9,9,9,9,9,9,1,1,9,9,9,9,9,9,9,9,9,9,9,9,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,1,
	9,9,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,1,1,
};

// map helper functions

int GetMap( int x, int y )
{

	if( x < 0 ||
	    x >= MAP_WIDTH ||
		 y < 0 ||
		 y >= MAP_HEIGHT
	  )
	{
		return 9;	 
	}

	return map[(y*MAP_WIDTH)+x];
}



// Definitions



bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{

	// same state in a maze search is simply when (x,y) are the same
	if( (x == rhs.x) &&
		(y == rhs.y) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	float xd = fabs(float(((float)x - (float)nodeGoal.x)));
	float yd = fabs(float(((float)y - (float)nodeGoal.y)));

	return xd + yd;
}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

	if( (x == nodeGoal.x) &&
		(y == nodeGoal.y) )
	{
		return true;
	}

	return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
{

	int parent_x = -1; 
	int parent_y = -1; 

	if( parent_node )
	{
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}
	

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards

	if( (GetMap( x, y+1 ) < 9) && !((parent_x == x) && (parent_y == y+1))) {
		NewNode = MapSearchNode( x, y+1 );
		astarsearch->AddSuccessor( NewNode );
	}
	if( (GetMap( x-1, y ) < 9) && !((parent_x == x-1) && (parent_y == y)) )  {
		NewNode = MapSearchNode( x-1, y );
		astarsearch->AddSuccessor( NewNode );
	}
	if( (GetMap( x+1, y ) < 9) && !((parent_x == x+1) && (parent_y == y)) ) {
		NewNode = MapSearchNode( x+1, y );
		astarsearch->AddSuccessor( NewNode );
	}
	if( (GetMap( x, y-1 ) < 9)  && !((parent_x == x) && (parent_y == y-1)) ) {
		NewNode = MapSearchNode( x, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}

	/*
	if( (GetMap( x-1, y-1 ) < 9) && !((parent_x == x-1) && (parent_y == y-1)) ) {
		NewNode = MapSearchNode( x-1, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}	
	if( (GetMap( x+1, y+1 ) < 9) && !((parent_x == x+1) && (parent_y == y+1))) {
		NewNode = MapSearchNode( x+1, y+1 );
		astarsearch->AddSuccessor( NewNode );
	}
	if( (GetMap( x+1, y-1 ) < 9) && !((parent_x == x+1) && (parent_y == y-1)) ) {
		NewNode = MapSearchNode( x+1, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}	
	if( (GetMap( x+1, y-1 ) < 9) && !((parent_x == x+1) && (parent_y == y-1)) )  {
		NewNode = MapSearchNode( x+1, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}
	*/

	return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	return (float) GetMap( x, y );

}
