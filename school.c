#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student_Node {
    int Student_ID;
    char Student_Name[20];
    char Subject_Options[100]; // Increased size for multiple subjects
    struct Student_Node *next;
} Student;

typedef struct Teacher_Node {
    int Teacher_ID;
    char Teacher_Name[20];
    char Teaching_Subjects[20];
    struct Teacher_Node* next;
} Teacher;

void save_student_to_file(Student *student);

void add_student(Student **head, int Student_id, char* name, char* subject, int saveToFile) {
    Student *newStudent = (Student*) malloc(sizeof(Student));
    if (newStudent == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return;
    }
    newStudent->Student_ID = Student_id;
    strcpy(newStudent->Student_Name, name);
    strcpy(newStudent->Subject_Options, subject);
    newStudent->next = *head;
    *head = newStudent;
    if (saveToFile) {
        save_student_to_file(newStudent);
    }
}

void save_teacher_to_file(Teacher *teacher) {
    FILE *file = fopen("teachers.txt", "a"); // Open the file in append mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%d,%s,%s\n", teacher->Teacher_ID, teacher->Teacher_Name, teacher->Teaching_Subjects);
    fclose(file);
}

void add_teacher(Teacher **head, int Teacher_id, char* name, char* subject, int saveToFile) {
    Teacher *newTeacher = (Teacher*) malloc(sizeof(Teacher));
    if (newTeacher == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return;
    }
    newTeacher->Teacher_ID = Teacher_id;
    strcpy(newTeacher->Teacher_Name, name);
    strcpy(newTeacher->Teaching_Subjects, subject);
    newTeacher->next = *head;
    *head = newTeacher;
    if (saveToFile) {
        save_teacher_to_file(newTeacher);
    }
}

int loadTeachersFromFile(Teacher **head) {
    FILE *file = fopen("teachers.txt", "r");
    int maxID = 0;

    if (file == NULL) {
        return maxID; // Return 0 if file doesn't exist
    }

    int id;
    char name[20];
    char subjects[100];

    // Quick Note on how the code below works
    // "%d,%19[^,],%99[^\n]\n": This is the format string, guiding how fscanf reads from the file.
    // %d: Reads an integer and stores it in id.
    // %19[^,]: Reads a string of up to 19 characters or until a comma is encountered, and stores it in name. The [^,] part is a negated scanset, meaning it reads everything until it finds a comma.
    // %99[^\n]: Reads a string of up to 99 characters or until a newline character is encountered, and stores it in subjects. The [^\n] part means it reads everything until it finds a newline character.
    // \n: Reads the newline character at the end of the line.
    while (fscanf(file, "%d,%19[^,],%99[^\n]\n", &id, name, subjects) == 3) {
        add_teacher(head, id, name, subjects, 0); // Don't save to file
        if (id > maxID) {
            maxID = id; // Update maxID if current ID is greater
        }
    }

    fclose(file);
    return maxID;
}

void traverse_students(Student *head) {
    Student *temp = head;
    printf("====================================\n");
    printf("Students List:\n");
    printf("====================================\n");
    while (temp != NULL) {
        printf("ID:%d, Name: %s, Subject: %s\n", temp->Student_ID, temp->Student_Name, temp->Subject_Options);
        temp = temp->next;
    }
}

void traverse_teachers(Teacher *head) {
    Teacher *temp = head;
    printf("====================================\n");
    printf("Teachers List:\n");
    printf("====================================\n");
    while (temp != NULL) {
        printf("ID:%d, Name: %s, Subject: %s\n", temp->Teacher_ID, temp->Teacher_Name, temp->Teaching_Subjects);
        temp = temp->next;
    }
}

