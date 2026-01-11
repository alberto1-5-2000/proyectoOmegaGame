#include "GameModes/FPSGameMode.h"

#include "Characters/FPSCharacter.h"

AFPSGameMode::AFPSGameMode()
{
    DefaultPawnClass = AFPSCharacter::StaticClass();
}
