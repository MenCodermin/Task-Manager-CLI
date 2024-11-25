#include <iostream>
#include <json.hpp>
#include <set>

class List
{
private:
    int id;
    std::set<int> deletedIds;
    std::string directory = "E:/Projects/TaskManagerCLI/build";
    std::string fileName = directory + "/tasks.json";
    nlohmann::json task;
    std::string getCurrentTimestamp();

public:
    List() : id(0){}
    ~List(){}
    void printMenu();
    void updateList();
    void addList();
    void deleteList();
    void printList();
    void ensureFileExists();
    void updateTaskStatus();
    void printTasksByStatus();
    void saveToFile(const nlohmann::json& tasks);
    void waitForUser();
    int getValidStatus();
};

