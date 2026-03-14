#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

// AHMET ALPEREN GUNGOR - 28847

//node's constructor
mNode::mNode() : up(nullptr), down(nullptr), left(nullptr), right(nullptr), breakingVisited(false), findingVisited(false), next(nullptr){
	walls[0] = true;
	walls[1] = true;
	walls[2] = true;
	walls[3] = true;
}

//stack class's constructor that sets the current top element to nullptr
template <typename o>
Stack<o>::Stack(): topStack(nullptr){}

//stack class's isEmpty function that checks whether the stack is currently empty or not
template <typename o>
bool Stack<o>::isEmpty() const{return topStack==nullptr;}

//stack class's pop function that removes the top element from the stack
template <typename o>
void Stack<o>::pop(){
	if (isEmpty()){
		cout << "Empty stack" << endl;
	}
	else{
		topStack = topStack->next;
	}
}

//stack class's push function that inserts the new element at the top of the stack
template <typename o> 
void Stack<o>::push(o* theElement){
	o* temp = theElement;
	if (isEmpty()){
		topStack = temp;
		topStack->next = nullptr;
	}
	else{
		temp->next = topStack;
		topStack = temp;
	}
}

//initial maze with no walls broken yet
mNode* initialMaze(int M, int N){
	//first create 0,0 called head which is placed at bottom left (therefore no connection with bottom and left)
	mNode* head = new mNode();
	mNode* tempHead = head; // will move upwards for each row in first column
	mNode* tempHorizontal = tempHead; // will move horizontal for each column in rows 
	mNode* tempDownIterator = tempHead; // will iterate through 1 row below (except for the first row) and connect up/down nodes

	// creating the base (bottom row)
	for (int ctr = 0; ctr< N; ctr++){
		if (ctr!= N-1){

			//create right node and connect left and right nodes together  
			tempHorizontal->right = new mNode();
			tempHorizontal->right->left = tempHorizontal;
		}
		//assign col and row values
		tempHorizontal->columnValue = ctr;
		tempHorizontal->rowValue = 0;
		//move iterator to the next (right) node
		tempHorizontal = tempHorizontal->right;
	}

	// creating the rest of the maze (except first bottom row, which was created above)
	for (int ctr1 = 1; ctr1 < M; ctr1++){ //iterate for each row
		tempHead->up = new mNode(); // create a new node above in the first column
		tempDownIterator = tempHead; // tempdowniterator is currently one row below and will start iterating this row
		tempHead = tempHead->up; // move vertical first column iterator to the new row
		tempHead->columnValue = 0; 
		tempHead->rowValue = ctr1;
		tempHorizontal = tempHead; // assign horizontal iterator to the current rows first column node
		for (int ctr2 = 0; ctr2 < N; ctr2++){ //iterate for each column
			if (ctr2!=N-1){ //N-1 is the rightmost node 
				tempHorizontal->right = new mNode();
				tempHorizontal->right->left = tempHorizontal;
			}
			tempHorizontal->columnValue = ctr2;
			tempHorizontal->rowValue = ctr1;
			// connect upper and lower layer
			tempHorizontal->down = tempDownIterator;
			tempHorizontal->down->up = tempHorizontal;
			// move both iterators to the right
			tempHorizontal = tempHorizontal->right;
			tempDownIterator = tempDownIterator->right;
		}
	}
	return head;
}

