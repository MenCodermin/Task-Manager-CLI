#include "include/list.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include "include/json.hpp"

using json = nlohmann::json;

void List::printMenu()
{
    int command;
    std::cout << "==============\n";
    std::cout << " Task Tracker\n";
    std::cout << "==============\n";

    while (true)
    {
        std::cout << "\n\n\nPlease use words as a command.\n\n\n";
        std::cout << "1 - Print the Task List.\n";
        std::cout << "2 - Add a task to the List.\n";
        std::cout << "3 - Delete a task from the List.\n";
        std::cout << "4 - Update the task description.\n";
        std::cout << "5 - Update task status.\n";
        std::cout << "6 - Print list by status.\n";
        std::cout << "7 - Exit the programm.\n";

        std::cout << "\nEnter a command: \n";
        std::cin >> command;
        std::cin.ignore();

        switch (command)
        {
            case 1:
                printList();
                break;
            case 2:
                addList();
                break;
            case 3:
                deleteList();
                break;
            case 4:
                updateList();
                break;
            case 5:
                updateTaskStatus();
                break;
            case 6:
                printTasksByStatus();
                break;
            case 7:
                std::cout << "Exiting programm. Bye-bye!\n";
                exit(1);
                break;
            default:
                std::cout << "Wrong command entered. Try again.\n";
                break;
        }
    }
}

void List::printList()
{
    json task;
    ensureFileExists();

    std::ifstream inFile(fileName);
    if (inFile.is_open())
    {
        inFile >> task;
        inFile.close();
    }
    else
    {
        std::cout << "Error opening file for reading: " << fileName << std::endl;
        return;
    }

    if (task.empty())
    {
        std::cout << "No tasks available." << std::endl;
        return;
    }

    std::cout << "==========================\n";
    std::cout << "       Task List\n";
    std::cout << "==========================\n";
    for (const auto &tasks : task)
    {
        std::cout << "ID:" << tasks["id"] << "\n";
        std::cout << "Description: " << tasks["description"] << "\n";
        std::cout << "Status " << tasks["status"] << "\n";
        std::cout << "--------------------------\n";
    }

    waitForUser();
}

void List::addList()
{
    json tasks, task;

    std::string description;

    if (std::filesystem::exists(fileName))
    {  // Checking if file exists or not, if
        // not, then create the one
        std::ifstream inFile(fileName);
        if (inFile.is_open())
        {
            inFile >> tasks;  // Parse JSON from file
            inFile.close();

            for (const auto &task : tasks)
            {
                if (task.contains("id") && task["id"].is_number())
                {
                    id = std::max(id, static_cast<int>(task["id"]));
                }
            }
        }
        else
        {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
            return;
        }
    }
    else
        tasks = json::array();

    ++id;

    std::cout << "Enter the task description: ";
    std::getline(std::cin, description);

    std::string timestamp = getCurrentTimestamp();

    task = {{"id", id},
            {"description", description},
            {"status", "to-do"},
            {"created_at", timestamp},
            {"updated_at", timestamp}};

    tasks.push_back(task);

    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << tasks.dump(4);
        outFile.close();
        std::cout << "Task added successfully!" << std::endl;
    }
    else
    {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
    waitForUser();
}

void List::saveToFile(const json &tasks)
{
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << tasks.dump(4);  // Pretty print with 4 spaces
        outFile.close();
        std::cout << "Tasks updated successfully in: " << fileName << std::endl;
    }
    else
    {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
}

void List::deleteList()
{
    json tasks;
    unsigned int idToDelete;
    bool taskFound = false;
    char confirm;

    if (std::filesystem::exists(fileName))  // Checking if file exists or not,
                                            // if not, then create the one
    {
        std::ifstream inFile(fileName);
        if (inFile.is_open())
        {
            inFile >> tasks;  // Parse JSON from file
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
        std::cout << "File does not exist:" << fileName << std::endl;
        return;
    }

    std::cout << "Enter the id of the task to delete: ";
    std::cin >> idToDelete;
    std::cin.ignore();
    for (auto it = tasks.begin(); it != tasks.end(); it++)
    {
        if ((*it)["id"] == idToDelete)
        {
            std::cout << "Are you sure you want to delete the task? (Y/N):" << std::endl;
            std::cin >> confirm;
            std::cin.ignore();
            if (confirm == 'y' || confirm == 'Y')
            {
                tasks.erase(it);
                taskFound = true;
                break;
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                std::cout << "Task removal canceled." << std::endl;
                taskFound = false;
                return;
            }
            else
            {
                std::cout << "Wrong command. Returning to the Main Menu." << std::endl;
                return;
            }
        }
    }

    if (taskFound)
    {
        std::cout << " Task with ID: " << idToDelete << " removed successfully." << std::endl;
    }
    else
    {
        std::cout << " Task with ID: " << idToDelete << " not found." << std::endl;
    }

    saveToFile(tasks);
    waitForUser();
}

void List::updateList()
{
    json task;
    char choice;
    int idToUpdate;
    std::string newDescription;
    bool taskFound;

    ensureFileExists();

    std::ifstream inFile(fileName);
    if (inFile.is_open())
    {
        inFile >> task;
        inFile.close();
    }
    else
    {
        std::cout << "Error reading file: " << fileName << std::endl;
        return;
    }

    if (task.empty())
    {
        std::cout << "No task available." << std::endl;
        return;
    }

    for (const auto &tasks : task)
    {
        std::cout << "ID:" << tasks["id"] << "\n";
        std::cout << "Description: " << tasks["description"] << "\n";
        std::cout << "Status " << tasks["status"] << "\n";
        std::cout << "--------------------------\n";
    }
    std::cout << "Enter id of the task for update: " << std::endl;
    std::cin >> idToUpdate;
    std::cin.ignore();
    for (auto &tasks : task)
    {
        if (tasks["id"] == idToUpdate)
        {
            std::cout << "Enter the new description : ";
            std::getline(std::cin, newDescription);
            if (newDescription.empty())
            {
                std::cerr << "Error: Description cannot be empty.\n";
                return;
            }
            tasks["description"] = newDescription;
            tasks["updated_at"] = getCurrentTimestamp();
            taskFound = true;
            break;
        }
    }

    if (taskFound)
    {
        std::cout << "Task was successfully updated." << std::endl;
    }
    else
    {
        std::cout << "Task with ID:" << idToUpdate << "not found." << std::endl;
        return;
    }

    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << task.dump(4);
        outFile.close();
    }
    else
    {
        std::cout << "Error opening file for writing: " << fileName << std::endl;
    }

    waitForUser();
}

