#include "Database.h"

#include "Pokemon.h"
#include "StringToTypes.h"

void Database::LoadEmbedPokemon()
{
	pokedex.emplace_back(1, "Bulbasaur", 45, 49, 49, 65, 65, 45, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 69);

	pokedex.emplace_back(2, "Ivysaur", 60, 62, 63, 80, 80, 60, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 130);

	pokedex.emplace_back(3, "Venusaur", 80, 82, 83, 100, 100, 80, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 1000);

	pokedex.emplace_back(4, "Charmander", 39, 52, 43, 60, 50, 65, "Fire", PokemonType::Fire, "None", PokemonType::None, 85);

	pokedex.emplace_back(5, "Charmeleon", 58, 64, 58, 80, 65, 80, "Fire", PokemonType::Fire, "None", PokemonType::None, 190);

	pokedex.emplace_back(6, "Charizard", 78, 84, 78, 109, 85, 100, "Fire", PokemonType::Fire, "Flying", PokemonType::Flying, 905);

	pokedex.emplace_back(7, "Squirtle", 44, 48, 65, 50, 64, 43, "Water", PokemonType::Water, "None", PokemonType::None, 90);

	pokedex.emplace_back(8, "Wartortle", 59, 63, 80, 65, 80, 58, "Water", PokemonType::Water, "None", PokemonType::None, 225);

	pokedex.emplace_back(9, "Blastoise", 79, 83, 100, 85, 105, 78, "Water", PokemonType::Water, "None", PokemonType::None, 855);

	pokedex.emplace_back(10, "Caterpie", 45, 30, 35, 20, 20, 45, "Bug", PokemonType::Bug, "None", PokemonType::None, 29);

	pokedex.emplace_back(11, "Metapod", 50, 20, 55, 25, 25, 30, "Bug", PokemonType::Bug, "None", PokemonType::None, 99);

	pokedex.emplace_back(12, "Butterfree", 60, 45, 50, 90, 80, 70, "Bug", PokemonType::Bug, "Flying", PokemonType::Flying, 320);

	pokedex.emplace_back(13, "Weedle", 40, 35, 30, 20, 20, 50, "Bug", PokemonType::Bug, "Poison", PokemonType::Poison, 32);

	pokedex.emplace_back(14, "Kakuna", 45, 25, 50, 25, 25, 35, "Bug", PokemonType::Bug, "Poison", PokemonType::Poison, 100);

	pokedex.emplace_back(15, "Beedrill", 65, 90, 40, 45, 80, 75, "Bug", PokemonType::Bug, "Poison", PokemonType::Poison, 295);

	pokedex.emplace_back(16, "Pidgey", 40, 45, 40, 35, 35, 56, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 18);

	pokedex.emplace_back(17, "Pidgeotto", 63, 60, 55, 50, 50, 71, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 300);

	pokedex.emplace_back(18, "Pidgeot", 83, 80, 75, 70, 70, 101, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 395);

	pokedex.emplace_back(19, "Rattata", 30, 56, 35, 25, 35, 72, "Normal", PokemonType::Normal, "None", PokemonType::None, 35);

	pokedex.emplace_back(20, "Raticate", 55, 81, 60, 50, 70, 97, "Normal", PokemonType::Normal, "None", PokemonType::None, 185);

	pokedex.emplace_back(21, "Spearow", 40, 60, 30, 31, 31, 70, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 20);

	pokedex.emplace_back(22, "Fearow", 65, 90, 65, 61, 61, 100, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 380);

	pokedex.emplace_back(23, "Ekans", 35, 60, 44, 40, 54, 55, "Poison", PokemonType::Poison, "None", PokemonType::None, 69);

	pokedex.emplace_back(24, "Arbok", 60, 95, 69, 65, 79, 80, "Poison", PokemonType::Poison, "None", PokemonType::None, 650);

	pokedex.emplace_back(25, "Pikachu", 35, 55, 40, 50, 50, 90, "Electric", PokemonType::Electric, "None", PokemonType::None, 60);

	pokedex.emplace_back(26, "Raichu", 60, 90, 55, 90, 80, 110, "Electric", PokemonType::Electric, "None", PokemonType::None, 300);

	pokedex.emplace_back(27, "Sandshrew", 50, 75, 85, 20, 30, 40, "Ground", PokemonType::Ground, "None", PokemonType::None, 120);

	pokedex.emplace_back(28, "Sandslash", 75, 100, 110, 45, 55, 65, "Ground", PokemonType::Ground, "None", PokemonType::None, 295);

	pokedex.emplace_back(29, "Nidoran-F", 55, 47, 52, 40, 40, 41, "Poison", PokemonType::Poison, "None", PokemonType::None, 70);

	pokedex.emplace_back(30, "Nidorina", 70, 62, 67, 55, 55, 56, "Poison", PokemonType::Poison, "None", PokemonType::None, 200);

	pokedex.emplace_back(31, "Nidoqueen", 90, 92, 87, 75, 85, 76, "Poison", PokemonType::Poison, "Ground", PokemonType::Ground, 600);

	pokedex.emplace_back(32, "Nidoran-M", 46, 57, 40, 40, 40, 50, "Poison", PokemonType::Poison, "None", PokemonType::None, 90);

	pokedex.emplace_back(33, "Nidorino", 61, 72, 57, 55, 55, 65, "Poison", PokemonType::Poison, "None", PokemonType::None, 195);

	pokedex.emplace_back(34, "Nidoking", 81, 102, 77, 85, 75, 85, "Poison", PokemonType::Poison, "Ground", PokemonType::Ground, 620);

	pokedex.emplace_back(35, "Clefairy", 70, 45, 48, 60, 65, 35, "Normal", PokemonType::Normal, "None", PokemonType::None, 75);

	pokedex.emplace_back(36, "Clefable", 95, 70, 73, 95, 90, 60, "Normal", PokemonType::Normal, "None", PokemonType::None, 400);

	pokedex.emplace_back(37, "Vulpix", 38, 41, 40, 50, 65, 65, "Fire", PokemonType::Fire, "None", PokemonType::None, 99);

	pokedex.emplace_back(38, "Ninetales", 73, 76, 75, 81, 100, 100, "Fire", PokemonType::Fire, "None", PokemonType::None, 199);

	pokedex.emplace_back(39, "Jigglypuff", 115, 45, 20, 45, 25, 20, "Normal", PokemonType::Normal, "None", PokemonType::None, 55);

	pokedex.emplace_back(40, "Wigglytuff", 140, 70, 45, 85, 50, 45, "Normal", PokemonType::Normal, "None", PokemonType::None, 120);

	pokedex.emplace_back(41, "Zubat", 40, 45, 35, 30, 40, 55, "Poison", PokemonType::Poison, "Flying", PokemonType::Flying, 75);

	pokedex.emplace_back(42, "Golbat", 75, 80, 70, 65, 75, 90, "Poison", PokemonType::Poison, "Flying", PokemonType::Flying, 550);

	pokedex.emplace_back(43, "Oddish", 45, 50, 55, 75, 65, 30, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 54);

	pokedex.emplace_back(44, "Gloom", 60, 65, 70, 85, 75, 40, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 86);

	pokedex.emplace_back(45, "Vileplume", 75, 80, 85, 110, 90, 50, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 186);

	pokedex.emplace_back(46, "Paras", 35, 70, 55, 45, 55, 25, "Bug", PokemonType::Bug, "Grass", PokemonType::Grass, 54);

	pokedex.emplace_back(47, "Parasect", 60, 95, 80, 60, 80, 30, "Bug", PokemonType::Bug, "Grass", PokemonType::Grass, 295);

	pokedex.emplace_back(48, "Venonat", 60, 55, 50, 40, 55, 45, "Bug", PokemonType::Bug, "Poison", PokemonType::Poison, 300);

	pokedex.emplace_back(49, "Venomoth", 70, 65, 60, 90, 75, 90, "Bug", PokemonType::Bug, "Poison", PokemonType::Poison, 125);

	pokedex.emplace_back(50, "Diglett", 10, 55, 25, 35, 45, 95, "Ground", PokemonType::Ground, "None", PokemonType::None, 8);

	pokedex.emplace_back(51, "Dugtrio", 35, 100, 50, 50, 70, 120, "Ground", PokemonType::Ground, "None", PokemonType::None, 333);

	pokedex.emplace_back(52, "Meowth", 40, 45, 35, 40, 40, 90, "Normal", PokemonType::Normal, "None", PokemonType::None, 42);

	pokedex.emplace_back(53, "Persian", 65, 70, 60, 65, 65, 115, "Normal", PokemonType::Normal, "None", PokemonType::None, 320);

	pokedex.emplace_back(54, "Psyduck", 50, 52, 48, 65, 50, 55, "Water", PokemonType::Water, "None", PokemonType::None, 196);

	pokedex.emplace_back(55, "Golduck", 80, 82, 78, 95, 80, 85, "Water", PokemonType::Water, "None", PokemonType::None, 766);

	pokedex.emplace_back(56, "Mankey", 40, 80, 35, 35, 45, 70, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 280);

	pokedex.emplace_back(57, "Primeape", 65, 105, 60, 60, 70, 95, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 320);

	pokedex.emplace_back(58, "Growlithe", 55, 70, 45, 70, 50, 60, "Fire", PokemonType::Fire, "None", PokemonType::None, 190);

	pokedex.emplace_back(59, "Arcanine", 90, 110, 80, 100, 80, 95, "Fire", PokemonType::Fire, "None", PokemonType::None, 1550);

	pokedex.emplace_back(60, "Poliwag", 40, 50, 40, 40, 40, 90, "Water", PokemonType::Water, "None", PokemonType::None, 124);

	pokedex.emplace_back(61, "Poliwhirl", 65, 65, 65, 50, 50, 90, "Water", PokemonType::Water, "None", PokemonType::None, 200);

	pokedex.emplace_back(62, "Poliwrath", 90, 95, 95, 70, 90, 70, "Water", PokemonType::Water, "Fighting", PokemonType::Fighting, 540);

	pokedex.emplace_back(63, "Abra", 25, 20, 15, 105, 55, 90, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 195);

	pokedex.emplace_back(64, "Kadabra", 40, 35, 30, 120, 70, 105, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 565);

	pokedex.emplace_back(65, "Alakazam", 55, 50, 45, 135, 95, 120, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 480);

	pokedex.emplace_back(66, "Machop", 70, 80, 50, 35, 35, 35, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 195);

	pokedex.emplace_back(67, "Machoke", 80, 100, 70, 50, 60, 45, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 705);

	pokedex.emplace_back(68, "Machamp", 90, 130, 80, 65, 85, 55, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 1300);

	pokedex.emplace_back(69, "Bellsprout", 50, 75, 35, 70, 30, 40, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 40);

	pokedex.emplace_back(70, "Weepinbell", 65, 90, 50, 85, 45, 55, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 64);

	pokedex.emplace_back(71, "Victreebel", 80, 105, 65, 100, 70, 70, "Grass", PokemonType::Grass, "Poison", PokemonType::Poison, 155);

	pokedex.emplace_back(72, "Tentacool", 40, 40, 35, 50, 100, 70, "Water", PokemonType::Water, "Poison", PokemonType::Poison, 455);

	pokedex.emplace_back(73, "Tentacruel", 80, 70, 65, 80, 120, 100, "Water", PokemonType::Water, "Poison", PokemonType::Poison, 550);

	pokedex.emplace_back(74, "Geodude", 40, 80, 100, 30, 30, 20, "Rock", PokemonType::Rock, "Ground", PokemonType::Ground, 200);

	pokedex.emplace_back(75, "Graveler", 55, 95, 115, 45, 45, 35, "Rock", PokemonType::Rock, "Ground", PokemonType::Ground, 1050);

	pokedex.emplace_back(76, "Golem", 80, 120, 130, 55, 65, 45, "Rock", PokemonType::Rock, "Ground", PokemonType::Ground, 3000);

	pokedex.emplace_back(77, "Ponyta", 50, 85, 55, 65, 65, 90, "Fire", PokemonType::Fire, "None", PokemonType::None, 300);

	pokedex.emplace_back(78, "Rapidash", 65, 100, 70, 80, 80, 105, "Fire", PokemonType::Fire, "None", PokemonType::None, 950);

	pokedex.emplace_back(79, "Slowpoke", 90, 65, 65, 40, 40, 15, "Water", PokemonType::Water, "Psychic", PokemonType::Psychic, 360);

	pokedex.emplace_back(80, "Slowbro", 95, 75, 110, 100, 80, 30, "Water", PokemonType::Water, "Psychic", PokemonType::Psychic, 785);

	pokedex.emplace_back(81, "Magnemite", 25, 35, 70, 95, 55, 45, "Electric", PokemonType::Electric, "Steel", PokemonType::Steel, 60);

	pokedex.emplace_back(82, "Magneton", 50, 60, 95, 120, 70, 70, "Electric", PokemonType::Electric, "Steel", PokemonType::Steel, 600);

	pokedex.emplace_back(83, "Farfetch'd", 52, 90, 55, 58, 62, 60, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 150);

	pokedex.emplace_back(84, "Doduo", 35, 85, 45, 35, 35, 75, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 392);

	pokedex.emplace_back(85, "Dodrio", 60, 110, 70, 60, 60, 110, "Normal", PokemonType::Normal, "Flying", PokemonType::Flying, 852);

	pokedex.emplace_back(86, "Seel", 65, 45, 55, 45, 70, 45, "Water", PokemonType::Water, "None", PokemonType::None, 900);

	pokedex.emplace_back(87, "Dewgong", 90, 70, 80, 70, 95, 70, "Water", PokemonType::Water, "Ice", PokemonType::Ice, 1200);

	pokedex.emplace_back(88, "Grimer", 80, 80, 50, 40, 50, 25, "Poison", PokemonType::Poison, "None", PokemonType::None, 300);

	pokedex.emplace_back(89, "Muk", 105, 105, 75, 65, 100, 50, "Poison", PokemonType::Poison, "None", PokemonType::None, 300);

	pokedex.emplace_back(90, "Shellder", 30, 65, 100, 45, 25, 40, "Water", PokemonType::Water, "None", PokemonType::None, 40);

	pokedex.emplace_back(91, "Cloyster", 50, 95, 180, 85, 45, 70, "Water", PokemonType::Water, "Ice", PokemonType::Ice, 1325);

	pokedex.emplace_back(92, "Gastly", 30, 35, 30, 100, 35, 80, "Ghost", PokemonType::Ghost, "Poison", PokemonType::Poison, 1);

	pokedex.emplace_back(93, "Haunter", 45, 50, 45, 115, 55, 95, "Ghost", PokemonType::Ghost, "Poison", PokemonType::Poison, 1);

	pokedex.emplace_back(94, "Gengar", 60, 65, 60, 130, 75, 110, "Ghost", PokemonType::Ghost, "Poison", PokemonType::Poison, 405);

	pokedex.emplace_back(95, "Onix", 35, 45, 160, 30, 45, 70, "Rock", PokemonType::Rock, "Ground", PokemonType::Ground, 2100);

	pokedex.emplace_back(96, "Drowzee", 60, 48, 45, 43, 90, 42, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 324);

	pokedex.emplace_back(97, "Hypno", 85, 73, 70, 73, 115, 67, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 756);

	pokedex.emplace_back(98, "Krabby", 30, 105, 90, 25, 25, 50, "Water", PokemonType::Water, "None", PokemonType::None, 65);

	pokedex.emplace_back(99, "Kingler", 55, 130, 115, 50, 50, 75, "Water", PokemonType::Water, "None", PokemonType::None, 600);

	pokedex.emplace_back(100, "Voltorb", 40, 30, 50, 55, 55, 100, "Electric", PokemonType::Electric, "None", PokemonType::None, 104);

	pokedex.emplace_back(101, "Electrode", 60, 50, 70, 80, 80, 150, "Electric", PokemonType::Electric, "None", PokemonType::None, 666);

	pokedex.emplace_back(102, "Exeggcute", 60, 40, 80, 60, 45, 40, "Grass", PokemonType::Grass, "Psychic", PokemonType::Psychic, 25);

	pokedex.emplace_back(103, "Exeggutor", 95, 95, 85, 125, 75, 55, "Grass", PokemonType::Grass, "Psychic", PokemonType::Psychic, 1200);

	pokedex.emplace_back(104, "Cubone", 50, 50, 95, 40, 50, 35, "Ground", PokemonType::Ground, "None", PokemonType::None, 65);

	pokedex.emplace_back(105, "Marowak", 60, 80, 110, 50, 80, 45, "Ground", PokemonType::Ground, "None", PokemonType::None, 450);

	pokedex.emplace_back(106, "Hitmonlee", 50, 120, 53, 35, 110, 87, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 498);

	pokedex.emplace_back(107, "Hitmonchan", 50, 105, 79, 35, 110, 76, "Fighting", PokemonType::Fighting, "None", PokemonType::None, 502);

	pokedex.emplace_back(108, "Lickitung", 90, 55, 75, 60, 75, 30, "Normal", PokemonType::Normal, "None", PokemonType::None, 655);

	pokedex.emplace_back(109, "Koffing", 40, 65, 95, 60, 45, 35, "Poison", PokemonType::Poison, "None", PokemonType::None, 10);

	pokedex.emplace_back(110, "Weezing", 65, 90, 120, 85, 70, 60, "Poison", PokemonType::Poison, "None", PokemonType::None, 95);

	pokedex.emplace_back(111, "Rhyhorn", 80, 85, 95, 30, 30, 25, "Ground", PokemonType::Ground, "Rock", PokemonType::Rock, 1150);

	pokedex.emplace_back(112, "Rhydon", 105, 130, 120, 45, 45, 40, "Ground", PokemonType::Ground, "Rock", PokemonType::Rock, 1200);

	pokedex.emplace_back(113, "Chansey", 250, 5, 5, 35, 105, 50, "Normal", PokemonType::Normal, "None", PokemonType::None, 346);

	pokedex.emplace_back(114, "Tangela", 65, 55, 115, 100, 40, 60, "Grass", PokemonType::Grass, "None", PokemonType::None, 350);

	pokedex.emplace_back(115, "Kangaskhan", 105, 95, 80, 40, 80, 90, "Normal", PokemonType::Normal, "None", PokemonType::None, 800);

	pokedex.emplace_back(116, "Horsea", 30, 40, 70, 70, 25, 60, "Water", PokemonType::Water, "None", PokemonType::None, 80);

	pokedex.emplace_back(117, "Seadra", 55, 65, 95, 95, 45, 85, "Water", PokemonType::Water, "None", PokemonType::None, 250);

	pokedex.emplace_back(118, "Goldeen", 45, 67, 60, 35, 50, 63, "Water", PokemonType::Water, "None", PokemonType::None, 150);

	pokedex.emplace_back(119, "Seaking", 80, 92, 65, 65, 80, 68, "Water", PokemonType::Water, "None", PokemonType::None, 390);

	pokedex.emplace_back(120, "Staryu", 30, 45, 55, 70, 55, 85, "Water", PokemonType::Water, "None", PokemonType::None, 345);

	pokedex.emplace_back(121, "Starmie", 60, 75, 85, 100, 85, 115, "Water", PokemonType::Water, "Psychic", PokemonType::Psychic, 800);

	pokedex.emplace_back(122, "Mr. Mime", 40, 45, 65, 100, 120, 90, "Psychic", PokemonType::Psychic, "Fairy", PokemonType::Fairy, 545);

	pokedex.emplace_back(123, "Scyther", 70, 110, 80, 55, 80, 105, "Bug", PokemonType::Bug, "Flying", PokemonType::Flying, 560);

	pokedex.emplace_back(124, "Jynx", 65, 50, 35, 115, 95, 95, "Ice", PokemonType::Ice, "Psychic", PokemonType::Psychic, 406);

	pokedex.emplace_back(125, "Electabuzz", 65, 83, 57, 95, 85, 105, "Electric", PokemonType::Electric, "None", PokemonType::None, 300);

	pokedex.emplace_back(126, "Magmar", 65, 95, 57, 100, 85, 93, "Fire", PokemonType::Fire, "None", PokemonType::None, 445);

	pokedex.emplace_back(127, "Pinsir", 65, 125, 100, 55, 70, 85, "Bug", PokemonType::Bug, "None", PokemonType::None, 550);

	pokedex.emplace_back(128, "Tauros", 75, 100, 95, 40, 70, 110, "Normal", PokemonType::Normal, "None", PokemonType::None, 884);

	pokedex.emplace_back(129, "Magikarp", 20, 10, 55, 15, 20, 80, "Water", PokemonType::Water, "None", PokemonType::None, 100);

	pokedex.emplace_back(130, "Gyarados", 95, 125, 79, 60, 100, 81, "Water", PokemonType::Water, "Flying", PokemonType::Flying, 2350);

	pokedex.emplace_back(131, "Lapras", 130, 85, 80, 85, 95, 60, "Water", PokemonType::Water, "Ice", PokemonType::Ice, 2200);

	pokedex.emplace_back(132, "Ditto", 48, 48, 48, 48, 48, 48, "Normal", PokemonType::Normal, "None", PokemonType::None, 40);

	pokedex.emplace_back(133, "Eevee", 55, 55, 50, 45, 65, 55, "Normal", PokemonType::Normal, "None", PokemonType::None, 65);

	pokedex.emplace_back(134, "Vaporeon", 130, 65, 60, 110, 95, 65, "Water", PokemonType::Water, "None", PokemonType::None, 290);

	pokedex.emplace_back(135, "Jolteon", 65, 65, 60, 110, 95, 130, "Electric", PokemonType::Electric, "None", PokemonType::None, 245);

	pokedex.emplace_back(136, "Flareon", 65, 130, 60, 95, 110, 65, "Fire", PokemonType::Fire, "None", PokemonType::None, 250);

	pokedex.emplace_back(137, "Porygon", 65, 60, 70, 85, 75, 40, "Normal", PokemonType::Normal, "None", PokemonType::None, 365);

	pokedex.emplace_back(138, "Omanyte", 35, 40, 100, 90, 55, 35, "Rock", PokemonType::Rock, "Water", PokemonType::Water, 75);

	pokedex.emplace_back(139, "Omastar", 70, 60, 125, 115, 70, 55, "Rock", PokemonType::Rock, "Water", PokemonType::Water, 350);

	pokedex.emplace_back(140, "Kabuto", 30, 80, 90, 55, 45, 55, "Rock", PokemonType::Rock, "Water", PokemonType::Water, 115);

	pokedex.emplace_back(141, "Kabutops", 60, 115, 105, 65, 70, 80, "Rock", PokemonType::Rock, "Water", PokemonType::Water, 405);

	pokedex.emplace_back(142, "Aerodactyl", 80, 105, 65, 60, 75, 130, "Rock", PokemonType::Rock, "Flying", PokemonType::Flying, 590);

	pokedex.emplace_back(143, "Snorlax", 160, 110, 65, 65, 110, 30, "Normal", PokemonType::Normal, "None", PokemonType::None, 4600);

	pokedex.emplace_back(144, "Articuno", 90, 85, 100, 95, 125, 85, "Ice", PokemonType::Ice, "Flying", PokemonType::Flying, 554);

	pokedex.emplace_back(145, "Zapdos", 90, 90, 85, 125, 90, 100, "Electric", PokemonType::Electric, "Flying", PokemonType::Flying, 526);

	pokedex.emplace_back(146, "Moltres", 90, 100, 90, 125, 85, 90, "Fire", PokemonType::Fire, "Flying", PokemonType::Flying, 600);

	pokedex.emplace_back(147, "Dratini", 41, 64, 45, 50, 50, 50, "Dragon", PokemonType::Dragon, "None", PokemonType::None, 33);

	pokedex.emplace_back(148, "Dragonair", 61, 84, 65, 70, 70, 70, "Dragon", PokemonType::Dragon, "None", PokemonType::None, 165);

	pokedex.emplace_back(149, "Dragonite", 91, 134, 95, 100, 100, 80, "Dragon", PokemonType::Dragon, "Flying", PokemonType::Flying, 2100);

	pokedex.emplace_back(150, "Mewtwo", 106, 110, 90, 154, 90, 130, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 1220);

	pokedex.emplace_back(151, "Mew", 100, 100, 100, 100, 100, 100, "Psychic", PokemonType::Psychic, "None", PokemonType::None, 40);
}