# PK-Battle-Simulator

<details open>
  <summary>
    
## BUILD NOTES

  </summary>
This was coded in VS2022 Community, utilizing up to the C++20 standard features (std::filesystem, std::string_view and std::ranges). Uploaded in this repository is a simple CMake build. Only tested on Windows.
</details>

<details open>
  <summary>

## GENERAL INFO

   </summary>
This is just a small console based Pokemon battle simulator program. I started this project a long time ago and have been working on it off and on while also learning C++. I wanted to put what I've learned to practice, and Pokemon is one of my most cherished gaming series of all time. I don't plan on making this into a full graphical game (as there are also some copyright issues with that in any case). Just something to put out there to show what I've learned so far. Along with future updates I hope to be more proficient at architectural/code organization design.
</details>

<details open>
  <summary>

## Screenshots
![Setup Menu](https://github.com/user-attachments/assets/3da09c41-9d3a-4c50-83ef-82a5c438c40e) ![Changing Move](https://github.com/user-attachments/assets/77723cef-5437-4dc4-93e6-b8546e79a299) 

![Battle 1](https://github.com/user-attachments/assets/7055fbcb-7391-4854-a01e-c61975625706) ![Battle 2](https://github.com/user-attachments/assets/8fc46a2e-2637-40db-abdd-06c234c1d162)


   </summary>

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
- A.I (only an easy mode right now)
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

### 7/11/2025
- Made it so TurnManager correctly exhibits ownership of other classes and what it borrows (many of the battle related classes it should instantiate rather than Game.h itself).
- Fixed up some inconsistencies having to do with random rolls (should all be 1-100 now instead of some being 1-101).
- Fixed CalculateHitChance to actually use a real distribution of a roll (0.0, 100.0) for more correct statistical outcomes when chance to hit is calculated lower than 100%.
- Added correct Bide behavior based on Bulbapedia info. Sleep should disrupt it no matter what, and a full paralyze on a turn neither pauses nor disrupts Bide.
- Added correct behavior for a transform quirk. Transform now copies over the attack stat penalty if burned, and speed stat penalty if paralyzed, but not the status itself. If transformed pokemon is then burned or paralyzed, it does NOT stack.

### 7/9/2025
- Fixed the erasing Pokemon team on switching player controller type issue.
- Some more software architectural changes and code cleanup. Moved most console text output to separate classes.
   
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
