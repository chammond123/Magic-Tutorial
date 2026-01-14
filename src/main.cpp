#include <iostream>
#include "../headers/gamemanager.h"
#include "../headers/gamestate.h"

int main() {
    std::cout << "Magic Tutorial - C++ Game Engine" << std::endl;
    std::cout << "=================================" << std::endl;
    
    // Create game manager
    gamemanager manager;
    
    // TODO: Set up event system to replace Qt signals/slots
    // This is where you'd connect your UI or event handlers
    
    std::cout << "Game engine initialized successfully!" << std::endl;
    std::cout << "Current phase: " << static_cast<int>(manager.state->currentPhase) << std::endl;
    
    // TODO: Implement game loop
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}