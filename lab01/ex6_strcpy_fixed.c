#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // TODO: Create space to store the string "hello"
  // You may use your solution from a previous exercise;
  char message[6] = "hello";

  // Print out the value before we change message
  printf("Before copying: %s\n", message);

  // Creates another_string that contains a longer string
  char* long_message = "Here's a really long string";

  // TODO: Copy the string in long_message to message
  strncpy(message, long_message, strlen(message));

  // Print out the value after we change message
  printf("After copying: %s\n", message);

  // Out of curiosity check the contents and length of message
  printf("6th index of message: %s\n", message[5]);
  printf("Length of message: %d\n", strlen(message));

  return 0;
}
