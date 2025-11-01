#include <stdio.h>
#include <unistd.h> //sleep
#include <stdlib.h> //malloc
#include <string.h>
#include <ctype.h> //toupper
#include "cardfunctions.h"
#include "safeinput.h"
#include "waitforenter.h"

#define RED   "\x1B[31m" //byt färg till röd
#define GREEN "\x1B[32m" //byt färg till grön
#define RESET "\x1B[0m" //äterställ till normalt läge

void loadCardsFromFile(CardList *allCards)
{
    //öppnar filen i binärt läsläge
    FILE *f = fopen("cardlist.bin", "rb");
    if (f == NULL)
    {
        printf("Error! The file couldn't be opened!");
        return;
    }
    //läser in hur många kort som ska läsas in
    fread(&allCards->count, sizeof(int), 1, f);
    //allokerar minne för listans kort
    //vi vet precis hur mycket plats behövs och därför ska vi inte utöka med realloc för att läsa in
    allCards->cards = (Card *)malloc(allCards->count * sizeof(Card));
    //läsa in alla kort från filen
    fread(allCards->cards, sizeof(Card), allCards->count, f);
  
    fclose(f); //stänger filen
}

void saveCardsToFile(const CardList *allCards)
{   
    //filen öppnas i binärt skrivläge
    //(skapas filen om den inte finns eller töms om den finns)
    FILE *f = fopen("cardlist.bin", "wb");
    if (f == NULL)
    {
        printf("Error! The file couldn't be opened!");
        return;
    }
    //först skriva i filen hur många kort som ska läsas in
    //(här kan vi veta inte som när användaren skriver)
    //->funktionen vet sen hur mycket plats ska allokeras för korten
    fwrite(&allCards->count, sizeof(int), 1, f);

    //skriver alla kort i filen
    fwrite(allCards->cards, sizeof(Card), allCards->count, f);
    /*eller:
    for (int i = 0; i < allCards->count; i++)
    {
       fwrite(&allCards->cards[i], sizeof(Card), 1, f);
    }
    */
    
    fclose(f); //stänger filen
}

//1. Remote open door
void remoteOpenDoor()
{
    printf(GREEN "CURRENTLY LAMP IS: GREEN\n" RESET);
    sleep(3);
    printf("CURRENTLY LAMP IS: Off\n");
}

//2. List all cards in system
//'PrintCards' är separerat isf man vill skriva ut ett kort och inte hela listan
void PrintCards(const Card *p)
{
    printf("%d %s Added to system: %s\n", p->cardNumber, p->hasAccess ? "Access" : "No access", ctime(&p->dateAdded)); //utan ctime ser man bara ett konstigt värde
}

void listAllCards(const CardList *allCards) //det ska inte ändra listan, read-only
{
    if (allCards->count > 0)
    {
        printf("All cards in system:\n");
        for (int i = 0; i < allCards->count; i++)
        {
            PrintCards(&allCards->cards[i]);
        }
    }
    else
    {
        printf("No cards added\n");
    }
    waitForEnter();
}

//3. Add/remove access (+ create new card)
void InputCard(Card *p)
{
    printf("New card\n");
    while(1)
    {   
        if (!GetInputInt("Card number: ", &p->cardNumber))
        {
            printf("Invalid choice, enter a number\n");
            printf("\n");
            continue;
        }
        // vill inte att användaren ska kunna välja nr 0
        if (p->cardNumber == 0)
        {
            printf("You can't choose 0.\n");
            continue;
        }
        break;
    }

    int access;
    //skriv ut så länge användaren skriver fel
    while(1)
    {
        //om input inte är ett heltal
        if(!GetInputInt("Has access (1 = yes, 0 = no:) ", &access))
        {  
            printf("Invalid choice, enter 1 or 0\n");
            printf("\n");
            continue; 
        }

        //om input heltal men inte 0 eller 1
        if (access != 0 && access != 1)
        {
            printf("Invalid choice, enter 1 or 0\n");
            printf("\n");
            continue;
        }
        break;
    }
    p->hasAccess = access; //om man skriver annat än 0, blir det true
    p->dateAdded = time(NULL);
}

