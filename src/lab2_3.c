#include <stdio.h>

/*
    Task:
    Write a function `int is_prime(int n)` that returns 1 if n is prime,
    0 otherwise.

    In main():
      - Ask user for an integer n (>= 2)
      - If invalid, print an error
      - Otherwise, print all prime numbers up to n
*/

int is_prime(int n) {
  if (n < 2) {
    return 0;
  }

  for (int i = 2; i <= n / i; i++) {
    if (n % i == 0) {
      return 0;
    }
  }

  return 1;
}

int main(void) {
  int n;

  printf("Enter an integer n (>= 2): ");

  if (scanf("%d", &n) != 1 || n < 2) {
    printf("enter an integer >= 2.\n");
    return 1;
  }

  for (int i = 2; i < n; i++) {
    if (is_prime(i)) {
      printf("%d ", i);
    }
  }

  if (is_prime(n)) {
    printf("%d ", n);
  }

  printf("\n");

  return 0;
}