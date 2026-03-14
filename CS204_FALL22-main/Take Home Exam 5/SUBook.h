#include <iostream>
#include <string>
#include <vector>

using namespace std;

//User class
class User{
friend class Post;
friend class Comment;
private:
	string name; 
	int num_comments; 
	vector<Post*> posts; 
public:
	User(string inName); // constructor 
	~User(); // destructor 
	// class member functions
	Post& createPost(string content); 
	vector<Post*> getPosts();
	int getNumComments();
	string getName();
};

// Post class
class Post{
friend class Comment;
friend class User;
private:
	User* user;
	string content;
	vector<Comment*> comments;;
public:
	Post(User& inUser, string inContent); // constructor
	~Post(); // destructor
	// class member functions
	void addComment(string content, User* commenter);
	vector<Comment*> getComments();
	User& getUser();
	string getContent();
};

class Comment{
friend class Post;
friend class User;
private:
	User* user;
	string content;
	Post* post;
public:
	Comment(User* inUser, Post* inPost, string inContent); // constructor
	~Comment(); // destructor
	// class member functions
	User& getUser();
	string getContent();
	Post& getPost();
};