void CreateCard(CardList *allCards)
{
    allCards->count++;
    //först behöver vi plats i minnet för korten
    //om det är det första kortet -> malloc -> allokera minne för ett kort
    if (allCards->count == 1)
    {
        allCards->cards = (Card *)malloc(1 * sizeof(Card));
    }
    //annars, om listan redan innehåller ett kort -> utöka med realloc
    else
    {
        allCards->cards = (Card *)realloc(allCards->cards, allCards->count * sizeof(Card));
    }
    //skickar in sista kortet som lagts till
    InputCard(&allCards->cards[allCards->count - 1]);
}

void AddAccess(CardList *allCards, int cardNumberInput)
{
    for (int i = 0; i < allCards->count; i++)
    {
        if (allCards->cards[i].cardNumber == cardNumberInput)
        {
            allCards->cards[i].hasAccess = true;
        } 
    }
}

void RemoveAccess(CardList *allCards, int cardNumberInput)
{
    for (int i = 0; i < allCards->count; i++)
    {
        if (allCards->cards[i].cardNumber == cardNumberInput)
        {
            allCards->cards[i].hasAccess = false;
        } 
    }
}

void addRemoveAccess(CardList *allCards)
{
    int cardNumberInput;

    while(1)
    {   
        if (!GetInputInt("Enter card number: ", &cardNumberInput))
        {
            printf("Invalid choice, enter a number\n");
            printf("\n");
            continue;
        }
        break;
    }
    
    for (int i = 0; i < allCards->count; i++)
    {
        if (allCards->cards[i].cardNumber == cardNumberInput)
        {
            printf("This card %s access\n", allCards->cards[i].hasAccess ? "has" : "has no");

            int answerAccess;

            while(1)
            {
                if(!GetInputInt("Enter 1 for access, 2 for no access: ", &answerAccess))
                {
                    printf("Invalid choice\n");
                    printf("\n");
                    continue;
                }
            
                if (answerAccess == 1)
                {
                    AddAccess(allCards, cardNumberInput);
                    printf("Access given to card number %d\n", cardNumberInput);
                }
                else if (answerAccess == 2)
                {
                    RemoveAccess(allCards, cardNumberInput);
                    printf("Access removed from card number %d\n", cardNumberInput);
                }
                else
                {
                    printf("Invalid choice\n");
                    continue;
                }
                break;
            }
            waitForEnter();
            return;
        } 
    }
    
    //om inte hittar kortet
    printf("Card number is not found\n");

    while(1)
    {
        char newCardAnswer;
        GetInputChar("Do you want to add a new card? Y/N: ", &newCardAnswer);
        printf("\n");

        newCardAnswer = toupper(newCardAnswer);

        if (newCardAnswer == 'N')
        {
            break;
        }
        else if (newCardAnswer == 'Y')
        {
            CreateCard(allCards);
            printf("Card is created\n");
            waitForEnter();
            return;
        }
        else
        {
            printf("Invalid choice. Choose between Y and N\n");
            continue;
        }    
    }
}

//9. Fake card scan test
void fakeCardScan(CardList *allCards)
{
    printf("CURRENTLY LAMP IS: Off\n");

    char input[10];
    int scannedCard;

    while(1)
    {
        GetInput("Please scan card to enter or X to go back to admin menu\n", input, sizeof(input));
        // == funkar inte med 'sträng' så därför strcmp
        if (strcmp (input, "X") == 0 || strcmp (input, "x") == 0)
        {
            return;
        }

        //konvertera till int
        scannedCard = atoi(input);

        //om användaren skriver t.ex bokstäver eller 0 konverterar den till 0
        if (scannedCard == 0)
        {
            printf("Invalid choice\n");
            continue;
        }
        break;
    }

    for (int i = 0; i < allCards->count; i++)
    {
        if (allCards->cards[i].cardNumber == scannedCard)
        {
            if (allCards->cards[i].hasAccess)
            {
                printf(GREEN "CURRENTLY LAMP IS: Green\n" RESET);  
            }
            else
            {
                printf(RED "CURRENTLY LAMP IS: Red\n" RESET);
            }
            sleep(3);
            //printf("CURRENTLY LAMP IS: Off\n");
            return;
        }
    }

    printf(RED "CURRENTLY LAMP IS: Red\n" RESET);
    sleep(3);
    //printf("CURRENTLY LAMP IS: Off\n");
}