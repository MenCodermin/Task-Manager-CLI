#include <iostream>
#include <json.hpp>

class List
{
private:
    unsigned int id = 0;
    std::string directory = "E:/Projects/TaskManagerCLI/build";
    std::string fileName = directory + "/tasks.json";

public:
    List(){}
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

