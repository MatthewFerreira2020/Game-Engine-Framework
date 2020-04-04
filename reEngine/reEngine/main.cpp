#include"Engine/Core/Engine.h"
#include"Game\Game1.h"

int main(int argc, char* args[]) {

	EngineClass::GetInstance()->SetGameInterface(new Game1());
	EngineClass::GetInstance()->Initialize("engine", 800, 600);
	EngineClass::GetInstance()->Run();

	return 0;
}