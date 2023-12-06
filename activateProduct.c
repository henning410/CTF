#include <stdio.h>
#include <string.h>

char VT_RED[] = "\033[31m";
char VT_GREEN[] = "\033[32m";
char VT_RESET[] = "\033[0m";

int checkKey(char *key) {
   
    return strcmp(key, "YouGotSomeFlag!!!") == 0;
}
//Funcion principal
int main() {
    //Buffer key
    char key[100];
    
    //Asks the user for the key
    printf("%s[ERROR]%s the product is not activated\n", VT_RED, VT_RESET);
    printf("Enter the activation key: ");
    scanf("%s", key);
    //Checks if the key is correct
    if (checkKey(key)) {
        //If it's correct, give the okay
        printf("%s[OK]%s Product activated successfully\n", VT_GREEN, VT_RESET);
        printf("ASOC{l4_cl4v3_3st4_3n_3l_c0d1g0}\n");
    } else {
        //Otherwise, alert the user
        printf("%s[ERROR]%s Incorrect key\n", VT_RED, VT_RESET);
    }
}
