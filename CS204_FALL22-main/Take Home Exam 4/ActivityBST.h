#include <string>
#include <iostream>
using namespace std;

struct tree_node {
	int time;
	string activity;
	tree_node *right;
	tree_node *left;
	tree_node *parent;
	tree_node(int t, const string &a) : time(t), activity(a),
		right(nullptr), left(nullptr),
		parent(nullptr) {}
};



class ActivityBST{
private:
	tree_node* root;
public:
	ActivityBST();
	ActivityBST(const ActivityBST& copied);
	~ActivityBST();
	void add(int time, const string& activity);
	const ActivityBST& operator = (const ActivityBST& rhs);
	ActivityBST operator + (const ActivityBST& rhs) const;
	const ActivityBST& operator += (const ActivityBST& rhs);

	void deepCopyHelper(tree_node* temp);
	void addOperatorHelper(tree_node* temp);
	

friend ostream& operator << (ostream& os, const ActivityBST& rhs);
};


string fill_with_zeros(int num_zeros, const string& input);
string number_to_time(int number);
bool timeChecker(int time);
void destructorHelper(tree_node* root);
void printActivityHelper(tree_node* root, ostream& os);

