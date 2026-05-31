# PK-Battle-Simulator-Redux

<details>
   <summary>

## UPDATES

   </summary>

### 5/30/2026
- **More AI switching logic improvements**
  - Optimized the mid-turn and Post-KO switching algorithms which resulted in decent performance gains in simulation times with medium and hard difficulty AIs.
  - Better AI switching logic. Should result in much less switch jittering, more consistent and conservative decision making.
    - AI switching won't be as aggressive to switch to another Pokemon.
    - AI shouldn't be sending in a Pokemon to its death.
- **Simulations now added into the main menu**
  - You can sim x amount of iterations of 2 customizable AI teams. You can use this for benchmarking and/or see how different trainers at different AI levels compare against eachother.
  - **NOTE:** The simulations will utilize all your CPU cores to the fullest. Recommend not to go over 10 million iterations. Even less iterations on less powerful CPUs. It's just gonna take a while.
    - As an example on my 8 core 16 thread AMD 5800x CPU, Red trainer vs Blue trainer and with both AIs at hard difficulty, 10 million iterations takes around 25-27 seconds.
- **Developer notes**
  - Besides the AI switching algorithms, I also converted my Move Routines to use compile-time function pointer jump table instead std::variant which resulted in a negligible speed bump, but slightly cleaner code.
  - The other big performance gain was actually from making sure to not needlessly run RandomizePostTurnOrder() in my PostTurnEffectProcessor after every status check.
    - The RandomizePostTurnOrder() method only runs when both Pokemon have a specific status (such as both having leech seed or both having some sort of damage status or same field effect).
  - Stage ratios are no longer stored, they are calculated but result in the same outcomes. This allowed for multiple files having access to the same GetStageRatio() constexpr functions.
  - Added MoveRoutines namespace. 
  - StageRatio.h, AppState.h, PokedexConstexpr.h, MovedexConstexpr.h, and LearnsetData.h files added.
    - Removed LoadEmbed .cpp files.
  - Instead of Pokemon, moves and learnsets being instantiated via std::vector, they're compile-time constant arrays now.
  - Experimented with data types. A lot of int types are now unsigned int types. Also using size_t a little more when indexing into contiguous memory data structures.

### 5/15/2026
- **AI Switching improvements, additions and bug fixes**
  - Added a few more gating checks for when AI determines if it should switch or not.
  - Fixed many bugs when determining which Pokémon to switch to in both mid-turn and post-KO.
    - Bugs were similar to gen 4's AI switching bugs funnily enough such as using current active pokemon to determine offensive/defensive values.
  - Easy difficulty AI will not do mid-turn switching.
- **Hard difficulty AI implemented**
  - For now just in switching logic. Hard or expert AI type move scoring not implemented yet.
- **New party Pokémon now default to all 31 IVs**
  - This includes any Pokémon that are loaded from a saved party file that do not have any IVs.
- **Saving and loading now more modular**
  - You can load any saved party file to any player rather than just Red to player one, and Blue to player two.
  - You can save the party file as any name but the name must starts with a letter.
- **Developer Notes**
  - Did a lot of code cleanup in various places.
    - Changed many for loops to range based for loops where I could.
    - Fixed up some const correctness.
    - Made use of std::span.
    - Added a headless simulation option that tests battles across all your CPU threads (only available through cloning the source and editing).
    - Various bug fixes that I didn't really document.
    - If you want to try to simulate many battle iterations yourself, clone this GitHub, edit GameEngine.cpp, change the SetupUI(false) to true, and further down where you see currentState = AppState::InitBattle, change to AppState::Simulate and build.
    - You can change the amount of iterations under void GameEngine::RunSimulate() with the simIterations variable.
    - Make sure both players are set to AI when doing this!
   
### 3/6/2026
- **Medium difficulty AI implemented**
  - Medium difficulty discourages moves that deal less damage than the highest damaging available move.
    - Will also evaluate charge and recharge based moves.
  - Easy difficulty has been toned down a bit.
    - Easy no longer considers the highest damaging move and instead randomly selects from the highest scored moves.
