#ifndef CARDFUNCTIONS_H
#define CARDFUNCTIONS_H

#include <stdbool.h>
#include <time.h> //time_t

//ett enda kort
typedef struct
{
    int cardNumber; //kortets unika nummer
    bool hasAccess; //true om har access, annars false
    time_t dateAdded; //datum och klockslag man lägger till kortet
} Card;

//en lista med kort
typedef struct
{
    Card *cards; //pekare till en array av "Card" (till flera kort)
    int count; //antalet kort i listan
} CardList;

void loadCardsFromFile(CardList *allCards);
void saveCardsToFile(const CardList *allCards);
void remoteOpenDoor();
void PrintCards(const Card *p);
void listAllCards(const CardList *allCards);
void InputCard(Card *p);
void CreateCard(CardList *allCards);
void AddAccess(CardList *allCards, int cardNumberInput);
void RemoveAccess(CardList *allCards, int cardNumberInput);
void addRemoveAccess(CardList *allCards);
void fakeCardScan(CardList *allCards);

#endif