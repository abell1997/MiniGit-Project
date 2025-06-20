#include <iostream> 
#include <filesystem> //work with file and folders 
#include <fstream>  //helps to read from files or write to files
#include <ctime>
#include <sstream> //for reading full file content into  a buffer  
namespace fl = std::filesystem; 

void init(){ 
    fl::path mingit_repo = ".mingit"; // function that handles the init command 

    // We check if .mingit already exists 
    if(fl::exists(mingit_repo)){ 
       std::cout << "MinGit repository is already initialized. \n"; 
        return; 
  } 

    // if it doesn't exist we create this 
     fl::create_directory(mingit_repo); 
     fl::create_directory(mingit_repo/ "objects"); 
     fl::create_directory(mingit_repo/ "refs"); 

     std::ofstream head_file(mingit_repo/ "HEAD"); // creatng a head file that stores the current branch. 

    if(head_file){ 
        head_file << "ref: refs/main \n"; 
        head_file.close(); 
    } 

    std::cout << "Initialized empty MinGit reposory in .mingit/  \n"; 
    } 

    // add commands with linked lists 
    // It stores: 
    //  the name of file that's staged and a pointer to the next file in the list 
    struct FileNode{ 
    std::string filename; 
    FileNode* next; 

     FileNode(const std::string& fname){ 
        filename = fname; 
        next = nullptr; 
     } 
}; 
 
 

    // It manages which files are staged and avoid duplicates  
    // and also shows the staged files. 
   class StagingArea{ 
    private:  
        FileNode* head; 
    public:  
        StagingArea(){ 
        head = nullptr; 
     } 
     bool isStaged(const std::string& fname){ //Checking if the file is already in the staging list 
         FileNode* temp = head; 
         while(temp != nullptr){ 
            if(temp -> filename == fname){ 
                 return true; 
             } 
             temp = temp -> next; 
         } 
         return false; 
     } 

        void  addFile(const std::string& fname){  // Adds new file to the begining of linked list 
         if(isStaged(fname)){ 
         std::cout << "File" << fname << "' is already staged. \n"; 
             return; 
         } 
         if(!fl::exists(fname)){ 
            std::cout << "Error!: File '" << fname << "' doesn't exist.\n"; 
                return; 
        } 

        // Otherwise we create a new node and link it in. 
        FileNode* newNode = new FileNode(fname); 
         newNode -> next = head; 
         head = newNode; 

         std::cout << "Added '" << fname << "' to staging area. \n"; 
     } 

    void printStagedFiles(){  // Loops through the list and prints each staged file 
        std::cout << "Currently staged files: \n"; 
        FileNode* temp = head; 

        while(temp != nullptr){ 
            std::cout << "- " << temp -> filename << "\n"; 
                temp = temp->next; 
         } 
     } 
     FileNode* getHead() {
      return head;
     }
  }; 

    // Represents one commit(snapshot of our project)
    struct CommitNode{
    int commitID;
    std::string message;
    std::string timestamp;
    FileNode* files;
    CommitNode* prev;  //pointer to the previous commit(linked lst)

    CommitNode(int id, const std::string& msg, FileNode* stagedFiles, CommitNode* parent) : 
        commitID(id), message(msg), prev(parent), files(stagedFiles){
        time_t now = time(0);
        timestamp = std::ctime(&now);
        timestamp.pop_back();
    }
};

