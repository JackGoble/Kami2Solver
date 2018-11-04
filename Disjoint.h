#ifndef DISJOINT_H
#define DISJOINT_H

#include <iostream>
#include <set>

/*
takes a 290 array representing a board
290 sets each with a adjacency list of max size 3
*/

struct Blobs
{
	int id = -1;
	int color = -1;
	int size = -1;
	int parent = -1;
};

//a node is a set of blobs
class Node
{
public:

	Node(int board[])//constructor
	{
		for (int i = 0; i < 290; i++)
		{
			board[i] = 
		}
	}


	//use lowest id when 
	void merge();//merge two adjacent colors together

private:

	Blobs board[290];//storage for blobs

	//merging when you color a blob a different if no merges happen do not
	void color()//changes the color of a blob
	{

	}

	getAdjacent()
	{

	}


};

#endif