//break the walls in order to create a unique maze
template <typename o>
o* wallBreaker(o* head, int M, int N){
	// since regular rng function does not work properly because of seeding restrictions, found the following Mersenne Twister algorithm code on the internet
    random_device rd;
    mt19937 rng(rd());
	// storing the number of total nodes and nodes that are currently visited in order to find a solution with loops instead of recursion
	int totalNodes = M * N;
	int currentVisited = 1;
	o* tempHead = head;
	tempHead->breakingVisited = true;
	Stack<o> stack;
	stack.push(tempHead);
	while (totalNodes!=currentVisited){
		// checking if the top element in the stack has 0 walls (all walls are already destructed)
		bool totallyBroken = false;
		vector<int> rollable; // will store the directions that we can roll (restrictions are given in the below if statements)
		if (stack.topStack->walls[0] == 0 && stack.topStack->walls[1] == 0 && stack.topStack->walls[2] == 0 && stack.topStack->walls[3] == 0){
			totallyBroken = true;
		}
		if(totallyBroken){ // if so, then pop the top element and start the iteration again
			stack.pop();
		}
		else{
			vector<int> rollable;
			int randomNumber;
			int breakable=0; // number of breakable balls, 1 for each side if both there exists a neighbouring node there & it is not visited yet & wall between not broken
			// reminder, walls is in 0)up 1)down 2)left 3)right order

			if (stack.topStack->up != nullptr && stack.topStack->up->breakingVisited == false && stack.topStack->walls[0] == true){ // if the current node has a node above which is not visited yet & there is a wall between
				breakable++; //then breakable
				rollable.push_back(0);
			}
			else if (stack.topStack->up==nullptr){
				breakable = breakable;
			}

			if (stack.topStack->down != nullptr && stack.topStack->down->breakingVisited == false && stack.topStack->walls[1] == true){ // if the current node has a node below which is not visited yet & there is a wall between
				breakable++; //then breakable
				rollable.push_back(1);
			}
			else if (stack.topStack->down==nullptr){
				breakable = breakable;
			}
			if (stack.topStack->left != nullptr && stack.topStack->left->breakingVisited == false && stack.topStack->walls[2] == true){ // if the current node has a node left which is not visited yet & there is a wall between
				breakable++; //then breakable
				rollable.push_back(2);
			}
			else if (stack.topStack->left==nullptr){
				breakable = breakable;
			}
			if (stack.topStack->right != nullptr && stack.topStack->right->breakingVisited == false && stack.topStack->walls[3] == true){ // if the current node has a node right which is not visited yet & there is a wall between
				breakable++; //then breakable
				rollable.push_back(3);
			}
			else if (stack.topStack->right==nullptr){
				breakable = breakable;
			}
			else{
				breakable = breakable;
			}

			if(breakable!=0){
				if (breakable==1){
					randomNumber = 0;
				}
				else if(breakable ==2){
					uniform_int_distribution<int> dist(0, 1); // random number between 0 and breakable, where breakable represent number of possible paths
					randomNumber = dist(rng);
				}
				else if(breakable ==3){
					uniform_int_distribution<int> dist(0, 2); // random number between 0 and breakable, where breakable represent number of possible paths
					randomNumber = dist(rng);
				}
				else if (breakable ==4){
					uniform_int_distribution<int> dist(0, 3); // random number between 0 and breakable, where breakable represent number of possible paths
					randomNumber = dist(rng);
				}
				else{
					randomNumber = randomNumber;
				}

				currentVisited++;
				int luckyPath = rollable[randomNumber];
				stack.topStack->walls[luckyPath] = false; // break the wall of the current top node

				
				if (luckyPath==0){
					stack.push(stack.topStack->up); // push the new node to the top of the stack
					stack.topStack->walls[1] = false; // break the wall of the new node
					stack.topStack->breakingVisited = true; // now this node is visited
				}
				else if (luckyPath==1){
					stack.push(stack.topStack->down); // push the new node to the top of the stack
					stack.topStack->walls[0] = false; // break the wall of the new node
					stack.topStack->breakingVisited = true; // now this node is visited
				}
				else if (luckyPath==2){
					stack.push(stack.topStack->left); // push the new node to the top of the stack
					stack.topStack->walls[3] = false; // break the wall of the new node
					stack.topStack->breakingVisited = true; // now this node is visited
				}
				else if (luckyPath==3){
					stack.push(stack.topStack->right); // push the new node to the top of the stack
					stack.topStack->walls[2] = false; // break the wall of the new node
					stack.topStack->breakingVisited = true; // now this node is visited
				}
				else{
					breakable = breakable;
				}
			}
			else{
				stack.pop();
			}
		}
	}
	return head;
}

