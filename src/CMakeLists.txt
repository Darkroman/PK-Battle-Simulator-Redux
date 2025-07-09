# All source files grouped by folder

set(SOURCES
    main.cpp
    Game.h

    # data
    data/Database.cpp
    data/LoadEmbeddedLearnset.cpp
    data/LoadEmbeddedMoves.cpp
    data/LoadEmbeddedPokemon.cpp
    data/Move.cpp
    data/Pokemon.cpp
    data/StringToTypes.cpp

    # entities
    entities/AIPlayer.cpp
    entities/BattlePokemon.cpp
    entities/HumanPlayer.cpp
    entities/Player.cpp

    # ai strategies
    "entities/ai strategies/EasyAIStrategy.cpp"

    # battle
    battle/BattleCalculations.cpp
    battle/BattleStatusManager.cpp
    battle/MoveExecutor.cpp
    battle/PostTurnEffectProcessor.cpp
    battle/RandomEngine.cpp
    battle/TurnManager.cpp
    battle/TurnProcessor.cpp
    battle/TurnUtils.cpp
    battle/WinChecker.cpp

    # moves
    moves/MoveEffects.cpp

    # UI
    ui/BattleTextMenu.cpp
    ui/Menu.cpp
    ui/MoveResultsText.cpp
    ui/StatusEffectText.cpp

    # save
    save/LoadParty.cpp
    save/SaveParty.cpp
)

# Header include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/entities
    "${CMAKE_CURRENT_SOURCE_DIR}/entities/ai strategies"
    ${CMAKE_CURRENT_SOURCE_DIR}/battle
    ${CMAKE_CURRENT_SOURCE_DIR}/moves
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/save
)

add_executable(PokemonBattleSimulator ${SOURCES})


add_custom_command(TARGET PokemonBattleSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/PlayerOneSave.txt
        ${CMAKE_CURRENT_BINARY_DIR}/PlayerOneSave.txt
)

add_custom_command(TARGET PokemonBattleSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/PlayerTwoSave.txt
        ${CMAKE_CURRENT_BINARY_DIR}/PlayerTwoSave.txt
)
