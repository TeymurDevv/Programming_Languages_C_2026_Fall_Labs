// week5_task4_struct_binary_io.c   (BONUS)
// Name:Teymur  Surname:Suleymanov  Student ID:251ADB097
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int save_student_binary(const Student* s,
                        const char* filename);  // 0 on success, -1 on error
int load_student_binary(const char* filename,
                        Student* out);  // 0 on success, -1 on error
long file_size(const char* filename);   // bytes, -1 on error

int main(void) {
  Student s1 = {
      0};  // zero-initialised so unused name bytes are not garbage on disk
  strcpy(s1.name, "Alice");
  s1.age = 21;
  s1.gpa = 3.75f;

  Student loaded = {0};
  const char* bin_file = "student.bin";
  const char* txt_file = "student.txt";
  FILE* fp = NULL;

  printf("saving student in binary format...\n");
  if (save_student_binary(&s1, bin_file) != 0) {
    fprintf(stderr, "failed to save student in binary format\n");
    return EXIT_FAILURE;
  }

  fp = fopen(txt_file, "w");
  if (fp == NULL) {
    perror(txt_file);
    return EXIT_FAILURE;
  }
  fprintf(fp, "%s %d %.2f\n", s1.name, s1.age, s1.gpa);
  fclose(fp);

  printf("loading student from binary file...\n");
  if (load_student_binary(bin_file, &loaded) != 0) {
    fprintf(stderr, "failed to load student from binary file\n");
    return EXIT_FAILURE;
  }

  printf("loaded student: %s, %d, gpa %.2f\n", loaded.name, loaded.age,
         loaded.gpa);
  printf("text file size: %ld bytes\n", file_size(txt_file));
  printf("binary file size: %ld bytes\n", file_size(bin_file));

  return EXIT_SUCCESS;
}

int save_student_binary(const Student* s, const char* filename) {
  FILE* fp;

  fp = fopen(filename, "wb");
  if (fp == NULL) {
    return -1;
  }

  if (fwrite(s, sizeof(Student), 1, fp) != 1) {
    fclose(fp);
    return -1;
  }

  fclose(fp);
  return 0;
}

int load_student_binary(const char* filename, Student* out) {
  FILE* fp;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  if (fread(out, sizeof(Student), 1, fp) != 1) {
    fclose(fp);
    return -1;
  }

  fclose(fp);
  return 0;
}

long file_size(const char* filename) {
  FILE* fp;
  long size;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return -1;
  }

  size = ftell(fp);
  fclose(fp);

  if (size < 0) {
    return -1;
  }

  return size;
}
