#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "include/json.hpp"
#include "list.h"

using json = nlohmann::json;

void List::printMenu()
{
    std::cout<<"==============\n";
    std::cout<<"\tTask Tracker\n";
    std::cout<<"==============\n";
    std::cout<<"1 - Print the Task List.\n";
    std::cout<<"2 - Add a task to the List.\n";
    std::cout<<"3 - Delete a task from the List.\n";
    std::cout<<"4 - Mark the task.\n";
}

void List::printList()
{
    std::for_each(taskData.begin(),taskData.end(), [](std::string x){std::cout<<x<<"\n";});
}

void List::addList()
{
    std::string task;
    std::cin>>task;
}