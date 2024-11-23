#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "include/list.h"
#include "include/json.hpp"

using json = nlohmann::json;


void List::printMenu()
{
    std::string command;
    std::cout<<"==============\n";
    std::cout<<" Task Tracker\n";
    std::cout<<"==============\n";

    while (true)
    {

        std::cout<<"\n\n\nPlease use words as a command.\n\n\n"; 
        std::cout<<"1 - Print the Task List. (list)\n";
        std::cout<<"2 - Add a task to the List. (add)\n";
        std::cout<<"3 - Delete a task from the List. (delete)\n";
        std::cout<<"4 - Mark the task. (update)\n";
        std::cout<<"5 - Exit the programm. (exit)\n";
     
        std::cout<<"\nEnter a command: \n";
        std::getline(std::cin, command);

        if(command == "exit")
        {
            std::cout<<"Good bye;\n";
            std::exit(1);
        }
        else if (command.find("add") == 0)
        {
            addList();
        }
        else if (command.find("list") == 0)
        {
            printList();
        }
        else if(command.find("delete") == 0)
        {
            deleteList();
        }
        else if(command.find("update") == 0)
        {
            updateList();
        }
        else if(command == "list-done")
        {
            listDoneTask();
        }
        else if(command == "list-pending")
        {
            listPendingTask();
        }
        else
            std::cout<<"\nInvalid command. Try again\n\n";
    }

}

void List::printList()
{
    json task;
    ensureFileExists();

    std::ifstream inFile(fileName);
    if(inFile.is_open())
    {
        inFile >> task;
        inFile.close();
    }
    else
    {
        std::cout<<"Error opening file for reading: " << fileName << std::endl;
        return;
    }

    if(task.empty())
    {
        std::cout<<"No tasks available."<<std::endl;
        return;
    }

    std::cout << "==========================\n";
    std::cout << "       Task List\n";
    std::cout << "==========================\n";
    for(const auto& tasks: task)
    {
        std::cout << "ID:" << tasks["id"] << "\n";
        std::cout << "Description: " << tasks["description"] << "\n";
        std::cout << "Status " << tasks["status"] << "\n";
        std::cout << "--------------------------\n";
    }

    
}

void List::addList()
{
    json tasks,task;

    std::string descirption;
    unsigned int id;
    
    if (std::filesystem::exists(fileName)) 
    { //Checking if file exists or not, if not, then create the one
        std::ifstream inFile(fileName);
        if (inFile.is_open()) 
        {
            inFile >> tasks; // Parse JSON from file
            inFile.close();
        } 
        else 
       {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
            return;
        }
    } 
    else {tasks = json::array();} 
    
    id = tasks.size() + 1;

    std::cout<<"Enter the task description: ";
    std::getline(std::cin, descirption);

    task = {
        {"id", id},
        {"description", descirption},
        {"status", "in progress"}
    };

    tasks.push_back(task);


    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << tasks.dump(4); 
        outFile.close();
        std::cout << "Task added successfully!" << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
}

void List::saveToFile(const json& tasks)
{
    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << tasks.dump(4); // Pretty print with 4 spaces
        outFile.close();
        std::cout << "Tasks updated successfully in: " << fileName << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
}

void List::deleteList()
{
    json tasks;
    unsigned int idToDelete;
    bool taskFound = false;
    char confirm;

    if (std::filesystem::exists(fileName)) //Checking if file exists or not, if not, then create the one
    {
        std::ifstream inFile(fileName);
        if (inFile.is_open()) 
        {
            inFile >> tasks; // Parse JSON from file
            inFile.close();
        } 
        else
        {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
            return;
        }
    }
    else
    {
        std::cout<<"File does not exist:"<< fileName<< std::endl;
        return;
    }

    std::cout<<"Enter the id of the task to delete: ";
    std::cin>> idToDelete;
    for(auto it = tasks.begin(); it != tasks.end(); it++)
    {
        if((*it)["id"] == idToDelete)
        {
            std::cout<<"Are you sure you want to delete the task? (Y/N):"<<std::endl;
            std::cin>> confirm;
            if(confirm == 'y' || confirm == 'Y')
            {
                tasks.erase(it);
                taskFound = true;
                break;
            }
            else if( confirm == 'n' || confirm == 'N')
            {
                std::cout<<"Task removal canceled."<<std::endl;
                taskFound = false;
                return;
            }
            else
            {
                std::cout<<"Wrong command. Returning to the Main Menu."<<std::endl;
                return;
            }
            
            
        }
    }

    if( taskFound)
    {
        std::cout<<" Task with ID: "<< idToDelete << " removed successfully."<< std::endl;
    }
    else
    {
        std::cout<<" Task with ID: "<< idToDelete << " not found."<< std::endl;
    }

    saveToFile(tasks);
}

void List::updateList()
{

}

void List::listDoneTask()
{

}
void List::listPendingTask()
{

}

void List::ensureFileExists() {
    if (!std::filesystem::exists(fileName)) {
        std::ofstream outFile(fileName);
        if (outFile.is_open()) {
            outFile << json::array().dump(4); // Create an empty JSON array if file doesn't exist
            outFile.close();
            std::cout << "Created an empty task file: " << fileName << std::endl;
        } else {
            std::cerr << "Error creating file: " << fileName << std::endl;
            std::exit(1);
        }
    }
}