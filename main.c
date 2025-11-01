#include <stdio.h>
#include <stdbool.h>
#include "menu.h"
#include "cardfunctions.h"
#include "safeinput.h"
#include "waitforenter.h"

int main()
{   
    CardList allCards; //skapar en lista med kort som kan innehålla många 'Card'
    allCards.cards = NULL; //pekar till alla korten
    allCards.count = 0; //0 kort i listan

    loadCardsFromFile(&allCards);

    while(1)
    {
        printMenu();
        printf("\n");

        int selection;
        
        if(!GetInputInt("Select option: ", &selection))
        {
            printf("Invalid choice. Choose between 1, 2, 3, 4 and 9\n");
            waitForEnter();
            continue;
        }
        printf("\n");

        switch(selection)
        {
            case 1:
                remoteOpenDoor();
                break;
            case 2:
                listAllCards(&allCards); //skickar pekare, inte kopia
                break;
            case 3:
                addRemoveAccess(&allCards);
                break;
            case 4:
                //alla kort ska sparas i filen innan programmet slutar
                saveCardsToFile(&allCards);
                return 0;
            case 9:
                fakeCardScan(&allCards);
                break;
            default:
                printf("Invalid choice. Choose between 1, 2, 3, 4 and 9\n");
                waitForEnter();
        }
    }
}