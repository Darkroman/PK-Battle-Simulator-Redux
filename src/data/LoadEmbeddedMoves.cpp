#include "Database.h"

#include "Move.h"
#include "../moves/MoveEffectEnums.h"
#include "StringToTypes.h"

void Database::LoadEmbedMoves()
{
	movedex.emplace_back(1, "Pound", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 35, 56, 40, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(2, "Karate Chop", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::IncreasedCritical, 0, 25, 40, 50, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(3, "Double Slap", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 10, 16, 15, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(4, "Comet Punch", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 15, 24, 18, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(5, "Mega Punch", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 20, 32, 80, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(6, "Pay Day", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 20, 32, 40, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(7, "Fire Punch", "Fire", PokemonType::Fire, "Physical", Category::Physical, 0, MoveEffect::BurnHit, 10, 15, 24, 75, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(8, "Ice Punch", "Ice", PokemonType::Ice, "Physical", Category::Physical, 0, MoveEffect::FreezeHit, 10, 15, 24, 75, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(9, "Thunder Punch", "Electric", PokemonType::Electric, "Physical", Category::Physical, 0, MoveEffect::ParalyzeHit, 10, 15, 24, 75, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(10, "Scratch", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 35, 56, 40, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(11, "Vise Grip", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 30, 48, 55, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(12, "Guillotine", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::OHKO, 0, 5, 8, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(13, "Razor Wind", "Normal", PokemonType::Normal, "Special", Category::Special, 0, MoveEffect::RazorWind, 0, 10, 16, 80, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(14, "Swords Dance", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::AttackUp2, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(15, "Cut", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 30, 48, 50, 95, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(16, "Gust", "Flying", PokemonType::Flying, "Special", Category::Special, 0, MoveEffect::Gust, 0, 35, 56, 40, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(17, "Wing Attack", "Flying", PokemonType::Flying, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 35, 56, 60, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(18, "Whirlwind", "Normal", PokemonType::Normal, "Status", Category::Status, -6, MoveEffect::MakeEnemySwitch, 0, 20, 32, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1);

	movedex.emplace_back(19, "Fly", "Flying", PokemonType::Flying, "Physical", Category::Physical, 0, MoveEffect::Fly, 0, 15, 24, 90, 95, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(20, "Bind", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Bound, 0, 20, 32, 15, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(21, "Slam", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 20, 32, 80, 75, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(22, "Vine Whip", "Grass", PokemonType::Grass, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 25, 40, 45, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(23, "Stomp", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Stomp, 30, 20, 32, 65, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(24, "Double Kick", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::DoubleHit, 0, 30, 48, 30, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(25, "Mega Kick", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 5, 8, 120, 75, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(26, "Jump Kick", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::JumpKick, 0, 10, 16, 100, 95, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(27, "Rolling Kick", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 30, 15, 24, 60, 85, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(28, "Sand Attack", "Ground", PokemonType::Ground, "Status", Category::Status, 0, MoveEffect::AccuracyDown, 0, 15, 24, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(29, "Headbutt", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 30, 15, 24, 70, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(30, "Horn Attack", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 25, 40, 65, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(31, "Fury Attack", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 20, 32, 15, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(32, "Horn Drill", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::OHKO, 0, 5, 8, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(33, "Tackle", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 35, 56, 40, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(34, "Body Slam", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::BodySlam, 30, 15, 24, 85, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(35, "Wrap", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Bound, 0, 20, 32, 15, 90, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(36, "Take Down", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::RecoilQuarter, 0, 20, 32, 90, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(37, "Thrash", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Thrash, 0, 10, 16, 120, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(38, "Double-Edge", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::RecoilThird, 0, 15, 24, 120, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(39, "Tail Whip", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::DefenseDown, 0, 30, 48, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(40, "Poison Sting", "Poison", PokemonType::Poison, "Physical", Category::Physical, 0, MoveEffect::PoisonHit, 30, 35, 56, 15, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(41, "Twineedle", "Bug", PokemonType::Bug, "Physical", Category::Physical, 0, MoveEffect::Twineedle, 20, 20, 32, 25, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(42, "Pin Missile", "Bug", PokemonType::Bug, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 20, 32, 25, 95, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(43, "Leer", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::DefenseDown, 0, 30, 48, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(44, "Bite", "Dark", PokemonType::Dark, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 0, 25, 40, 60, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(45, "Growl", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::AttackDown, 0, 40, 64, 0, 100, 0, 1, 1, 0, 1, 0, 1, 1);

	movedex.emplace_back(46, "Roar", "Normal", PokemonType::Normal, "Status", Category::Status, -6, MoveEffect::MakeEnemySwitch, 0, 20, 32, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1);

	movedex.emplace_back(47, "Sing", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::BypassSubSleep, 0, 15, 24, 0, 55, 0, 1, 1, 0, 1, 0, 1, 1);

	movedex.emplace_back(48, "Supersonic", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Confuse, 0, 20, 32, 0, 55, 0, 1, 1, 0, 1, 0, 1, 1);

	movedex.emplace_back(49, "Sonic Boom", "Normal", PokemonType::Normal, "Special", Category::Special, 0, MoveEffect::SonicBoom, 0, 20, 32, 0, 90, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(50, "Disable", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Disable, 0, 20, 32, 0, 100, 0, 1, 1, 0, 1, 0, 0, 1);

	movedex.emplace_back(51, "Acid", "Poison", PokemonType::Poison, "Special", Category::Special, 0, MoveEffect::SpecialDefenseDownHit, 10, 30, 48, 40, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(52, "Ember", "Fire", PokemonType::Fire, "Special", Category::Special, 0, MoveEffect::BurnHit, 10, 25, 40, 40, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(53, "Flamethrower", "Fire", PokemonType::Fire, "Special", Category::Special, 0, MoveEffect::BurnHit, 10, 15, 24, 90, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(54, "Mist", "Ice", PokemonType::Ice, "Status", Category::Status, 0, MoveEffect::Mist, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(55, "Water Gun", "Water", PokemonType::Water, "Special", Category::Special, 0, MoveEffect::NormalHit, 0, 25, 40, 40, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(56, "Hydro Pump", "Water", PokemonType::Water, "Special", Category::Special, 0, MoveEffect::NormalHit, 0, 5, 8, 110, 80, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(57, "Surf", "Water", PokemonType::Water, "Special", Category::Special, 0, MoveEffect::NormalHit, 0, 15, 24, 90, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(58, "Ice Beam", "Ice", PokemonType::Ice, "Special", Category::Special, 0, MoveEffect::FreezeHit, 10, 10, 16, 90, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(59, "Blizzard", "Ice", PokemonType::Ice, "Special", Category::Special, 0, MoveEffect::FreezeHit, 10, 5, 8, 110, 70, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(60, "Psybeam", "Psychic", PokemonType::Psychic, "Special", Category::Special, 0, MoveEffect::ConfuseHit, 10, 20, 32, 65, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(61, "Bubble Beam", "Water", PokemonType::Water, "Special", Category::Special, 0, MoveEffect::SpeedDownHit, 10, 20, 32, 65, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(62, "Aurora Beam", "Ice", PokemonType::Ice, "Special", Category::Special, 0, MoveEffect::AttackDownHit, 10, 20, 32, 65, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(63, "Hyper Beam", "Normal", PokemonType::Normal, "Special", Category::Special, 0, MoveEffect::RechargeAttack, 0, 5, 8, 150, 90, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(64, "Peck", "Flying", PokemonType::Flying, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 35, 56, 35, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(65, "Drill Peck", "Flying", PokemonType::Flying, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 20, 32, 80, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(66, "Submission", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::RecoilQuarter, 0, 20, 32, 80, 80, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(67, "Low Kick", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::LowKick, 0, 20, 32, 0, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(68, "Counter", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, -5, MoveEffect::Counter, 0, 20, 32, 0, 100, 1, 1, 0, 0, 0, 1, 0, 0);

	movedex.emplace_back(69, "Seismic Toss", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::SeismicToss, 0, 20, 32, 0, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(70, "Strength", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 15, 24, 80, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(71, "Absorb", "Grass", PokemonType::Grass, "Special", Category::Special, 0, MoveEffect::Leech, 0, 25, 40, 20, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(72, "Mega Drain", "Grass", PokemonType::Grass, "Special", Category::Special, 0, MoveEffect::Leech, 0, 15, 24, 40, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(73, "Leech Seed", "Grass", PokemonType::Grass, "Special", Category::Special, 0, MoveEffect::LeechSeed, 0, 10, 16, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(74, "Growth", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Growth, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(75, "Razor Leaf", "Grass", PokemonType::Grass, "Physical", Category::Physical, 0, MoveEffect::IncreasedCritical, 0, 25, 40, 55, 95, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(76, "Solar Beam", "Grass", PokemonType::Grass, "Special", Category::Special, 0, MoveEffect::SolarBeam, 0, 10, 16, 120, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(77, "Poison Powder", "Poison", PokemonType::Poison, "Status", Category::Status, 0, MoveEffect::PoisonPowder, 0, 35, 56, 0, 75, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(78, "Stun Spore", "Grass", PokemonType::Grass, "Status", Category::Status, 0, MoveEffect::StunSpore, 0, 30, 48, 0, 75, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(79, "Sleep Powder", "Grass", PokemonType::Grass, "Status", Category::Status, 0, MoveEffect::SleepPowder, 0, 15, 24, 0, 75, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(80, "Petal Dance", "Grass", PokemonType::Grass, "Special", Category::Special, 0, MoveEffect::Thrash, 0, 10, 16, 120, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(81, "String Shot", "Bug", PokemonType::Bug, "Status", Category::Status, 0, MoveEffect::SpeedDown2, 0, 40, 64, 0, 95, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(82, "Dragon Rage", "Dragon", PokemonType::Dragon, "Special", Category::Special, 0, MoveEffect::DragonRage, 0, 10, 16, 0, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(83, "Fire Spin", "Fire", PokemonType::Fire, "Special", Category::Special, 0, MoveEffect::Bound, 0, 15, 24, 35, 85, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(84, "Thunder Shock", "Electric", PokemonType::Electric, "Special", Category::Special, 0, MoveEffect::ParalyzeHit, 10, 30, 48, 40, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(85, "Thunderbolt", "Electric", PokemonType::Electric, "Special", Category::Special, 0, MoveEffect::ParalyzeHit, 10, 15, 24, 90, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(86, "Thunder Wave", "Electric", PokemonType::Electric, "Status", Category::Status, 0, MoveEffect::Paralyze, 0, 20, 32, 0, 90, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(87, "Thunder", "Electric", PokemonType::Electric, "Special", Category::Special, 0, MoveEffect::ParalyzeHit, 30, 10, 16, 110, 70, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(88, "Rock Throw", "Rock", PokemonType::Rock, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 15, 24, 50, 90, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(89, "Earthquake", "Ground", PokemonType::Ground, "Physical", Category::Physical, 0, MoveEffect::Earthquake, 0, 10, 16, 100, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(90, "Fissure", "Ground", PokemonType::Ground, "Physical", Category::Physical, 0, MoveEffect::OHKO, 0, 5, 8, 0, 30, 0, 1, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(91, "Dig", "Ground", PokemonType::Ground, "Physical", Category::Physical, 0, MoveEffect::Dig, 0, 10, 16, 80, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(92, "Toxic", "Poison", PokemonType::Poison, "Status", Category::Status, 0, MoveEffect::Toxic, 0, 10, 16, 0, 90, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(93, "Confusion", "Psychic", PokemonType::Psychic, "Special", Category::Special, 0, MoveEffect::ConfuseHit, 10, 25, 40, 50, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(94, "Psychic", "Psychic", PokemonType::Psychic, "Special", Category::Special, 0, MoveEffect::SpecialDefenseDownHit, 10, 10, 16, 90, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(95, "Hypnosis", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::SleepMove, 0, 20, 32, 0, 60, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(96, "Meditate", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::AttackUp, 0, 40, 64, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(97, "Agility", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::SpeedUp2, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(98, "Quick Attack", "Normal", PokemonType::Normal, "Physical", Category::Physical, 1, MoveEffect::NormalHit, 0, 30, 48, 40, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(99, "Rage", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Rage, 0, 20, 32, 20, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(100, "Teleport", "Psychic", PokemonType::Psychic, "Status", Category::Status, -6, MoveEffect::Teleport, 0, 20, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(101, "Night Shade", "Ghost", PokemonType::Ghost, "Special", Category::Special, 0, MoveEffect::NightShade, 0, 15, 24, 0, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(102, "Mimic", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Mimic, 0, 10, 16, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1);

	movedex.emplace_back(103, "Screech", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::DefenseDown2, 0, 40, 64, 0, 85, 0, 1, 1, 0, 1, 0, 1, 1);

	movedex.emplace_back(104, "Double Team", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::EvasionUp, 0, 15, 24, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(105, "Recover", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::HealHalfHP, 0, 5, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(106, "Harden", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::DefenseUp, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(107, "Minimize", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Minimize, 0, 10, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(108, "Smokescreen", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::AccuracyDown, 0, 20, 32, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(109, "Confuse Ray", "Ghost", PokemonType::Ghost, "Status", Category::Status, 0, MoveEffect::Confuse, 0, 10, 16, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(110, "Withdraw", "Water", PokemonType::Water, "Status", Category::Status, 0, MoveEffect::DefenseUp, 0, 40, 64, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(111, "Defense Curl", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::DefenseUp, 0, 40, 64, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(112, "Barrier", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::DefenseUp2, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(113, "Light Screen", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::LightScreen, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(114, "Haze", "Ice", PokemonType::Ice, "Status", Category::Status, 0, MoveEffect::Haze, 0, 30, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

	movedex.emplace_back(115, "Reflect", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::Reflect, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(116, "Focus Energy", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::FocusEnergy, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(117, "Bide", "Normal", PokemonType::Normal, "Physical", Category::Physical, 1, MoveEffect::Bide, 0, 10, 16, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0);

	movedex.emplace_back(118, "Metronome", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Metronome, 0, 10, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(119, "Mirror Move", "Flying", PokemonType::Flying, "Status", Category::Status, 0, MoveEffect::MirrorMove, 0, 10, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(120, "Self-Destruct", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Explosion, 0, 5, 8, 200, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(121, "Egg Bomb", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::NormalHit, 0, 10, 16, 100, 75, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(122, "Lick", "Ghost", PokemonType::Ghost, "Physical", Category::Physical, 0, MoveEffect::ParalyzeHit, 30, 30, 48, 30, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(123, "Smog", "Poison", PokemonType::Poison, "Special", Category::Special, 0, MoveEffect::PoisonHit, 40, 20, 32, 30, 70, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(124, "Sludge", "Poison", PokemonType::Poison, "Special", Category::Special, 0, MoveEffect::PoisonHit, 30, 20, 32, 65, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(125, "Bone Club", "Ground", PokemonType::Ground, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 10, 20, 32, 65, 85, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(126, "Fire Blast", "Fire", PokemonType::Fire, "Special", Category::Special, 0, MoveEffect::BurnHit, 10, 5, 8, 110, 85, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(127, "Waterfall", "Water", PokemonType::Water, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 20, 15, 24, 80, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(128, "Clamp", "Water", PokemonType::Water, "Physical", Category::Physical, 0, MoveEffect::Bound, 0, 15, 24, 35, 85, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(129, "Swift", "Normal", PokemonType::Normal, "Special", Category::Special, 0, MoveEffect::Swift, 0, 20, 32, 60, 0, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(130, "Skull Bash", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::SkullBash, 0, 10, 16, 130, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(131, "Spike Cannon", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 15, 24, 20, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(132, "Constrict", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::SpeedDownHit, 10, 35, 56, 10, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(133, "Amnesia", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::SpecialDefenseUp2, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(134, "Kinesis", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::AccuracyDown, 0, 15, 24, 0, 80, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(135, "Soft-Boiled", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::HealHalfHP, 0, 5, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(136, "High Jump Kick", "Fighting", PokemonType::Fighting, "Physical", Category::Physical, 0, MoveEffect::JumpKick, 0, 10, 16, 130, 90, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(137, "Glare", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Paralyze, 0, 30, 48, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(138, "Dream Eater", "Psychic", PokemonType::Psychic, "Special", Category::Special, 0, MoveEffect::DreamEater, 0, 15, 24, 100, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(139, "Poison Gas", "Poison", PokemonType::Poison, "Status", Category::Status, 0, MoveEffect::PoisonGas, 0, 40, 64, 0, 90, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(140, "Barrage", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 20, 32, 15, 85, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(141, "Leech Life", "Bug", PokemonType::Bug, "Physical", Category::Physical, 0, MoveEffect::Leech, 0, 10, 16, 80, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(142, "Lovely Kiss", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::SleepMove, 0, 10, 16, 0, 75, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(143, "Sky Attack", "Flying", PokemonType::Flying, "Physical", Category::Physical, 0, MoveEffect::SkyAttack, 0, 5, 8, 140, 90, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(144, "Transform", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Transform, 0, 10, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(145, "Bubble", "Water", PokemonType::Water, "Special", Category::Special, 0, MoveEffect::SpeedDownHit, 10, 30, 48, 40, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(146, "Dizzy Punch", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::ConfuseHit, 20, 10, 16, 70, 100, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(147, "Spore", "Grass", PokemonType::Grass, "Status", Category::Status, 0, MoveEffect::SleepPowder, 0, 15, 24, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(148, "Flash", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::AccuracyDown, 0, 20, 32, 0, 100, 0, 1, 1, 0, 1, 0, 0, 0);

	movedex.emplace_back(149, "Psywave", "Psychic", PokemonType::Psychic, "Special", Category::Special, 0, MoveEffect::Psywave, 0, 15, 24, 0, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(150, "Splash", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Splash, 0, 40, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	movedex.emplace_back(151, "Acid Armor", "Poison", PokemonType::Poison, "Status", Category::Status, 0, MoveEffect::DefenseUp2, 0, 20, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(152, "Crabhammer", "Water", PokemonType::Water, "Physical", Category::Physical, 0, MoveEffect::IncreasedCritical, 0, 10, 16, 100, 90, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(153, "Explosion", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Explosion, 0, 5, 8, 250, 100, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(154, "Fury Swipes", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::MultiAttack, 0, 15, 24, 18, 80, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(155, "Bonemerang", "Ground", PokemonType::Ground, "Physical", Category::Physical, 0, MoveEffect::DoubleHit, 0, 10, 16, 50, 90, 0, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(156, "Rest", "Psychic", PokemonType::Psychic, "Status", Category::Status, 0, MoveEffect::Rest, 0, 5, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(157, "Rock Slide", "Rock", PokemonType::Rock, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 30, 10, 16, 75, 90, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(158, "Hyper Fang", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::FlinchHit, 10, 15, 24, 80, 90, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(159, "Sharpen", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::AttackUp, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(160, "Conversion", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Conversion, 0, 30, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(161, "Tri Attack", "Normal", PokemonType::Normal, "Special", Category::Special, 0, MoveEffect::TriAttack, 20, 10, 16, 80, 100, 0, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(162, "Super Fang", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::SuperFang, 0, 10, 16, 0, 90, 1, 1, 0, 0, 1, 0, 0, 0);

	movedex.emplace_back(163, "Slash", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::IncreasedCritical, 0, 20, 32, 70, 100, 1, 1, 0, 0, 1, 1, 0, 0);

	movedex.emplace_back(164, "Substitute", "Normal", PokemonType::Normal, "Status", Category::Status, 0, MoveEffect::Substitute, 0, 10, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0);

	movedex.emplace_back(165, "Struggle", "Normal", PokemonType::Normal, "Physical", Category::Physical, 0, MoveEffect::Struggle, 0, 1, 1, 50, 0, 1, 1, 0, 0, 0, 1, 0, 0);
}