void free_student_list(Student **head) {
    Student *temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void free_teacher_list(Teacher **head) {
    Teacher *temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void save_student_to_file(Student *student) {
    FILE *file = fopen("students.txt", "a"); // Open the file in append mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%d,%s,%s\n", student->Student_ID, student->Student_Name, student->Subject_Options);
    fclose(file);
}

int load_students_from_file(Student **head) {
    FILE *file = fopen("students.txt", "r");
    int maxID = 0;

    if (file == NULL) {
        return maxID; // Return 0 if file doesn't exist
    }

    int id;
    char name[20];
    char subjects[100];

    // Quick Note on how the code below works
    // "%d,%19[^,],%99[^\n]\n": This is the format string, guiding how fscanf reads from the file.
    // %d: Reads an integer and stores it in id.
    // %19[^,]: Reads a string of up to 19 characters or until a comma is encountered, and stores it in name. The [^,] part is a negated scanset, meaning it reads everything until it finds a comma.
    // %99[^\n]: Reads a string of up to 99 characters or until a newline character is encountered, and stores it in subjects. The [^\n] part means it reads everything until it finds a newline character.
    // \n: Reads the newline character at the end of the line.
    while (fscanf(file, "%d,%19[^,],%99[^\n]\n", &id, name, subjects) == 3) {
        add_student(head, id, name, subjects, 0); // Don't save to file
        if (id > maxID) {
            maxID = id; // Update maxID if current ID is greater
        }
    }
    // Close the file when done
    fclose(file);
    return maxID;
}


int max_int = 0;

int main() {
    Student *students_head = NULL;
    Teacher *teachers_head = NULL;

    // Load students from file
    int highestStudentID = load_students_from_file(&students_head);
    // Load teachers from file
    int highestTeacherID = loadTeachersFromFile(&teachers_head);

    // Check if there are already registered students and print them
    if (students_head != NULL) {
        // printf("Registered Students:\n");
        traverse_students(students_head);
    } else {
        printf("No students are currently registered.\n");
    }

    // Check if there are already registered teachers and print them
    if (teachers_head != NULL) {
        // printf("Registered Teachers:\n");
        traverse_teachers(teachers_head);
    } else {
        printf("No teachers are currently registered.\n");
    }

    // Add new students
    int num_of_Students;
    printf("Input Number of Students to Register: ");
    scanf("%d", &num_of_Students);
    getchar(); // Consume the newline character after scanf
    
    for (int i = 0; i < num_of_Students; ++i) {
        highestStudentID++;
        char studentName[20], studentSubject[100] = "", tempSubject[20];
        printf("Enter student name: ");
        fgets(studentName, 20, stdin);
        studentName[strcspn(studentName, "\n")] = 0; 

        int addMoreSubjects;
        do {
            printf("Enter student subject: ");
            fgets(tempSubject, 20, stdin);
            tempSubject[strcspn(tempSubject, "\n")] = 0; 
            strcat(studentSubject, tempSubject);
            strcat(studentSubject, ", ");

            printf("Do you want to add another subject? (1 for yes, 0 for no): ");
            scanf("%d", &addMoreSubjects);
            getchar(); 
        } while (addMoreSubjects);

        add_student(&students_head, highestStudentID, studentName, studentSubject, 1);
    }

    // Add new teachers
    int num_of_Teachers;
    printf("Input Number of Teachers to Register: ");
    scanf("%d", &num_of_Teachers);
    getchar(); // Consume the newline character after scanf

    for (int i = 0; i < num_of_Teachers; ++i) {
        highestTeacherID++;
        char teacherName[20], teacherSubject[20];
        printf("Enter teacher name: ");
        fgets(teacherName, 20, stdin);
        teacherName[strcspn(teacherName, "\n")] = 0; 

        printf("Enter teacher subject: ");
        fgets(teacherSubject, 20, stdin);
        teacherSubject[strcspn(teacherSubject, "\n")] = 0; 

        add_teacher(&teachers_head, highestTeacherID, teacherName, teacherSubject, 1);
    }


    // Print the updated list of students
    if (students_head != NULL) {
        // printf("Updated Students List:\n");
        traverse_students(students_head);
    }

    // Print the updated list of teachers
    if (teachers_head != NULL) {
        // printf("Updated Teachers List:\n");
        traverse_teachers(teachers_head);
    }

    // Free the student list
    free_student_list(&students_head);
    // Free the teacher list
    free_teacher_list(&teachers_head);

    return 0;
}

// Run the program with the following command: gcc -o school school.c
// Then run the executable with the following command: ./school