#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "list.h"
#include "json.hpp"

using json = nlohmann::json;


void List::printMenu()
{
    std::string command;
    std::cout<<"==============\n";
    std::cout<<" Task Tracker\n";
    std::cout<<"==============\n\n\n\n";

    while (true)
    {
        std::cout<<"1 - Print the Task List.\n";
        std::cout<<"2 - Add a task to the List.\n";
        std::cout<<"3 - Delete a task from the List.\n";
        std::cout<<"4 - Mark the task.\n";

        std::cout<<"\nEnter a command:\n";
        std::getline(std::cin, command);

        if(command == "exit")
        {
            std::cout<<"Good bye;\n";
            std::exit(0);
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
    std::for_each(taskData.begin(),taskData.end(), [](std::string x){std::cout<<x<<"\n";});
}

void List::addList()
{
    //testing all the features of json.hpp
    std::string task;
    std::cin>>task;
}

void List::deleteList()
{

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