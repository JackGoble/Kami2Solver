#ifndef DISJOINT_H
#define DISJOINT_H

#include <iostream>
#include <set>
#include <vector>

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


public:

	/*
	The color function changes the color of a blob and all adjacent blobs of the same color to one color.
	Merging happens when all adjacent blobs of the same color changes to the chosen color.

	merging when you color a blob a different if no merges happen do not
	*/
	void color(int id, int color_choice)//changes the color of a blob
	{
		Blob *bptr = &board[id];

		while (bptr->parent != -1)//find the parent
			bptr = &board[bptr->parent];

		bptr->color = color_choice;

		for (std::set<int>::iterator it = bptr->adjacencyList.begin(); it != bptr->adjacencyList.end(); it++)
			merge(*bptr, board[*it]);

	}

	Node(int given_board[], int g, int h, int nid)//constructor
	{
		gCost = g;
		hCost = h;
		nodeid = nid;

		for (int i = 0; i < 290; i++)
		{
			board[i].id = i;
			board[i].color = given_board[i];
			color_Layout[i] = given_board[i];
			board[i].size = 1;

			if (i > 9)
				board[i].adjacencyList.insert(i - 10);
			if(i < 280)
				board[i].adjacencyList.insert(i + 10);
			if(((i % 2 == 0) ^ (i / 10) % 2 == 0) && (i % 10 != 9))
				board[i].adjacencyList.insert(i + 1);
			else if (i % 10 != 0)
				board[i].adjacencyList.insert(i - 1);
		}

		std::cout << "node 0 adj: ";
			for (std::set<int>::iterator it = board[0].adjacencyList.begin(); it != board[0].adjacencyList.end(); it++)
				std::cout << *it << " ";
	}


	//use lowest id when 
	void merge(Blob blob1, Blob blob2)//merge two adjacent colors together
	{
		Blob *bptr1, *bptr2;

		bptr1 = &blob1;
		bptr2 = &blob2;

		//if they are not the same color, exit early since a merge cannot happen
		if (blob1.color != blob2.color)
			return;

		//blob1 follow parents till master parent is found
		while (bptr1->parent != -1)
			bptr1 = &board[bptr1->parent];

		//blob2 follow parents till master parent is found
		while (bptr2->parent != -1)
			bptr2 = &board[bptr2->parent];

		//compare parent sizes and attach the smaller to the larger
		if (bptr1->size > bptr2->size)
		{
			bptr2->parent = bptr1->id;
			bptr1->size += bptr2->size;
		}
		else
		{
			bptr1->parent = bptr2->id;
			bptr2->size += bptr1->size;
		}

	}

	int* getColorLayout()
	{return color_Layout;}

	vector <int> getParents()
	{
		vector <int> parentIndexes;
		std::vector <int>::iterator it;

		for (int i = 0; i < 290; i++)
		{
			if (board[i].parent = -1)
				parentIndexes.insert(it,i);
		}
	}

	int getColor(int id)
	{
		Blob* bptr = &board[id];

		while (bptr->parent != -1)
			bptr = &board[bptr->parent];

		return bptr->color;
	}

	int getGCost()
	{return gCost;}

	int getHCost()
	{return hCost;}

	int getNodeID()
	{return nodeid;}
};

#endif