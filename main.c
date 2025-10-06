#include <stdio.h>
#include <stdbool.h>

int main()
{
    while(true)
    {
        printf("Admin menu\n");
        printf("1. Remote open door\n");
        printf("2. List all cards in system\n");
        printf("3. Add/remove access\n");
        printf("4. Exit\n");
        printf("9. FAKE TEST SCAN CARD\n");
        printf("Select option: \n");
        
        int option;
        scanf(" %d", &option);
        if (option == 4)
            break;
    }

return 0;

}