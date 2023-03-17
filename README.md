HoMM3UnitHelper
====

Simple external overlay for the Heroes of Might and Magic 3, it aims to lower skill floor for the new players by displaying info about selected unit. Its main aprroach is to avoid DLL injection and any form of game data manipulation.

![Display unit stats example](https://raw.githubusercontent.com/ArtiFixal/HoMM3UnitHelper/main/data/exampleUnitStats.jpg)

### How it works
Program attaches to the game process and reads its memory. 

### Usage
1. Run the game.
2. Open program and click attach.
3. Right mouse click any unit in adventure map to display info about it.

### Features
- [x] Display unit info.
- [ ] Display artifact info.
- [ ] Display structure info.
- [ ] Read unit data directly from game files.

### Compatibility
- [x] HotA
- [ ] other

### Requirements
May require [vcredist](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist) x86 or both x64 and x86.