- **AI Switching Improvements**
  - Added additional check to mid-turn AI Switch logic.
    - Considers whether enemy Pokemon's typing is advantageous even without observing any moves.
  - Separated some mid-turn AI switch logic into easy and medium variants.
    - Post-KO switching remains unchanged.
- **UI Improvements**
  - Added Menu options to change player's controller type to medium difficulty.
  - The controller selection menu now displays the AI difficulty of each controller type.
- **Fixes**
  - Correctly sets player's CanSwitch flag back to true if their pokemon fainted.
  - Fixed a crash caused by an early return during AI action selection.
    - The early return set the action to Fight without selecting a move, resulting in a nullptr move error.
- **Developer Notes**
  - Added MediumScoringMove.h/.cpp with the MediumScoringMove namespace and associated methods.
  - AIController constructor is now explicit and takes a Difficulty enum class.
  - Added AIScoreTag to ScoringResults struct.
    - Score tags are now initialized when moves are first gathered for scoring.

### 3/1/2026
**MAJOR UPDATE** 
- **Basic AI implemented!**
  - Implemented Basic AI move scoring largely inspired by Gen 4 Basic AI trainer scoring.
    - For now final phase of move scoring logic randomly picks between highest tieing scored status move and highest damage move.
  - AI difficulty differentiation planned for future updates.
    - Easy will be slightly toned down and medium a little more competitive.
    - Expert AI or hard based scoring implementation planned.
  - Easy AI now behaves more like a standard trainer battle instead of a wild Pokemon battle.
  - AI Switching logic implemented.
- **Fixes**
  - Thunder wave now correctly doesn't affect ground types.
  - Fixed bug where speed EVs were not resetting upon changing Pokémon in the setup menu.
  - Fixed faint-check bug where a player's fainted count was compared against the opponent's Pokémon count.
    - (This bug had not surfaced prior to switch logic testing)
- **Developer Notes**
  - Added following namespaces and associated methods:
    - AIMoveScoring
    - BasicScoring
    - AILogicSwitch.
  - Added BattleContext.cpp
  - Removed experimental AI stat range adjustments.
  - Removed strategy interfaces; moving forward AI behavior will be determined via a Difficulty enum class.
  - Moved the Type Chart array from BattleCalculations into its own header as inline constexpr.
  - pokemonMove struct has been moved out of the BattlePokemon class into its own struct.
  - Added BattleContext& parameters to a few BattleCalculations methods to clarify when battle state is being modified.
  - AI now uses its own:
    - Damage calculation method
    - Move type effectiveness calculation method
    - Pokemon type effectiveness calculation method
    - (This helps to differentiate between methods that mutate state as well as reduces cross-class dependencies)
- Miscellaneous #include cleanup.
   
### 2/20/2026
- Refactored how CalculateDamage works. Should have largely the same outcomes, just made the code more consistent and easier to follow.
- One slight change to note is I took from Bulbapedia's damage page for Gen 5 onwards in the "other" section and implemented that here (with rounding up on 0.5 results).
- Refactored and fixed up the AI update observed enemy stat ranges subroutines.
- Added cap of 15 to the badly poison counter.
   
### 2/16/2026
- Post Turn evaluation order is now random before each check (more accurate to gen 9 mechanics).
- Now correctly evaluates winner based on first player to lose all Pokémon.
- Voluntary switch order evaluation now random if both players take same turn to switch.
- Post-faint switching order still determined by fainted Pokémon's speed, but random when speed is exactly tied.
- Fixed an error that mistakingly took in a Pokémon's speed rather than speed stage on computing overall speed in determining post turn order.
- Added check in ExecuteTurn to check if defending Pokémon is fainted. This fixes edge case where the 2nd turn pokemon still tries to use move against the 1st turn pokemon that KO'd itself via recoil move. Checks both attacker and defender now.
- Moved the Switch prompt after all post turn effects have been evaluated instead of right after the last damaging one.
- Put in faint checks in disable status evaluation subroutine that way a fainted pokemon doesn't have their disabled move evaluated.
- Fixed error in recoil damage checks to make sure recoil damage is always at least 1.
- Fixed rounding errors in post turn damage evaluations so it will always do at least do 1 damage.
- Badly poisoned damage now rounds up damage (Max of either 1, or HP / 16 x counter + 15).
- Fixed rounding errors in leech seed health as well leech move routines (Leech and Dream Eater).
- Improved battle outcome text separation consistency in battles.
   
