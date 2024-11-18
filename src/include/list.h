#include <iostream>


class List
{
private:
    std::vector<std::string> taskData;
    bool taskDone,taskInProgress,taskNotDone;
    unsigned int id = 0;

public:
    List(){}
    ~List(){}
    void printMenu();
    void updateList();
    void addList();
    void deleteList();
    void printList();
    void listDoneTask();
    void listPendingTask();
};

