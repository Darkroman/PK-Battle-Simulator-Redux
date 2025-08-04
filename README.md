# PK-Battle-Simulator-Redux

<details>
   <summary>

## UPDATES

   </summary>

### 8/4/2025
- Made damage calculations more in line with the official games which use fixed point math using 4096 scale, rather than floating point math
- Fixed type effectiveness bugs where it wouldn't display "doesn't affect" message and gives incorrect values
- Changed how Substitute damage message is outputted. Will only say either "sub took damage" or "sub faded".
- Fixed burn penalty also applying to special moves
- No more double types, no more needless static_cast<int> and static_cast<double> since all calculations are either floored through int truncation or rounded using half divisor addition.

### 8/2/2025
- Move objects now have secondary effect chance included, allowed me to reduce the number of move routines down from 92 to 87
- Added forwarding methods to pokemonMove struct so no need to call currentMove->mp_move->... now can just call currentMove->...
- Redid logic for all my move routines so they're all more consistent with the official games for the most part and the code is more consistent across multiple move routines
- Fixed bugs with partial trapping moves. Now correctly ends with a "pokemon freed" message upon user of move fainting and no longer lingers, or if user of move switches out it ends silently but no message
- Some more include header file cleanup

### 7/27/2025
A few more architectural changes and a few bug fixes:
- Changed names of some classes and updated their file names:
- TurnManager -> BattleManager
- BattleStatusManager -> StatusEffectProcessor
- TurnUtils -> SwitchExecutor
- IMoveEffect -> IMoveRoutine

- Added the secondary effect chance to the Move objects themselves. This allows me to
consolidate a few of the chance to inflict status MoveRoutines into one. Also updated the secondary effect enums appropriately (e.g ParalyzeHit10 and ParalyzeHit30 are just ParalyzeHit now)
- Updated DoMove routines that inflict a non-volatile status using the currentMove EffectChance object member

Bug fixes:
- Lick now appropriately uses ParalyzeHit MoveEffect instead of Earthquake
- On MultiAttack, DoubleHit and Twineedle moves, Bide now correctly takes into account only the very last hit's damage
- Fixed Stomp bugs: No longer has chance to flinch if defending Pokemon has a substitute or on same turn substitute goes down
- Added or modified HasSubstitute and BypassSubstitute logic checks to many status effect and stat stage affecting moves

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
</details>


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
This is just a small console based Pokemon battle simulator program. Starts up with a setup menu to customize your own team of 6 Pokemon. You can choose any of the 151 Pokemon and any moves they could learn based on their generation 1 (Red/Blue/Yellow) learnsets. You're able to set a nickname, level, 4 moves, stat IVs and EVs per Pokemon and send them into battle! The battle and move mechanics are more akin to (up to) generation 9 (Scarlet/Violet), so no weird bind move bugs or hyper beam not having a recharge phase when enemy Pokemon faints for example. Generation 2 special attack/defense stat split as well as generation 4 physical/special move split also is incorporated. <br/>
<br/>
I started this project a long time ago and have been working on it off and on while also learning C++. I wanted to put what I've learned to practice, and Pokemon is one of my most cherished gaming series of all time. I don't plan on making this into a full graphical game (as there are also some copyright issues with that in any case). Just something to put out there to show what I've learned so far. Along with future updates I hope to be more proficient at architectural/code organization design.
</details>

<details>
  <summary>

## SCREENSHOTS

   </summary>
   
![Setup Menu](https://github.com/user-attachments/assets/3da09c41-9d3a-4c50-83ef-82a5c438c40e) ![Changing Move](https://github.com/user-attachments/assets/77723cef-5437-4dc4-93e6-b8546e79a299) 

![Battle 1](https://github.com/user-attachments/assets/7055fbcb-7391-4854-a01e-c61975625706) ![Battle 2](https://github.com/user-attachments/assets/8fc46a2e-2637-40db-abdd-06c234c1d162)
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