// function to find the path with given inputs and outputs
template <typename o>
void pathFinder(o* head, int xEntry, int yEntry, int xExit, int yExit, int mazeNum){
	random_device rd;
    mt19937 rng(rd());
	uniform_int_distribution<int> dist(0, 3);

	// moving head to the destinated entry point
	for (int ctr1 = 0; ctr1<xEntry; ctr1++){
		head = head->right;
	}
	for (int ctr2 = 0; ctr2< yEntry; ctr2++){
		head = head->up;
	}

	Stack<o> stack;
	stack.push(head);

	// iterate until destination node is reached with correcet row and column values
    while(!(stack.topStack->rowValue==yExit&&stack.topStack->columnValue==xExit)){
        vector<o *> possiblePaths; // vector to store nodes, where possible going directions will be pushed depending on some conditions
        bool notRestricted = true;
        for (int i = 0; i < 4; i++){
            if (!stack.topStack->walls[i]) {notRestricted = false;}
        }
        if (notRestricted){stack.pop();}
        else{

            int totalNhbCount = 0; // total number of neighbours
            int visitedNhbCount = 0; // total number of neighbours that are already visited
            int findable=0; // possible paths, which requires to have a neighbour there, this neighbour node is not visited and also there isnt a wall between

            if (stack.topStack->up){// check the upper 
                possiblePaths.push_back(stack.topStack->up);
                if (stack.topStack->up->findingVisited) {visitedNhbCount++;}
                else if (!stack.topStack->walls[0]){findable++;}
				totalNhbCount++;
            }
            else {possiblePaths.push_back(nullptr);}

            if (stack.topStack->down){// check the lower 
                possiblePaths.push_back(stack.topStack->down);
                if (stack.topStack->down->findingVisited) {visitedNhbCount++;}
                else if (!stack.topStack->walls[1]){findable++;}
				totalNhbCount++;
            }
            else {possiblePaths.push_back(nullptr);}

            if (stack.topStack->left){// check the left node
                possiblePaths.push_back(stack.topStack->left);
                if (stack.topStack->left->findingVisited){
                    visitedNhbCount++;
                }
                else if (!stack.topStack->walls[2]){
                    findable++;
                }
				totalNhbCount++;
            }
            else{
                possiblePaths.push_back(nullptr);
            }
            if (stack.topStack->right){// check the right node
                possiblePaths.push_back(stack.topStack->right);
                if (stack.topStack->right->findingVisited){
                    visitedNhbCount++;
                }
                else if (!stack.topStack->walls[3]){
                    findable++;
                }
				totalNhbCount++;
            }
            else{
                possiblePaths.push_back(nullptr);
            }
            if (findable != 0){  // if there is at least 1 possible path   
                int randomNumberPath = dist(rng);
                if (possiblePaths[randomNumberPath]){
					if (!stack.topStack->walls[randomNumberPath]){                        
						if(!possiblePaths[randomNumberPath]->findingVisited)
						{
							stack.push(possiblePaths[randomNumberPath]);
							possiblePaths[randomNumberPath]->findingVisited =true;                       
						}
						else{
							if (visitedNhbCount == totalNhbCount)
							{
								stack.pop();
							}                   
						}
                    }

                    else{
                        if (visitedNhbCount == totalNhbCount)
                        {
                            stack.pop();
                        }
                    }
                }
                else{
                        if (visitedNhbCount == totalNhbCount){
                            stack.pop();
                        }
                    }
            }
            else{
                stack.pop();
            }
        }

    }
	//the stack that was created above consist of paths that were taken to reach the destination point, row and col values of these paths will be recorded below
	vector<string> finalString;
    while(!stack.isEmpty()){
        o*temp=stack.topStack;
        stack.pop();
        string tempString=to_string(temp->columnValue)+' '+to_string(temp->rowValue);
        finalString.push_back(tempString);
    }
	//creating the txt name
    string txtName="maze_"+to_string(mazeNum)+"_path_"+to_string(xEntry)+"_"+to_string(yEntry)+"_"+to_string(xExit)+"_"+to_string(yExit)+".txt";
    ofstream writing(txtName);
    for(int i=finalString.size()-1;i>=0;i--){
        writing<<finalString[i]<<endl;
    }
	
}






