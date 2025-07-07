# PK-Battle-Simulator

<details open>
  <summary>
    
## BUILD NOTES

  </summary>
This was coded in VS2022 Community, utilizing up to the C++20 standard features (std::filesystem, std::string_view and std::ranges). Uploaded in this repository is a simple CMake build, and releases come in a zip file containing the release executable build, the data text files and premade Pokemon parties if you just want to download and play. Only tested on Windows.
</details>

<details open>
  <summary>

## GENERAL INFO

   </summary>
This is just a small console based Pokemon battle simulator program. I started this project a long time ago and have been working on it off and on while also learning C++. I wanted to put what I've learned to practice, and Pokemon is one of my most cherished gaming series of all time. I don't plan on making this into a full graphical game (as there are also some copyright issues with that in any case). Just something to put out there to show what I've learned so far. Along with future updates I hope to be more proficient at architectural/code organization design.
</details>


<details>
   <summary>  
     
## FEATURES

   </summary>
   
- All of the generation 1 Pokemon
- All 165 moves and their respective secondary effects from generation 1, with generation 9 mechanics and stats.
- IVs, and EVs
- Physical/Special move split from gen 4 onward implemented.
- All generation 1 volatile and non-volatile status conditions working as they do in later generations.
- No A.I in the redux (yet). There is a simple one in my OG repo if you wanna try that one.
- There are however no natures, held items, or abilities. Whether I might add them in the future is up in the air.
</details>

<details>
  <summary>
    
## PLANNED UPDATES

   </summary>
   
- More sophisticated A.I
- Code architectural/organizational choices
- Not sure if I'll ever add in later generation stuff
</details>

<details>
   <summary>

## UPDATES

   </summary>
   
### 7/7/2025
- Added A.I (only easy difficulty right now)
- NOTE: When switching player one or two's controller type it will erase the team. Make sure to choose player type before setting up your team. (I'll fix that soon)
   
### 7/5/2025
- Redesigned the whole BattleSystem class. Now split up into smaller classes.
- Moves, Pokemon and their Learnsets now loaded from embedded calls rather than from a text file.
- Won't bother with listing bug fixes as there may be some fixed but new ones introduced that I haven't quite scrubbed out yet.
- From what I have tested it is in a fully playable state.
- AI has not been added yet
   
### 3/1/2025
- Some bug fixes in MakeEnemySwitch effect
- If Pokemon needs to recharge and is inflicted with a performative status condition, it will correctly now do the recharge turn first
- A couple other bug fixes

### 2/23/2025
- Fixed damage calculation bug
- Metronome and Mirror Move should now work correctly

### 2/19/2025:
- Fixed damage calculation bug when light screen or reflect are up
- "Set EV" menu now displays correctly max EV per stat as 252 instead of 255
- When showing stats in edit stats menu, speed stat now outputs.
- Counter correctly does 2x damage taken (originally had it like that but I changed stuff around and just forgot to readd in multiplying the variable by 2)
- Disable should now correctly fail when used on Struggle
- When choosing to fight with no moves left, will now automatically choose Struggle

### 2/18/2025:
- Added easy A.I (just picks random move and switches to next Pokemon in line when it faints)
- Note the default player settings is human. Make sure to set one of em to A.I if you wanna play against the computer, or both to A.I for funsies
- Some bug fixes with Bound damage, Reflect actually checking if it's already up rather than Light Screen, and some text dialog fixes
</details>
