using namespace std;

// nodes for the maze, implemented together with the stack nodes
struct mNode{
	//checks if the current node has already visited or not while breaking the walls
	bool breakingVisited;
	//checks if the current node has already visited or not while finding the correct path
	bool findingVisited;
	// walls on each side of node, at UP DOWN LEFT RIGHT order
	bool walls[4];
	// pointers at each side of node
	mNode* up;
	mNode* down;
	mNode* left;
	mNode* right;
	// row and column that node belongs to
	int rowValue;
	int columnValue;
	// next node in stack
	mNode* next;
	// default constructor
	mNode();
};

// stack class
template <class o>
class Stack{
public:
	o* topStack;
	Stack();
	bool isEmpty() const;
	void pop();
	void push(o*);

}; 

