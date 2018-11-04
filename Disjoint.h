#ifndef DISJOINT_H
#define DISJOINT_H

#include <iostream>
#include <set>
#include <vector>
#include <string.h>

using std::endl;
using std::cin;
using std::cout;
using std::set;
using std::vector;


/*
takes a 290 array representing a board
290 sets each with a adjacency list of max size 3
*/

struct Blob
{
	int id = -1;
	int color = -1;
	int size = -1;
	int parent = -1;

	set <int> adjacencyList;
};

//a node is a set of blobs
class Node
{
private:

	Blob board[290];//storage for blobs
	int color_Layout[290];

	int gCost;//cost so far to get to current board state
	int hCost;//heuristic cost
	int nodeid;//id for a node
	Node* parent = nullptr;


public:

	Node(int given_board[], int g, int h, int nid)//constructor
	{
		gCost = g;
		hCost = h;
		nodeid = nid;

		for (int i = 0; i < 290; i++)
		{
			board[i].id = i;
			board[i].color = given_board[i];
			board[i].size = 1;

			if (i > 9)
				board[i].adjacencyList.insert(i - 10);
			if (i < 280)
				board[i].adjacencyList.insert(i + 10);
			if (((i % 2 == 0) ^ (i / 10) % 2 == 0) && (i % 10 != 9))
				board[i].adjacencyList.insert(i + 1);
			if (!((i % 2 == 0) ^ (i / 10) % 2 == 0) && (i % 10 != 0))
				board[i].adjacencyList.insert(i - 1);
		}

		for(int i = 0; i < 290; i++)
		{
			for (std::set<int>::iterator it = board[i].adjacencyList.begin(); it != board[i].adjacencyList.end(); it++)
				merge(i, *it);
		}

		for (int i = 0; i < 290; i++)
			color_Layout[i] = getColor(i);
	}

	/*
	The color function changes the color of a blob and all adjacent blobs of the same color to one color.
	Merging happens when all adjacent blobs of the same color changes to the chosen color.

	merging when you color a blob a different if no merges happen do not
	*/
	void color(int id, int color_choice)//changes the color of a blob
	{

		int bptr = id;

		while (board[bptr].parent != -1)//find the parent
			bptr = board[bptr].parent;

		board[bptr].color = color_choice;

		for (std::set<int>::iterator it = board[bptr].adjacencyList.begin(); it != board[bptr].adjacencyList.end(); it++)
			merge(bptr, *it);

	}

	//use lowest id when 

	void merge(int blob1, int blob2)//merge two adjacent colors together
	{

		//blob1 follow parents till master parent is found
		while (board[blob1].parent != -1)
			blob1 = board[blob1].parent;

		//blob2 follow parents till master parent is found
		while (board[blob2].parent != -1)
			blob2 = board[blob2].parent;

		//compare parent sizes and attach the smaller to the larger
		if (board[blob1].size > board[blob2].size)
		{
			board[blob2].parent = blob1;
			board[blob1].size += board[blob2].size;
		}
		else
		{
			board[blob1].parent = blob2;
			board[blob2].size += board[blob1].size;
		}
		//put in logic to edit the adjacency list of the resulting blob****************** 

	}

	int* getColorLayout() const
	{
		int * rv = new int[290];
		memcpy(rv, color_Layout, 290);
		return rv;
	}

	vector<int> getBlobs() const
	{
		vector<int> parentIndexes;
		//std::vector<int>::iterator it;

		for (int i = 0; i < 290; i++)
		{
			if (board[i].parent == -1 && board[i].color != 0)
				parentIndexes.push_back(i);
		}

		return parentIndexes;
	}

	int getColor(int id) const
	{
		int bptr = id;

		while (board[bptr].parent != -1)
			bptr = board[bptr].parent;

		return board[bptr].color;
	}

	int getGCost() const
	{return gCost;}

	int getHCost() const
	{return hCost;}

	void setHCost(int h)
	{hCost = h;}

	int getNodeID() const
	{return nodeid;}

	Node* getParent()
	{return parent;}

	void setParent(Node* nptr)
	{parent = nptr;}

};

#endif