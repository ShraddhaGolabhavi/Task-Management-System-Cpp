#include <iostream>
#include <string>

using namespace std;

#define MAX_TASKS 1
#define MAX_USERS 100
#define MAX_ASSIGNMENTS 100
#define MAX_DEADLINES 100
#define MAX_NOTIFICATIONS 100

class Task {
protected:
    int taskId;
    string title;
    string description;

public:
    Task() {}
    Task(int taskId, const string& taskTitle, const string& taskDescription)
        : taskId(taskId), title(taskTitle), description(taskDescription) {}

    void printTaskDetails() const {
        cout << "Task ID: " << taskId << "\n";
        cout << "Title: " << title << "\n";
        cout << "Description: " << description << "\n";
    }

    int getId() const {
        return taskId;
    }
};

class User {
protected:
    int userId;
    string name;
    string email;
    Task* assignedTasks[MAX_TASKS];
    int numTasksAssigned;

    friend class TaskManager;

    void assignTaskToUser(Task* task) {
        // Check if the task is already assigned to this user
        for (int i = 0; i < numTasksAssigned; ++i) {
            if (assignedTasks[i]->getId() == task->getId()) {
                throw "Task is already assigned to this user.";
            }
        }

        if (numTasksAssigned >= MAX_TASKS) {
            throw "Maximum number of tasks assigned";
        }
        assignedTasks[numTasksAssigned++] = task;
    }

public:
    User() : numTasksAssigned(0) {}
    User(int id, const string& userName, const string& userEmail)
        : userId(id), name(userName), email(userEmail), numTasksAssigned(0) {}

    void printUserDetails() const {
        cout << "User ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Tasks Assigned: " << numTasksAssigned << endl;
        cout << "Assigned Tasks: ";
        for (int i = 0; i < numTasksAssigned; ++i) {
            cout << assignedTasks[i]->getId() << " ";
        }
        cout << endl;
    }
};


class Assignment {
private:
    int taskId;
    int userId;
    string status;
    string assignmentTime;

public:
    Assignment(int taskId, int userId, const string& status, const string& time)
        : taskId(taskId), userId(userId), status(status), assignmentTime(time) {}

    void printAssignmentDetails() const {
        cout << "Task ID: " << taskId << "\n";
        cout << "User ID: " << userId << "\n";
        cout << "Status: " << status << "\n";
        cout << "Assignment Time: " << assignmentTime << "\n";
    }

    int getTaskId() const {
        return taskId;
    }

    void setStatus(const string& newStatus) {
        status = newStatus;
    }

    string getStatus() const {
        return status;
    }
};

class DeadlineAndStatus {
private:
    int taskId;
    string dueDate;
    string status;

public:
    DeadlineAndStatus(int taskId, const string& deadline, const string& initialStatus)
        : taskId(taskId), dueDate(deadline), status(initialStatus) {}

       int getTaskId()
       {
           return taskId;
       }

    bool isDeadlineCrossed(const string& currentDate) const {
        return currentDate > dueDate && status != "Completed";
    }

    void setStatus(const string& newStatus) {
        status = newStatus;
    }

    void printDeadlineAndStatusDetails() const {
        cout << "Task ID: " << taskId << "\n";
        cout << "Deadline: " << dueDate << "\n";
        cout << "Status: " << status << "\n";
    }
};

class Notification {
private:
    int taskId;
    string message;

public:
    Notification(int taskId, const string& msg)
        : taskId(taskId), message(msg) {}

    int getTaskId() const {
        return taskId;
    }

    string getMessage() const {
        return message;
    }

    void setMessage(const string& msg) {
        message = msg;
    }
};

class TaskManager {
private:
    Task* tasks[MAX_TASKS];
    User* users[MAX_USERS];
    Assignment* assignments[MAX_ASSIGNMENTS];
    DeadlineAndStatus* deadlines[MAX_DEADLINES];
    Notification* notifications[MAX_NOTIFICATIONS];
    int taskCount;
    int userCount;
    int assignmentCount;
    int deadlineCount;
    int notificationCount;
    string currentDate;

public:
    TaskManager() : taskCount(0), userCount(0), assignmentCount(0), deadlineCount(0), notificationCount(0) {}

    void setCurrentDate(const string& date) {
        currentDate = date;
    }

    void createTask() {
    string title, description;
    cout << "Enter the title of the task: ";
    cin >> title;
    cout << "Enter the description of the task: ";
    cin >> description;

    tasks[taskCount] = new Task(taskCount + 1, title, description);
    taskCount++;
}

void addUser() {
    string name, email;
    cout << "Enter the name of the user: ";
    cin >> name;
    cout << "Enter the email of the user: ";
    cin >> email;

    users[userCount] = new User(userCount + 1, name, email);
    userCount++;
}

