# ProjectAlkaline

2D game engine in C++ mainly aimed at classic Isometric games such as Diablo, Sacred, Stronghold or Baldur's Gate.

The goal of this project is simply (re)learning C++ and core game programming concepts, so expect the overall architecture to change and evolve through time, and probably a lack of documentation.

## Project Structure

Project Alkaline is made out of multiple subprojects in order to make it easy to reuse in the future for different games and keep encapsulation of game-specific functionality.

- **AlkalineCore**: core engine implementation. Includes core Systems and Components, as well as serialization and script reflection functionalities.
- **AlkalineGame**: example of a game library. Currently only includes a Test component GameLogic subsystem for testing purposes.
- **AlkalineClient**: simply runs an Application instance.
- **AlkalineEditor**: project for the different tooling for games with the Alkaline Engine

## Features

The Alkaline Engine follows an Entity-Component-System architecture, and as of September 2025, implements the following:
- 2D isometric renderer
- Game Logic subsystems for gameplay
- Scene serialization/deserialization in Lua
- Input handling
- Scripting in Lua with systems exposing functionality to the scripts
- Debug options using ImGui
- Sigil defintion and spawning (similar to Unity Prefabs or UE Blueprints)

## Alkaline Editor

WIP editor for launching/stopping the Application, and we're currently building the following tools for it:
- Map Editor: tile based map editor, similar to Tiled, with custom export rules that integrate with the Alkaline Engine
- Sprite 3D Bounds Editor: for isometric rendering using AABB checks (not yet implemented in engine)

## External Libraries

- [Raylib](https://www.raylib.com/index.html)
- [ImGui](https://github.com/ocornut/imgui) + [rlImGui](https://github.com/raylib-extras/rlImGui)
- [Sol2](https://github.com/ThePhD/sol2)
- [Tracy Profiler](https://github.com/wolfpld/tracy)


## Contributors

[@PabloTomico](https://github.com/pablotomico)

[@JohnComrie](https://github.com/JohnComrie)
