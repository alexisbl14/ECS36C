#include "json.hpp" // including json.hpp to use json objects
#include <string> //since we will be storing commandline argument into a string
#include <iostream> //since we will be outputing to console
#include <fstream> //used to read from a file
#include "AVL.h"
 
using namespace std;
 
int main(int argc, char** argv) {
 
    if (argc != 2)
    {
        std::cerr << " ERROR: Incorrect Amount of Inputs " << endl;
        std::cerr << "USAGE: AVLCommands.cxx TestCase.json" << endl;
        return 0;
    }
    nlohmann::json jsonObject; //declaring a jsonobject
    string file_name = argv[1]; //storing command line arg to a string
    ifstream json_file(file_name); //creating an object of ifstream class and passing it the command line arg as its' argument
    if(!json_file.is_open()) //if the file doesn't open
    {
        cout << "The File Could Not Be Open" << endl; //print couldn't open file
    }
    json_file >> jsonObject; // get the contents of the file and put them in a json object
    nlohmann::json outObject; //creating another object that will be outputted to console.
    jsonObject.erase("metadata"); //erasing metadata preparing for iteration
    AVL AVLObject;
 
        for (auto itr = jsonObject.begin(); itr != jsonObject.end(); ++itr) //iterating over the keys/values of the object
        { //loop that will continous insert into our AVL Tree
            int key = itr.value()["key"];            
            AVLObject.Insert(key);
            
        }
    
 
    cout << AVLObject.JSON() << endl;
    return 0;// end of program 
}