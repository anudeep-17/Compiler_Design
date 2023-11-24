// header file of ABMMethods.c
#ifndef ABMMethods
#define ABMMethods

//Methods declarations
void abminstructionrunner(int clientsocketaddress,FILE* abmInstructionsfile);
int SendToBus(const int clientsocketaddress, const char* message, const char* value, const char* label);
#endif