### 2/14/2026
- Added controller classes for Player and AI and got rid of AIPlayer class. Just more untangling of separating some parts of the UI.
- Some console output formatting fixes and beautifying
- Correctly outputs Pokemon's name when editing stats of a Pokémon
- Added exit states for during add or change Pokémon or moves. Can now cancel by entering 0.
- Added a delete move option
- Added swap and reposition options for both Pokemon and moves! Swap trades places while reposition moves selected to a target slot (think of it like drag and drop).
- Player Impact: Formatting for Pokemon's stats and moves is greatly improved. Better spacing and info. Can see what statuses your party Pokemon are currently when switching.
   
### 2/10/2026
- Some major UI refactoring. 
- UI fully separated from core game logic (MoveResultsUI & StatusEffectUI) for clearer messages
- Move routines now consistently display results through MoveResultsUI
- Fixed post-turn and status effect logic (thrash/rampage, leech, bound, disable, etc.)
- Corrected minor move text bugs (Sky Attack now shows correct start text)
- Improved overall message consistency between attacker and defender
- Player Impact: Battles now show more accurate, consistent messages; status effects and multi-turn moves behave as expected.

### 11/8/2025
- Changed MoveRoutines to use std::variant rather than using runtime polymorphism. Less verbose, and (very slightly) faster (AI only battles seemed snappier). I did also test with void* type erasure and even simple function pointer array lookups and it seemed std::variant was the snappiest feeling one.
- Added in some AI routines to observe and update its memory based on outcome of battle turns. The features have been implemented but they're not in use by the AI yet to determine what to do in battle.
- CalculateTypeEffectiveness is now only ever used in Move Routines, not in CalculateDamage(). It updates effectiveness in BattleContext.
- Added a default argument in CalculateDamage() "bool IsAI = false" so AI can use it to determine possible damage outcome with the random number being 92.
- Fixed bug in DetermineWhoGoesFirst() where it was getting the pokemon's current speed rather than its speed stage.
- Fixed CalculateHitChance() missing in the Seismic Toss Move Routine.
- ApplyDamage() now used in all Move Routines that do damage.
- If going against AI, the AI Pokémon's HP is seen as a percent now. Increments of 0.25% is known. This is to sort of mimic Sword/Shield's 400 pixel wide HP bar. You'll still see exact damage inflicted.
- Changed the if else block in DisplaySubstituteDamageTextDialog to be a bit more readable and straightforward.