int main(){ 
   std::string command;
    StagingArea staging; //Creating the staging system we'll interact with.
    CommitNode* headCommit = nullptr;  //Points to the latest commit
    int commitCounter = 0;      //Gives each commit a unique ID
    std::cout << "Welcome to MinGit! \n";

    while(true){
        std::cout << "\n>";
        std::cin >> command;

        if(command == "init"){
           fl::path mingit_repo = ".mingit";

           if(fl::exists(mingit_repo)){
            std::cout <<  "MinGit repository is already initialized. \n";
            continue;
           }
            fl::create_directory(mingit_repo);
            fl::create_directory(mingit_repo/ "objects");
            fl::create_directory(mingit_repo/ "refs");

        std::ofstream head_file(mingit_repo/ "HEAD"); 
        if(head_file){
        head_file << "ref: refs/main \n";
        head_file.close();
    }
     std::cout << "Initialized empty MiniGit repository in .minigit/\n";

        }
        // ADD COMMAND
        else if(command == "add"){
        std::string filename;
        std::cin >> filename;
        staging.addFile(filename);
    }
    else if(command == "status"){
        staging.printStagedFiles(); // prints the list of current staged files
    }
    else if(command == "exit"){
        break;
    }
    //COMMIT COMMAND
    else if(command == "commit"){
        std::string dash, message;
        std::cin >> dash;
        std::getline(std::cin, message); //captures the entire message even with sapces

        if(dash != "-m" || message.empty()){ //Checking error
            std::cout << "Usage: commit -m \"your message\" \n";
            continue;
        }
        if(staging.getHead() == nullptr){  //Checks if anything is staged and prevents commit with no files 
            std::cout << "There are no files staged. Use 'add' before commiting. \n";
            continue;
        }
        //Reads the entire content of the file and stores it as a string
        FileNode* current = staging.getHead();
        while(current != nullptr){
            std::ifstream src(current -> filename, std::ios::binary);
            std::ostringstream buffer;
            buffer << src.rdbuf();
            std::string filecontent = buffer.str();

            //We save each file as a versioned copy using filename + commit ID
            std::string newName = current -> filename + "_" + std::to_string(commitCounter);
            std::ofstream dest(".mingit/objects/" + newName, std::ios::binary);
            dest << filecontent;

            current = current->next;
          }
          //Creates new commit node
        CommitNode* newCommit =  new CommitNode(commitCounter++, message, staging.getHead(), headCommit);
        headCommit = newCommit;

        std::cout << "Commited successfully: " << newCommit -> message << "\n";

        std::ifstream headFile(".mingit/HEAD");
        std::string headLine;
        std::getline(headFile, headLine);
        headFile.close();

        std::string branchRefPath = headLine.substr(5);
        std::ofstream branchOut(branchRefPath);         
        if (branchOut) {
        branchOut << headCommit->commitID << "\n";
        branchOut.close();
     }
    
        }
        //LOG COMMAND -> shows the history of all commands
        else if(command == "log"){
        if(headCommit == nullptr){
            std::cout << "No commits yet. \n";
            continue;
        }

        CommitNode* current = headCommit; //Sets up a temporary pointer(current)
        while(current != nullptr){ //loop through each commit in reverse(newest to oldest) using linked structure
            std::cout << "Comit #" << current -> commitID <<"\n";
            std::cout << "Message: " << current -> message <<"\n";
            std::cout << "Date: " << current -> timestamp <<"\n";
            std::cout << "-------------------------- \n";
            current = current->prev;
        }
    }
    //BRANCHING
    else if(command == "branch"){
        std::string branchName;
        std::cin >> branchName;

        if(branchName.empty()){  
            std::cout << "Please write a branch name. \n";
            continue;
        }
        if(headCommit == nullptr){  //This ensures there is a commit to branch from
            std::cout << "At least you must make one commit before creating branches. \n";
            continue;
        }
        std::string branchPath = ".mingit/refs/" + branchName; //This builds the branch to the new branch file

        if(fl::exists(branchPath)){  //This prevents overwriting an existing branch with same name
            std::cout << "Branch '" << branchName << "' already exists. \n";
            continue;
        }
        std::ofstream branchFile(branchPath);  // Creates .mingit/refs/<branchName> and writes the current commit ID into it
        if(branchFile){
            branchFile << headCommit -> commitID << "\n";
            branchFile.close();
            std::cout << "Branch '" << branchName << "' created pointing to commit #" 
            << headCommit -> commitID << "\n";

        }
        else{
            std::cout << "Failed to create branch file. \n";
        }
        
    }
    else{ 
    std::cout << "Command Unkown!. \n"; 
     } 
  }  
    return 0; 

} 