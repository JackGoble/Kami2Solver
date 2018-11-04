/*
Name: Jackson Goble
Class: CSCI4350
Assignment: OLA1
Due date: 10/2/2018

Description: This program is used in conjuntion with the output from
    random_board to solve the 8-puzzle problem using A* with 4 
    different heuristic functions.

Usage 1 (this program only)
    : make
    : ./a-star heuristic

Usage 2 (this program from a random board)
    : make
    : cat OLA1-input.txt | ./random_board seed moves | ./a-star heuristic

Usage 3 (this program 100 times for each of the 4 heuristics, writing
         the output to files stored in the /outfiles directory)
    : make
    : ./dataget
*/

#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <algorithm>

using std::endl;
using std::cin;
using std::cout;
using std::vector;
using std::set;

//the following constants are available to run the a-star algorithm
//on different sized puzzles.
static const int board_height = 4;
static const int board_width = 4;
static const int board_size = board_width*board_height;
static const int solution[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

struct Node {
    int id;   //id is unique to each node upon creation
    int g, h; //step cost is 1, so g is essentially the depth
    int b[board_size];
    int r0, c0;

    const Node * parent;
};

//comparison class for the closed list, considers two nodes
//to be the same if their b[] (board states) are equal
struct closedComp {
    bool operator() (const Node& lhs, const Node& rhs) const {
        return std::lexicographical_compare(lhs.b, lhs.b+board_size-1, 
                                            rhs.b, rhs.b+board_size-1);
    }
};

//comparison class for the open list, breaks ties with node id
//places nodes with a higher id (newer) as a lower value
//in the set (higher priority)
struct openComp {
    bool operator() (const Node& lhs, const Node& rhs) const {
        if(lhs.g+lhs.h == rhs.g+rhs.h) return lhs.id < rhs.id;
        return lhs.g+lhs.h < rhs.g+rhs.h;
    }
};

//A* Tree class
class AST {
    public:
        AST(int[board_size], int(*)(const Node&));
        void solve();

    private:
        Node *root;
        int (*hf)(const Node&); //pointer to heuristic function to use

        set<Node,openComp> OPEN;
        set<Node,closedComp> CLOSED;

        int next_id;    //next unique node id to give out
        int V, d;       //number of nodes (V)isited and (d)epth
        unsigned int N; //max (N)umber of nodes in memory at once
        float b;        //approximate effective (b)ranching factor N=b^d

        void expand(const Node&); //expands a node into a max of 4 new nodes
};

//constructor for A* Tree takes the problem (B)oard and a pointer to
//which heuristic to use
AST::AST(int B[board_size], int(*func)(const Node&)) {
    hf = func;
    root = new Node;
    next_id = 1;
    N = 0;
   
    //set all appropriate values for the root node (the problem state)
    for(int i = 0; i < board_height; i++) {
        for(int j = 0; j < board_width; j++) {
            int index = i*board_width+j;
            root->b[index] = B[index];
            if(root->b[index] == 0) {
                root->r0 = i;
                root->c0 = j;
            }
        }
    }
    root->h = hf(*root);
    root->parent = NULL;

    //and instert it into the open list
    OPEN.insert(*root);

    V = 1;
}

void AST::expand(const Node& node) {
    Node *tmp = new Node;
    if(node.r0 > 0) { //if space can move up
        *tmp = node;
        tmp->id = next_id++;
        tmp->g = node.g + 1;
        tmp->b[node.r0*board_width+node.c0] = 
                tmp->b[(node.r0-1)*board_width+node.c0];
        tmp->b[(node.r0-1)*board_width+node.c0] = 0;
        tmp->r0 = node.r0 - 1;
        tmp->h = hf(*tmp);
        tmp->parent = &node;
        OPEN.insert(*tmp);
    }
    if(node.r0 < board_height-1) { //if space can move down
        *tmp = node;
        tmp->id = next_id++;
        tmp->g = node.g + 1;
        tmp->b[node.r0*board_width+node.c0] = 
                tmp->b[(node.r0+1)*board_width+node.c0];
        tmp->b[(node.r0+1)*board_width+node.c0] = 0;
        tmp->r0 = node.r0 + 1;
        tmp->h = hf(*tmp);
        tmp->parent = &node;
        OPEN.insert(*tmp);
    }
    if(node.c0 > 0) { //if space can move left
        *tmp = node;
        tmp->id = next_id++;
        tmp->g = node.g + 1;
        tmp->b[node.r0*board_width+node.c0] = 
                tmp->b[node.r0*board_width+node.c0-1];
        tmp->b[node.r0*board_width+node.c0-1] = 0;
        tmp->c0 = node.c0 - 1;
        tmp->h = hf(*tmp);
        tmp->parent = &node;
        OPEN.insert(*tmp);
    }
    if(node.c0 < board_width-1) { //if space can move right
        *tmp = node;
        tmp->id = next_id++;
        tmp->g = node.g + 1;
        tmp->b[node.r0*board_width+node.c0] = 
                tmp->b[node.r0*board_width+node.c0+1];
        tmp->b[node.r0*board_width+node.c0+1] = 0;
        tmp->c0 = node.c0 + 1;
        tmp->h = hf(*tmp);
        tmp->parent = &node;
        OPEN.insert(*tmp);
    }
    delete tmp;
    tmp = NULL;
    if(CLOSED.size() + OPEN.size() > N) N = CLOSED.size() + OPEN.size();
    V++;
}

void AST::solve() {
    set<Node,openComp>::iterator sit;

    sit = OPEN.begin();

    //while the node at sit is not the solution
    while(!std::equal(sit->b, sit->b+board_size-1, 
                    solution, solution+board_size-1)) {

        //if sit is not in the closed list
        if(!CLOSED.count(*sit)) {
            
            //add it, and then expand it
            CLOSED.insert(*sit);
            
            expand(*CLOSED.find(*sit)); 
            //the above line was the hardest line ever
            //initally I had "expand(*sit);" which works fine
            //for performing A*, but not for keeping track of
            //the path
        }
        OPEN.erase(sit);
        sit = OPEN.begin();
        
    }

    //solution is found, so we now know d and b
    d = sit->g;
    b = log(N/d);

    //print out statistics
    cout << "V=" << V << endl;
    cout << "N=" << N << endl;
    cout << "d=" << d << endl;
    cout << "b=" << b << endl << endl;


    //print out path in reverse order
    const Node* path = &(*sit);
    vector<Node> r_path;

    while(path != NULL) {
        r_path.push_back(*path);
        path = path->parent;
    }

    for(int f = d; f >= 0; f--) {
        for(int i = 0; i < board_height; i++) {
            for(int j = 0; j < board_width; j++) {
                cout << r_path[f].b[i*board_height+j];
                if(j < board_width-1) cout << " ";
                else cout << endl;
            }
        }
        cout << endl;
    }
    return;
}

//h = 0
int h0(const Node& node) {
    return 0;
}

//h = number of misplaced tiles
int h1(const Node& node) {
    int h_val = 0;
    for(int i = 0; i < board_height; i++) {
        for(int j = 0; j < board_width; j++) {
            if(node.b[i*board_width+j] != 0 && 
                node.b[i*board_width+j] != i*board_width+j ) h_val++;
        }
    }
    return h_val;
}

//h = sum of Manhattan square distances
int h2(const Node& node) {
    int h_val = 0;
    for(int i = 0; i < board_height; i++) {
        for(int j = 0; j < board_width; j++) {
            int val = node.b[i*board_width+j];
            if(val != 0)  {
                h_val += abs( (val / board_width) - i);
                h_val += abs( (val % board_width) - j);
            }
        }
    }

    return h_val;
}

//h = sum of number of tiles in wrong row plus number 
//of tiles in wrong column (should fall between h1 and h2)
int h3(const Node& node) {
    int h_val = 0;
    for(int i = 0; i < board_height; i++) {
        for(int j = 0; j < board_width; j++) {
            int val = node.b[i*board_width+j];
            if(val != 0)  {
                h_val += (val / board_width) != i;
                h_val += (val % board_width) != j;
            }
        }
    }
    return h_val;
}

int main(int argc, char* argv[]) {
    int input_board[board_size];
    int i = 0;
    int (*f)(const Node&);
    
    switch ( atoi(argv[1]) ) {
        case 0:
            f = &h0;
            break;
        case 1:
            f = &h1;
            break;
        case 2:
            f = &h2;
            break;
        case 3:
            f = &h3;
            break;
    }

    //get the starting board state from std input
    while(cin >> input_board[i]) i++;

    AST myAST(input_board, f);

    myAST.solve();

    return 0;
}
