#include <iostream>
#include <string>
using namespace std;

struct Task {
    string subject;
    string type;
    int basePriority;
    int finalPriority;
    int daysLeft;
    Task* next;
};

Task* head = NULL;

// Priority based on type
int priorityFromType(string type) {
    if (type == "MiniProject" || type == "LabEval") return 1;  //highest priority 
    if (type == "TutSheet") return 2;
    return 3; // Assignment
}

void addTask(string sub, string type, int days) {
    Task* t = new Task;
    t->subject = sub;
    t->type = type;
    t->daysLeft = days;
    t->basePriority = priorityFromType(type);

    // Deadline override
    if (days == 1)          t->finalPriority = 1;
    else if (days == 2)     t->finalPriority = min(2, t->basePriority);
    else                    t->finalPriority = t->basePriority;

    t->next = NULL;

    if (head == NULL) {
        head = t;
        return;
    }

    Task* temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = t;
}

// Sort by final priority and remaining days
void sortTasks() {
    for (Task* i = head; i != NULL; i = i->next) {
        for (Task* j = i->next; j != NULL; j = j->next) {
            if ((i->finalPriority > j->finalPriority) ||
                (i->finalPriority == j->finalPriority && i->daysLeft > j->daysLeft)) {

                swap(i->subject, j->subject);
                swap(i->type, j->type);
                swap(i->basePriority, j->basePriority);
                swap(i->finalPriority, j->finalPriority);
                swap(i->daysLeft, j->daysLeft);
            }
        }
    }
}

bool tasksRemaining() {
    Task* t = head;
    while (t != NULL) {
        if (t->daysLeft > 0) return true;
        t = t->next;
    }
    return false;
}

void generateSchedule(string today) {
    string week[7] = 
        {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    // Map today → index
    int start = 0;
    for (int i = 0; i < 7; i++) 
        if (week[i] == today)
            start = i;

    cout << "\n-------------------------------------\n";
    cout << "         SMART STUDY SCHEDULE\n";
    cout << "-------------------------------------\n";

    int dayCount = 0;

    // Continue until tasks finish
    while (tasksRemaining()) {

        cout << "\n" << week[(start + dayCount) % 7] << ":\n";

        int assignedToday = 0;
        Task* t = head;

        // First pass → assign urgent tasks (daysLeft == 1)
        t = head;
        while (t != NULL) {
            if (t->daysLeft == 1) { 
                cout << "   - " << t->type << " for " << t->subject 
                     << " (Days Left: " << t->daysLeft << ")\n";
                t->daysLeft--;
                assignedToday++;
            }
            t = t->next;
        }

        // Second pass → assign other tasks ONLY up to 3 per day
        t = head;
        while (t != NULL && assignedToday < 3) {
            if (t->daysLeft > 1) {
                cout << "   - " << t->type << " for " << t->subject 
                     << " (Days Left: " << t->daysLeft << ")\n";
                t->daysLeft--;
                assignedToday++;
            }
            t = t->next;
        }

        dayCount++;
    }
}

int main() {
    int n;
    string today;

    cout << "Enter today's day (Monday/Tuesday/...): ";
    cin >> today;

    cout << "Enter number of tasks: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        string subject, type;
        int days;

        cout << "\nTask " << i + 1 << ":\n";

        cout << "Enter Subject Name: ";
        cin >> subject;

        cout << "Enter Task Type (Assignment/TutSheet/MiniProject/LabEval): ";
        cin >> type;

        cout << "How many days left for submission? ";
        cin >> days;

        addTask(subject, type, days);
    }

    sortTasks();
    generateSchedule(today);

    return 0;
}