/*
int main(){
	
	//the following comment block is for initial maze structure check: upper and lower links + row&col numbers
	int M = 2;
	int N = 4;
	mNode* realHead= initialMaze(M,N);
	mNode* tempHeadChecker = realHead;
	
	for (int ctr1 = 0; ctr1 < M; ctr1++){ //this part checks the integrity of left and right links as well as row&col numbers
		mNode* tempRowChecker = tempHeadChecker;
		for (int ctr2 = 0; ctr2 < N; ctr2++){
			cout<<tempRowChecker->rowValue<<tempRowChecker->columnValue<<endl;
			tempRowChecker = tempRowChecker->right;
		}
		tempHeadChecker=tempHeadChecker->up;
	}
	
	for (int ctr1 = 0; ctr1< N; ctr1++){ //this part checks the integrity of up and down links as well as row&col numbers
		mNode* tempColChecker = tempHeadChecker;
		for (int ctr2 = 0; ctr2 < M; ctr2++){
			cout<<tempColChecker->rowValue<<tempColChecker->columnValue<<endl;
			tempColChecker = tempColChecker->up;
		}
		tempHeadChecker = tempHeadChecker->right;
	}
}*/

int main()
{   
    int K, M, N;
	int mazeIn;
    int xEntry;
    int yEntry;
    int xExit;
    int yExit;
	int counter=0;
    std::cout<<"Enter the number of mazes: ";
    cin >> K;
    std::cout<<"Enter the number of rows and columns (M and N): ";
    cin>> M >> N;
    vector<mNode*>mazes;
    while(counter<K){
        mNode *maze = new mNode();
        maze=initialMaze( M, N);
        mNode *pathMaze = wallBreaker(maze, M, N);
        mazes.push_back(pathMaze);
        string text_name="maze_"+to_string(counter+1)+".txt";
        ofstream w(text_name);
        w<<M<<" "<<N<<endl;
        mNode* temp=pathMaze;
        for(int i = 0 ; i < M;i++){
			for(int j = 0 ; j <N;j++){
				string walls_val[4];
				for(int k = 0 ; k <4;k++){
					if(temp->walls[k]){
						walls_val[k] = "1";
					}
					else{
						walls_val[k] = "0";
					}
				}
				w<<"x="<<temp->columnValue<<" y="<<temp->rowValue<<" l="<<walls_val[2]<<" r="<<walls_val[3]<<" u="<<walls_val[0]<<" d="<<walls_val[1]<<endl;
				if(j!=N-1){
					temp=temp->right;
				}
			}
        if(i!=M-1){
            temp=pathMaze;
            for(int l=0;l<=i;l++){
                temp=temp->up;
            }
        }
    }
        counter++;
    }
    std::cout<<"All mazes are generated."<<endl<<endl;

    std::cout<<"Enter a maze ID between 1 to "<<to_string(K)<<" inclusive to find a path: ";
    cin>>mazeIn;

    std::cout<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >>xEntry >> yEntry ;

    std::cout<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin>> xExit >> yExit;

    pathFinder(mazes[mazeIn-1],xEntry,yEntry,xExit,yExit,mazeIn);
    std::cout<<"Finished"<<endl;
    return 0;
}