    void assignTask() {
        int taskId, userId;
        string assignedTime, deadline;
        cout << "Enter the task ID to be assigned: ";
        cin >> taskId;
        cout << "Enter the user ID to whom the task has to be assigned: ";
        cin >> userId;
        cout << "Enter the assignment time: ";
        cin >> assignedTime;
        cout << "Enter the deadline for the task: ";
        cin >> deadline;


        if (taskId > 0 && taskId <= taskCount && userId > 0 && userId <= userCount) {
            users[userId - 1]->assignTaskToUser(tasks[taskId - 1]);
            assignments[assignmentCount++] = new Assignment(taskId, userId, "Assigned", assignedTime);
            deadlines[deadlineCount++] = new DeadlineAndStatus(taskId, deadline, "Assigned");
            cout << "Task assigned successfully.\n";
        } else {
            cout << "Invalid task ID or user ID.\n";
        }
    }

    void checkDeadline() {
        if(assignmentCount!=0){
        for (int i = 0; i < deadlineCount; ++i) {
            if (deadlines[i]->isDeadlineCrossed(currentDate)) {
                for (int j = 0; j < assignmentCount; ++j) {
                    if (assignments[j]->getTaskId() == deadlines[i]->getTaskId()) {
                        string message = "Deadline crossed for Task ID: " + to_string(deadlines[i]->getTaskId());
                        notifications[notificationCount++] = new Notification(deadlines[i]->getTaskId(), message);
                    }
                }
            }
    }
    cout << "Deadline checked and notifications sent.\n";
    }
    else if(assignmentCount==0)
            {
                cout<<"No tasks assigned";
            }
            else{cout<<" the deadline is not crossed,please check the assignment list";}
        }


     void markTaskAsCompleted(int taskId) {
         if(assignmentCount!=0){

        for (int i = 0; i < deadlineCount; ++i) {
            if (deadlines[i]->getTaskId() == taskId) {
                deadlines[i]->setStatus("Completed");
                assignments[i]->setStatus("Completed");
                cout << "Task ID " << taskId << " marked as completed.\n";
                break; // Exit the loop once the task is found
            }




        }

    }
    else{cout<<"No tasks assigned";}
     }

    void printNotifications() const {
        if(notificationCount==0){cout<<"No assignments";}
        else{
        cout << "Notifications:\n";
        for (int i = 0; i < notificationCount; ++i) {
            cout << notifications[i]->getMessage() << endl;
        }
    }
    }

    void printAllTasks() const {
        cout << "All Tasks:\n";
        for (int i = 0; i < taskCount; ++i) {
            tasks[i]->printTaskDetails();
            cout << "\n";
        }
    }

    void printAllUsers() const {
        if(userCount==0){cout<<"No user assigned any task";}
        else{
        cout << "All Users:\n";
        for (int i = 0; i < userCount; ++i) {
            users[i]->printUserDetails();
            cout << "\n";
        }
        }
    }

    void printAllAssignments() const {
        if(assignmentCount==0){cout<<"No assignments";}
        else{
        cout << "All Assignments:\n";
        for (int i = 0; i < assignmentCount; ++i) {
            assignments[i]->printAssignmentDetails();
            cout << "\n";
        }
        }
    }

    void printAllDeadlines() const {
        cout << "All Deadlines:\n";
        for (int i = 0; i < deadlineCount; ++i) {
            deadlines[i]->printDeadlineAndStatusDetails();
            cout << "\n";
        }
    }
};

int main() {
    int password;
    cout << "Enter the password: ";
    cin >> password;

    if (password == 123) {
        TaskManager taskManager;

        int choice;
        string currentDate; // Declare currentDate outside the switch statement
        int taskId;
        while (true) {
            cout << "1. Create a user\n";
            cout << "2. Create a task\n";
            cout << "3. Assign a task to a user\n";
            cout << "4. Display task details\n";
            cout << "5. Display user details\n";
            cout << "6. Display assignment details\n";
            cout << "7. Display deadline details\n";
            cout << "8. Check deadline and send notifications\n";
            cout << "9. Display notifications\n";
            cout << "10. Mark task as completed\n"; // New option
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    taskManager.addUser();
                    break;
                case 2:
                    taskManager.createTask();
                    break;
                case 3:
                    try {
                          taskManager.assignTask();
                        }
                    catch (const char* msg) {
                          cout << "Exception: " << msg << endl;
                       }

                    break;
                case 4:
                    taskManager.printAllTasks();
                    break;
                case 5:
                    taskManager.printAllUsers();
                    break;
                case 6:
                    taskManager.printAllAssignments();
                    break;
                case 7:
                    taskManager.printAllDeadlines();
                    break;
                case 8:
                    cout << "Enter today's date (YYYY-MM-DD): ";
                    cin >> currentDate; // Initialize currentDate here
                    taskManager.setCurrentDate(currentDate);
                    taskManager.checkDeadline();

                    break;
                case 9:
                    taskManager.printNotifications();
                    break;
                case 10:
                    cout << "Enter the task ID to mark as completed: ";
                    cin >> taskId;
                    taskManager.markTaskAsCompleted(taskId);
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    } else {
        cout << "Incorrect password.\n";
    }

    return 0;
}
