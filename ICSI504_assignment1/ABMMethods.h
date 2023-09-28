#ifndef ABMMethods
#define ABMMethods
#include "Stack.h"
#include "Variablearray.h"
#include "addresstovaluedict.h"
void abminstructionrunner(FILE* abmInstructionsfile, struct CharStack* stack, struct Stringarray* array, struct Map* map);

#endif
