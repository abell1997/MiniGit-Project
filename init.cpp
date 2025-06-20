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

      std::ofstream head_file(mingit_repo/ "HEAD");  

    // creatng a head file that stores the current branch. 
    if(head_file){ 
        head_file << "ref: refs/main \n"; 
         head_file.close(); 
     } 
    
     std::cout << "Initialized empty MinGit reposory in .mingit/  \n"; 
} 

int main(){ 
    std::string command; 
    std::cout << "Welcome to MinGit! \n"; 

 while(true){ 
        std::cout << "\n>"; 
        std::cin >> command; 
        
        if(command == "init"){ 
        init(); 
     } 
         else if(command == "exit"){ 
         break; 
    } 
     else{ 
        std::cout << "Command Unkown!. \n"; 
     } 
 }  

return 0; 

} 
