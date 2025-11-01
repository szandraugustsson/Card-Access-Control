#include <termios.h> //hanterar terminalinställningar
#include <unistd.h> //POSIX funktioner som STDIN_FILENO
#include <stdio.h> 

//ignorerar alla tecken och väntar på enter, så användaren kan inte skriva något
void waitForEnter()
{
    struct termios oldt, newt;
    int c;

    printf("Press enter to return to the admin menu");

    tcgetattr(STDIN_FILENO, &oldt);  //spara terminalinställningar
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); //disable buffring & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((c = getchar()) != '\n') 
    {
        //ignorera alla tecken
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  //återställ inställningar
}