#include <iostream> 
#include <filesystem> //work with file and folders 
#include <fstream>  //helps to read from files or write to files  
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

    //add commands with linked lists 
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
    }; 

int main(){
    std::string command;
    StagingArea staging; //Creating the staging system we'll interact with.
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
    else{
        std::cout << "Command Unlown!. \n";
    }
 }     
    return 0;
}



 
