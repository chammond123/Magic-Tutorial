# MagicTutorial

A **Qt6-based Magic: The Gathering game engine** built as a comprehensive tutorial application. Features a turn-based game simulation with Scryfall API integration, physics-based card interactions, and an interactive learning system.

## Overview

MagicTutorial demonstrates a complete game implementation from rules engine to UI—showcasing software architecture patterns, API integration, and real-time game state management. The project serves as both a functional Magic: The Gathering learning tool and a reference architecture for turn-based card game systems.

### Key Features

- **Complete Game Rules Engine**: 11-phase turn structure with full validation pipeline, mana system, and card zone management
- **Scryfall API Integration**: Dynamic card fetching with metadata caching and async progress tracking
- **Interactive Learning System**: Tutorial mode with contextual game tips and guided play
- **Physics-Based Interactions**: Box2D integration for future card animation and visual effects
- **AI Opponent**: Rule-compliant bot opponent for solo play
- **Signal/Slot Architecture**: Decoupled game logic from UI using Qt's event-driven patterns
- **Command Pattern Implementation**: Action history and undo support via command objects

## Tech Stack

- **Language**: C++17
- **UI Framework**: Qt 6.x
- **Physics Engine**: Box2D (embedded)
- **External API**: Scryfall Card Database
- **Build System**: Qt Creator / qmake

## Architecture Highlights

### Layered Design

```
┌─────────────────────────┐
│   UI Layer              │
│ (MainWindow, CardButton)│
└────────────┬────────────┘
             │ signals/slots
┌────────────▼────────────┐
│  Game Manager           │
│  (Commands + Validation)│
└────────────┬────────────┘
             │
┌────────────▼────────────┐
│  Game Engine            │
│  (GameState, Player,    │
│   Card, Zone, Phase)    │
└─────────────────────────┘
```

**Key Components:**

- **`GameState`**: Enforces 11-phase turn structure, validates card legality, manages the stack
- **`GameManager`**: Central orchestrator receiving player actions via signals, executing commands
- **`Player`**: Manages zones (Library, Hand, Graveyard, Exile, Battlefield), mana pool, life total
- **`CardDictionary`**: Cached singleton providing card properties; pre-populated from `uniqueCards.txt`
- **`CardAPIManager`**: Async Scryfall API client with batch fetching and progress signals
- **`Zone`**: Stack-based card container (index 0 = bottom, last = top) with shuffle/draw semantics

### Design Patterns

| Pattern | Implementation | Benefit |
|---------|---|---|
| **Command** | `playCardCommand` objects stored in `GameState` | Enables countering and resolving cards true to ruleset |
| **Signal/Slot** | Qt event-driven arch for action dispatch | Decouples game logic from UI; testable game engine |
| **Dictionary** | `CardDictionary` static cache | O(1) card property lookups; memory efficient |
| **Observer** | Zone/Player state changes emit signals | UI auto-updates without polling |


## Quick Start

### Prerequisites
- Qt 6.x (with Qt Creator)
- MinGW 64-bit compiler
- CMake (for Box2D compilation)

### Building

1. **Clone and navigate to project:**
   ```bash
   git clone https://github.com/your-org/cs3505-assignment9-Tweagan11.git
   cd cs3505-assignment9-Tweagan11
   ```

2. **Open in Qt Creator:**
   - File → Open File or Project
   - Select `MagicTutorial.pro`

3. **Build and Run:**
   - Press `Ctrl+R` to build and run
   - Or use Build menu → Run

### First Game
- Launch the application
- Select "New Game" from main menu
- Game board appears with tutorial tips enabled by default

## How to Extend

### Adding a New Card Feature

1. **Extend the `Card` class** ([card.h](card.h)):
   ```cpp
   bool hasIndestructible;  // New property
   ```

2. **Configure in `CardDictionary::giveAbility()`** ([carddictionary.cpp](carddictionary.cpp)):
   ```cpp
   if (c->name == "Darksteel Colossus") {
       c->hasIndestructible = true;
   }
   ```

3. **Update game logic** ([gamestate.cpp](gamestate.cpp)):
   ```cpp
   void GameState::validateBattlefield(Player* player) {
       // Use hasIndestructible flag in damage/death logic
   }
   ```

### Adding a New Game Screen

1. Create new `.ui` file in Qt Designer
2. Implement header class inheriting from `QWidget`
3. Connect signals to `MainMenu` for navigation
4. Add `.qrc` resource entry for fonts/icons

### Expanding the Card Pool

Current deck sources:
- `additional_files/uniqueCards.txt`: Full card list (loaded into `CardDictionary`)
- `additional_files/deck.txt`: Default starting deck

To add cards:
1. Add card name to `uniqueCards.txt`
2. Define properties in `CardDictionary::giveAbility()` (or future smart parser)

## Future Roadmap

**Planned Rewrites & Improvements:**

- [ ] **React + Node.js Migration**: More approachable codebase, cross-platform support (web, mobile)
- [ ] **Smart Card Parser**: Automatic rule translation from Scryfall card text to game engine rules
- [ ] **Expanded Card Pool**: Build out comprehensive card library with all rules variations
- [ ] **Advanced AI Opponent**: Minimax with alpha-beta pruning for strategic plays
- [ ] **Multiplayer Networking**: Turn-based server for remote games
- [ ] **Replay System**: Save/load games with full move history
- [ ] **Mobile UI**: Responsive design for tablet/mobile play

## Credits

- **Game Engine**: Magic: The Gathering rules & mechanics
- **Card Database**: [Scryfall API](https://scryfall.com/docs/api) — Comprehensive Magic card data
- **Physics**: [Box2D](https://box2d.org/) — Physics engine by Erin Catto
- **Framework**: [Qt Project](https://www.qt.io/) — Cross-platform application framework


---

**Questions?** Open an issue or reach out to the development team.
