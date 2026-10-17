// week5_task3_student_management_system.c
// Name:Teymur  Surname:Suleymanov  Student ID:251ADB097
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int load_students(Student arr[], int max);  // returns number of records loaded
int save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int* count);
void list_students(const Student arr[], int count);
int read_line(const char* prompt, char* buf, size_t size);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  char input[LINE_LEN];
  int running = 1;

  count = load_students(students, MAX_STUDENTS);
  printf("loaded %d student(s) from %s\n", count, DATA_FILE);

  while (running) {
    printf("\n=== student management system ===\n");
    printf("1. list students\n");
    printf("2. add student\n");
    printf("3. save and exit\n");

    int choice;
    if (read_line("select an option: ", input, sizeof input) != 0) {
      choice = 3;
    } else {
      char* end;
      long value = strtol(input, &end, 10);
      choice = (end != input && *end == '\0') ? (int)value : 0;
    }

    switch (choice) {
      case 1:
        list_students(students, count);
        break;
      case 2:
        add_student(students, &count);
        break;
      case 3:
        if (save_students(students, count) != 0) {
          fprintf(stderr, "failed to save students to %s\n", DATA_FILE);
          return EXIT_FAILURE;
        }
        printf("data saved. goodbye!\n");
        running = 0;
        break;
      default:
        printf("invalid option. try again.\n");
    }
  }

  return EXIT_SUCCESS;
}

int read_line(const char* prompt, char* buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  size_t len = strcspn(buf, "\r\n");
  if (buf[len] == '\0' && len == size - 1) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }
  buf[len] = '\0';
  return 0;
}

int load_students(Student arr[], int max) {
  FILE* fp;
  char line[LINE_LEN];
  int count = 0;

  fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    return 0;
  }

  while (count < max && fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%49[^,],%d,%f", arr[count].name, &arr[count].id,
               &arr[count].gpa) == 3) {
      count++;
    }
  }

  fclose(fp);
  return count;
}

int save_students(const Student arr[], int count) {
  FILE* fp;
  int i;

  fp = fopen(DATA_FILE, "w");
  if (fp == NULL) {
    return -1;
  }

  for (i = 0; i < count; i++) {
    fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa);
  }

  fclose(fp);
  return 0;
}

void add_student(Student arr[], int* count) {
  char name[NAME_LEN];
  char id_buf[LINE_LEN];
  char gpa_buf[LINE_LEN];
  char* id_end = NULL;
  char* gpa_end = NULL;
  long id_value;
  float gpa_value;

  if (*count >= MAX_STUDENTS) {
    printf("cannot add more students (limit reached).\n");
    return;
  }

  if (read_line("enter name: ", name, sizeof(name)) != 0) {
    return;
  }
  if (name[0] == '\0' || strchr(name, ',') != NULL) {
    printf("invalid input. student not added.\n");
    return;
  }

  if (read_line("enter id: ", id_buf, sizeof(id_buf)) != 0) {
    return;
  }
  id_value = strtol(id_buf, &id_end, 10);
  if (id_buf[0] == '\0' || id_end == id_buf || *id_end != '\0') {
    printf("invalid input. student not added.\n");
    return;
  }

  if (read_line("enter gpa: ", gpa_buf, sizeof(gpa_buf)) != 0) {
    return;
  }
  gpa_value = strtof(gpa_buf, &gpa_end);
  if (gpa_buf[0] == '\0' || gpa_end == gpa_buf || *gpa_end != '\0' ||
      gpa_value < 0.0f || gpa_value > 4.0f) {
    printf("invalid input. student not added.\n");
    return;
  }

  snprintf(arr[*count].name, sizeof(arr[*count].name), "%s", name);
  arr[*count].id = (int)id_value;
  arr[*count].gpa = gpa_value;
  (*count)++;

  printf("student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  int i;

  if (count == 0) {
    printf("no students found.\n");
    return;
  }

  printf("id    name                 gpa\n");
  for (i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}
