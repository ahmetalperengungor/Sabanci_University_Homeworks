#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <vector> 
#include <pthread.h>

#include <sys/wait.h>






using namespace std;
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;


struct word_options{ 
    public:
    string bigger_redirection = ">";
    string smaller_redirection = "<";
    string background = "&";
    string option = "-";
    string wait_command = "wait"; // wait command is a special case, since it does not have any other arguments
};


void * thread_function_read(void * arg){ // function to be executed by threads, reads from the pipe and prints to the console 
    long int fd_read = (long int)(arg); //fd[0] in c, reading end of the pipe
    FILE * pipe_file;
    bool guard_lock = false;
    int counter = 0;
    while(guard_lock == false){ 
        pthread_mutex_lock(&global_lock);
        char buff[100]; 
        pipe_file = fdopen(fd_read, "r"); //fd[0]
        
        // before proceding, need to check whether the file exists or not
        if(pipe_file){ 
            // in addition, need to check whether the file is empty or not
            if((fgets(buff, sizeof(buff), pipe_file) != NULL)){ 
                printf("---------- %ld\n", pthread_self()); //print the thread ID to check if the threads are working properly
                printf("%s", buff); 
                fsync(STDOUT_FILENO); 
                while (fgets(buff, sizeof(buff), pipe_file) != NULL){ //keep reading until the file is empty (EOF reached)
                    printf("%s", buff);
                    fsync(STDOUT_FILENO);
                }
                printf("---------- %ld\n", pthread_self()); 
                fsync(STDOUT_FILENO);
                guard_lock = true;
                counter++;
            } 
            /*
            else{ // in case of edge cases like empty grep result, we need to set guard_lock to true to avoid infinite loop
                guard_lock = true;
            } // removed this part since it caused some errors and were contradicting the overall logic of the while loop
            */
        }
        pthread_mutex_unlock(&global_lock);
    }
    fclose(pipe_file); // close the pipe_file
    return NULL;
}

int main(int argc, const char * argv[]) {
    ifstream command_file;
    ofstream parse_file;
    command_file.open("commands.txt"); 
    parse_file.open("parse.txt"); 
    string line, word;
    vector<int> process_list; //in order to store ids of child processes
    vector<pthread_t> thread_list; //in order to store ids of threads
    
    while(getline(command_file, line)){ //loop through commands.txt
        istringstream ss(line); //to read the whole line
        ss >> word; //read the first word, which is the command itself 
        char option_redirect = '-';
        string fname, input_word, options_word;
        char * command[4] = {}; //command, input, option + null (for execvp)
        int idx = 0;
        command[idx] = strdup(word.c_str()); //first index is for the command tbe
        idx++;
        word_options new_word_options;
        
       
        
        // since the command is taken as the first word in input, the rest can be parsed with the following checks:
            //if the word starts with -, it is an option
            //if the word is > or < , it is a redirection
            // 

            
        while(ss >> word){
            if(word.find(new_word_options.option) == 0){ //if the word starts with -, it is an option
                options_word = word;
                command[idx] = strdup(word.c_str()); //adds it to the array
                idx++;
            }
            else if (word == new_word_options.bigger_redirection){ //if the word is >, it is a redirection
                option_redirect = '>'; 
                ss >> word;
                fname = word; 
            }
            else if (word == new_word_options.smaller_redirection){ //if the word is <, it is a redirection 
                option_redirect = '<';
                ss >> word;
                fname = word;
            }
            else if (word == new_word_options.background){ //if the word is &, it is a background, do nothing for now
                ;
            }
            else{ // if the word is not any of the above, it is an input
                input_word = word;
                command[idx] = strdup(word.c_str()); 
                idx++;
            }
        }

        // parsing is completed
        parse_file << "----------" << endl;
        parse_file << "Command: " << command[0] << endl;
        parse_file << "Inputs: " << input_word << endl;
        parse_file << "Options: " << options_word << endl;
        parse_file << "Redirection: " << option_redirect << endl;
        parse_file << "Background Job: " << (word == "&" ? "y": "n") << endl; // if the last word read is &, it is a background
        parse_file << "----------" << endl;
        parse_file.flush();
        
        bool checker = false;

        if (word == new_word_options.wait_command){ //in order to print everyting, wait for all the processes and threads to finish
            for (int i = 0; i < process_list.size(); i++){
                waitpid(process_list[i], NULL, NULL);
            }
            process_list.clear(); 
            for(int i=0; i <  thread_list.size(); i ++){
                pthread_join(thread_list[i], NULL);
            }
            thread_list.clear();
        }
        else{ //execute the command via fork and execvp
            if(option_redirect == '>'){ 
                int pid_redirect = fork();
        
                if (pid_redirect < 0){ // if fork fails
                    cout << "Fork failed"<< endl;
                    checker = true;
                }
                else if (pid_redirect == 0){ // if child process
                    int out = open(fname.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); //output file
                    dup2(out, STDOUT_FILENO); //stdout is now the file, like we did in pa1
                    close(out); 
                    execvp(command[0], command); 
                }
                else{ // parent process continues
                    if (word == "&"){ // if a background process, push it to keep track
                        process_list.push_back(pid_redirect); //push it to keep track
                    }
                    else{
                        waitpid(pid_redirect, NULL, NULL); //if not, wait before continuing to read more commands.
                    }
                }
            }
            else{ //in this case, output will be on the console
                int fd[2];
                pipe(fd);
                pthread_t thread_1;
                int pid_child = fork();
                if(pid_child < 0){ // if fork fails
                    cout << "Fork failed" << endl;
                    checker = true;
                }

                else if (pid_child == 0){ // if child process
                    if (option_redirect == '<'){
                        int read_only_file_value = open(fname.c_str(), O_RDONLY); //read only
                        dup2(read_only_file_value, STDIN_FILENO); //redirect the input to the file
                        close(read_only_file_value); 
                        
                    }
                    close(fd[0]); //reading end closed
                    dup2(fd[1], STDOUT_FILENO); //redirect the output to the pipe
                    close(fd[1]);
                    execvp(command[0], command); 
                }
                else{ //parent process continues
                    close(fd[1]); //close the write end
                    pthread_create(&thread_1, NULL, thread_function_read, (void*) fd[0]); // use our handcrafted function for threads, defined in the beginning of the code
                    if (word == "&"){ //background, push them to keep track
                        process_list.push_back(pid_child);
                        thread_list.push_back(thread_1); 
                    }
                    else{ //if not, wait for the child process to finish
                        waitpid(pid_child, NULL, NULL); 
                        pthread_join(thread_1, NULL); 

                    }
                }
            }
        }
    }

    //waiting, joining every thread to make sure everything is printed.
    for (int i=0; i < process_list.size(); i++){
        waitpid(process_list[i], NULL, NULL);
    }
    process_list.clear();
    for(int i=0; i <  thread_list.size(); i ++){
        pthread_join(thread_list[i], NULL);
    }
    parse_file.close(); 
    command_file.close();
    thread_list.clear();
    return 0;
}