void List::ensureFileExists()
{
    if (!std::filesystem::exists(fileName))
    {
        std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            outFile << json::array().dump(4);  // Create an empty JSON array if file doesn't exist
            outFile.close();
            std::cout << "Created an empty task file: " << fileName << std::endl;
        }
        else
        {
            std::cerr << "Error creating file: " << fileName << std::endl;
            std::exit(1);
        }
    }
}

void List::updateTaskStatus()
{
    json task;
    int idToUpdate, newStatus;
    bool taskFound = false;
    std::map<int, std::string> statusList = {{1, "to-do"}, {2, "in progress"}, {3, "done"}};

    ensureFileExists();

    std::ifstream inFile(fileName);
    if (inFile.is_open())
    {
        inFile >> task;
        inFile.close();
    }

    else
    {
        std::cout << "Error reading file: " << fileName << std::endl;
        return;
    }

    if (task.empty())
    {
        std::cout << "No task available." << std::endl;
        return;
    }

    for (const auto &tasks : task)
    {
        std::cout << "ID:" << tasks["id"] << "\n";
        std::cout << "Description: " << tasks["description"] << "\n";
        std::cout << "Status " << tasks["status"] << "\n";
        std::cout << "--------------------------\n";
    }

    std::cout << "Enter id of the task for update: " << std::endl;
    std::cin >> idToUpdate;
    std::cin.ignore();

    for (auto &tasks : task)
    {
        if (tasks["id"] == idToUpdate)
        {
            taskFound = true;

            std::cout << "Choose the new status:\n";
            for (const auto &[key, value] : statusList)
            {
                std::cout << key << ". " << value << "\n";
            }

            std::cout << "Enter your choice: ";
            std::cin >> newStatus;
            std::cin.ignore();
            if (statusList.find(newStatus) != statusList.end())
            {
                tasks["status"] = statusList[newStatus];
                tasks["updated_at"] = getCurrentTimestamp();
                std::cout << "Task was updated successfully\n";
            }
            else
            {
                std::cout << "Invalid status. Update aborted.\n";
                return;
            }

            break;
        }
    }

    if (!taskFound)
    {
        std::cout << "Task with ID:" << idToUpdate << "not found." << std::endl;
        return;
    }

    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << task.dump(4);
        outFile.close();
    }
    else
    {
        std::cout << "Error opening file for writing: " << fileName << std::endl;
    }
    waitForUser();
}

void List::printTasksByStatus()
{
    json task;
    std::string statusFilter;

    ensureFileExists();

    std::ifstream inFile(fileName);
    if (inFile.is_open())
    {
        inFile >> task;
        inFile.close();
    }
    else
    {
        std::cout << "Error reading file: " << fileName << std::endl;
        return;
    }

    if (task.empty())
    {
        std::cout << "No tasks available." << std::endl;
        return;
    }

    std::cout << "Enter the status to filter the list:\n"
              << "(to-do) (in progress) (done)\n";
    std::getline(std::cin, statusFilter);

    std::cout << "\n==========================\n";
    std::cout << "Tasks with status: " << statusFilter << "\n";
    std::cout << "==========================\n";

    bool tasksFound = false;

    for (const auto &tasks : task)
    {
        if (tasks["status"] == statusFilter)
        {
            tasksFound = true;
            std::cout << "ID: " << tasks["id"] << "\n";
            std::cout << "Description: " << tasks["description"] << "\n";
            std::cout << "Status: " << tasks["status"] << "\n";
            std::cout << "--------------------------\n";
        }
    }

    if (!tasksFound)
    {
        std::cout << "No tasks found with status: " << statusFilter << "\n";
    }
    waitForUser();
}

int List::getValidStatus()
{
    int status = 0;

    while (true)
    {
        std::cout << "Enter the new status: \n"
                  << "1. To-do\n"
                  << "2. In progress\n"
                  << "3. Done.\n";
        std::cout << "Enter your choice (1, 2, or 3): ";
        std::cin >> status;
        std::cin.ignore();

        if (std::cin.fail() || status < 1 || status > 2)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between 1 and 3.\n";
        }
        else
            break;
    }
    return status;
}

void List::waitForUser()
{
    std::cout << "\nPress any key to return to the menu...";
    std::cin.get();
}

std::string List::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
