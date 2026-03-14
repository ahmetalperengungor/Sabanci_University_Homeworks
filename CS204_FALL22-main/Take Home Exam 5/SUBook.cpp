#include "SUBook.h"

//User class functions:

// Constructor of User, takes string parameter and sets users name, also sets number of comments to 0
User::User(string inName){
	name = inName;
	num_comments = 0;
}

// Desturctor of User, deletes posts of user
User::~User(){
	for (int d = 0; d<posts.size(); d++){
		delete posts[d];
	}
	posts.clear();
}

//User function that creates post 
Post& User::createPost(string content){
	Post* newPostPtr = new Post(*this, content); //post pointer pointing to a new Post object 
	posts.push_back(newPostPtr); // adding this pointer to vector of posts
	return *newPostPtr; // returning the post pointers value
}

// User function that returns vector of post pointers
vector<Post*> User::getPosts(){
	return posts;
}

// User function that returns number of comments
int User::getNumComments(){
	return num_comments;
}

// User function that returns users name
string User::getName(){
	return name;
}

//Post class functions:
// Constructor of Post, with user inUser and content inContent
Post::Post(User& inUser, string inContent): user(&inUser), content(inContent){
}

// Destructor of Post, deletes Comment object pointer
Post::~Post(){
	for (int d = 0; d<comments.size(); d++){
		delete comments[d]; 
	}
	comments.clear();
}

// Post function that adds comment with given comment and by User commenter
void Post::addComment(string content, User* commenter){
	Comment* newCommentPtr = new Comment(commenter, this, content); // comment pointer points to a new comment object using constructor of comment
	comments.push_back(newCommentPtr); // add comment pointer to the vector of comment pointers
	commenter->num_comments+=1; // increase commenters number of comments by 1 
}

// Post function that returns vector of comment pointers
vector<Comment*> Post::getComments(){
	return comments;
}

// Post function that returns the user of the post
User& Post::getUser(){
	return *user;
}

// Post function that returns the content of the post
string Post::getContent(){
	return content;
}

//Comment class functions:
// Constructor of Comment class, sets commenter to inUser, post to inPost and content to inContent
Comment::Comment(User* inUser, Post* inPost, string inContent): user(inUser), post(inPost), content(inContent){
}

// Destructor of Comment class, decrements number of comments by 1 
Comment::~Comment(){
	if (user!=nullptr){
		user->num_comments-=1;
	}
}

// Comment function, gets the user of commenter
User& Comment::getUser(){
	return *user;
}

// Comment function, gets the content of comment
string Comment::getContent(){
	return content;
}

// Comment function, gets the post comment is made on
Post& Comment::getPost(){
	return *post;
}


// Ahmet Alperen Gungor - 28847