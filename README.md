HoMM3UnitHelper
=====

External overlay for Heroes of Might and Magic 3 to display stats of selected unit, designed to avoid DLL injection and game manipulation.
It aims to lower the skill floor for new players.

![Example - selected unit](https://raw.githubusercontent.com/ArtiFixal/HoMM3UnitHelper/main/exampleUnitStats.jpg)

#Features:
- [x] Displaying unit info
- [ ] Displaying artifact info
- [ ] Displaying structure info
- [ ] Read unit stats directly from game data

#Usage
1. Run the game.
2. Click attach in program menu.
3. While in adventure map click with right mouse button on unit to display its stats.

#How it works:
Program attaches to the game process and reads its memory.

#Compability:
- [x] HotA
- [ ] Other

#Requirements
May require [vcredist](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist) x86 (or both x86 and x64) to launch.