### 8/6/2025
- Fixed bug with Reflect and Light Screen. They should work properly now. Forgot to change variable baseDamage to finalDamage in reflect and light screen conditions in CalculateDamage().
- Sky Attack now correctly has a higher crit ratio
- Pokemon's weight value is now in hectograms. This makes it so I can have the weight value as an int rather than double.
- CalculateLowKickPower() is updated to take the new values and cleaned up the method a bit.
- There is now a move power modifier in BattleContext set by a move routine (for now only Gust since it's the only gen 1 move that doubles in power if target is in semi-invulnerable Fly state). CalculateDamage() uses it to set currentMovePower and powerModifier is taken out of the main baseDamage equation.

### 8/4/2025
- Made damage calculations more in line with the official games which use fixed point math using 4096 scale, rather than floating point math
- Fixed type effectiveness bugs where it wouldn't display "doesn't affect" message and gives incorrect values
- Changed how Substitute damage message is outputted. Will only say either "sub took damage" or "sub faded".
- Fixed burn penalty also applying to special moves
- No more double types, no more needless static_casting int and static_casting double since all calculations are either floored through int truncation or rounded using half divisor addition.

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
This was coded in VS2026 Community, utilizing up to the C++23 standard features. Uploaded in this repository is a simple CMake build. Only tested on Windows.
</details>

<details open>
  <summary>
  
## HOW TO PLAY

  </summary>
  
- On the main menu screen you can choose up to 8 options to edit Player One or Player Two's Pokemon, controller type (human, or different AI difficulties), load or save party.
- Option 9 is for sim iteration amount, 10 to run simulations and 11 for normal battle.
  - Each option screen requires pressing the number then hitting enter.
- The game comes with default teams of versions of gen 1's protagonist Red's team, the rival Blue's team and Lance's Yellow version team. Choose Load Party (8) in the menu if you want to use them.- Editing a player's pokemon puts you in a submenu to choose options to add, change, release (delete) pokemon as well as edit their moves, stats, and placing in the party.
  - You can select between all gen 1's 151 Pokemon, as well as their moves. Names of the Pokemon, moves and their numbers can be used as input.
  - There aren't level restrictions on moves so a level 1 Bulbasaur can have Solar Beam if you so choose.
    - **NOTE:** Move names are hyphen and space sensitive, but not case sensitive.
- When teams are set, type 11 then enter in the main menu to start a battle! If either team has no pokemon or the pokemon have no moves, it will let you know and kick you back to the main menu.
- If you want to sim AI teams vs eachother (multiple battles without any output accept total wins and losses), you can do that as well!
  - 9 in main menu to set amount of battle iterations, and 10 to start the sims. It will kick you out if no Pokemon or moves on pokemon or if player controller types are human.
</details>

<details open>
  <summary>

## GENERAL INFO

   </summary>
This is a console based Pokemon battle simulator program. It starts up with a setup menu to customize your own team of 6 Pokemon. You can choose any of the 151 Pokemon and any moves they could learn based on their generation 1 (Red/Blue/Yellow) learnsets. You're able to set a nickname, level, 4 moves, stat IVs and EVs per Pokemon and send them into battle! The battle and move mechanics are more akin to (up to) generation 9 (Scarlet/Violet), so no weird bind move bugs or hyper beam not having a recharge phase when enemy Pokemon faints for example. Generation 2 special attack/defense stat split as well as generation 4 physical/special move split also is incorporated. <br/>
<br/>
I started this project a long time ago and have been working on it off and on while also learning C++. I wanted to put what I've learned to practice with Pokemon being one of my most cherished gaming series of all time. This is just something to put out there to show what I've learned so far. Along with future updates I hope to be more proficient at architectural/code organization design.
</details>

<details>
  <summary>

## SCREENSHOTS

   </summary>
   
<img src ="https://github.com/user-attachments/assets/710a2dcd-be05-449d-b06f-10c9db0fb95f" width=505 alt="Main Menu">

<img width="1057" alt="Editing Pokemon" src="https://github.com/user-attachments/assets/ea7d15ce-7fcb-4983-bae7-5be89dc16d9b">

<img width="1061" alt="Editing Moves 1" src="https://github.com/user-attachments/assets/9f4fd77d-67cc-41ed-9b2c-f10d9d869bb3" />

<img width="1059" alt="Editing Moves 2" src="https://github.com/user-attachments/assets/3e58c729-aa65-4d43-86aa-5f2d8f8520d3" />

<img width="911" alt="Battle 1" src="https://github.com/user-attachments/assets/0ac9ea40-f106-41d2-a1c9-c88a6a724485" />

<img width="906" alt="Battle 2" src="https://github.com/user-attachments/assets/d0819c75-5937-4dc4-84a8-8d628a0bfa71" />
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
- Easy and Medium A.I difficulties largely based on gen 4's Basic and Strong/Evaluate Attack scoring.
- There are however no natures, held items, or abilities. Whether I might add them in the future is up in the air.
- Bulk match AI battle simulations. Sim up to x amount of battles between 2 AI teams utilizing all CPU cores.
</details>

<details>
  <summary>
    
## PLANNED UPDATES

   </summary>
   
- More sophisticated A.I
- Code architectural/organizational choices
- Graphical user interface
- Not sure if I'll ever add in later generation stuff
</details>