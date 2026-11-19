
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Student {
    int id;
    char name[50];
    float marks;
};

struct Student students[MAX];
int count = 0;

// Check whether student ID already exists
int idExists(int id) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id)
            return 1;
    }

    return 0;
}

// Load records from file
void loadData() {
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL)
        return;

    while (count < MAX &&
           fscanf(fp, "%d %49s %f",
                  &students[count].id,
                  students[count].name,
                  &students[count].marks) == 3) {
        count++;
    }

    fclose(fp);
}

// Save all records
void saveData() {
    FILE *fp = fopen("students.txt", "w");

    if (fp == NULL) {
        printf("Unable to save file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n",
                students[i].id,
                students[i].name,
                students[i].marks);
    }

    fclose(fp);
}

// Compare marks for descending sorting
int compareMarks(const void *a, const void *b) {
    const struct Student *s1 = a;
    const struct Student *s2 = b;

    if (s1->marks < s2->marks)
        return 1;

    if (s1->marks > s2->marks)
        return -1;

    return 0;
}

// Add a new student
void addStudent() {
    if (count >= MAX) {
        printf("Student limit reached!\n");
        return;
    }

    struct Student s;

    printf("Enter student ID: ");
    if (scanf("%d", &s.id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    if (idExists(s.id)) {
        printf("Student ID already exists!\n");
        return;
    }

    printf("Enter student name (one word): ");
    scanf("%49s", s.name);

    printf("Enter marks (0-100): ");
    if (scanf("%f", &s.marks) != 1 ||
        s.marks < 0 || s.marks > 100) {
        printf("Invalid marks!\n");
        return;
    }

    students[count] = s;
    count++;

    saveData();

    printf("Student added successfully!\n");
}

// Display ranking
void showRanking() {
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }

    qsort(students, count, sizeof(struct Student), compareMarks);

    printf("\n===== STUDENT RANKING =====\n");
    printf("%-8s %-15s %-10s %-8s\n",
           "Rank", "Name", "Marks", "ID");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-15s %-10.2f %-8d\n",
               i + 1,
               students[i].name,
               students[i].marks,
               students[i].id);
    }
}

// Search student by ID
void searchStudent() {
    int id;

    printf("Enter student ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("\nStudent Found!\n");
            printf("ID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Marks: %.2f\n", students[i].marks);
            return;
        }
    }

    printf("Student not found!\n");
}

// Calculate class average
void showAverage() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    float total = 0;

    for (int i = 0; i < count; i++) {
        total += students[i].marks;
    }

    printf("Class Average: %.2f\n", total / count);
}

int main() {
    int choice;

    loadData();

    do {
        printf("\n===== STUDENT PERFORMANCE SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Show Ranking\n");
        printf("3. Search Student\n");
        printf("4. Class Average\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                showRanking();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                showAverage();
                break;

            case 5:
                saveData();
                printf("Records saved. Goodbye!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 5);

    return 0;
}
