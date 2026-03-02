#pragma once

#include <unordered_map>
#include "../../../moves/MoveEffectEnums.h"
#include "AIScoreTag.h"

struct pokemonMove;

class Move;

class AIMoveClassifier {
public:
    static AIScoreTag Classify(const pokemonMove& move);

private:
	static inline const std::unordered_map<MoveEffect, AIScoreTag> s_effectToTag
	{
		{ MoveEffect::IncreasedCritical,     AIScoreTag::HighCritRatio },
		{ MoveEffect::OHKO,					 AIScoreTag::OHKO },
		{ MoveEffect::BodySlam,              AIScoreTag::TargetUsedMinimize },
		{ MoveEffect::Stomp,                 AIScoreTag::TargetUsedMinimize },
		{ MoveEffect::RecoilQuarter,         AIScoreTag::RecoilOnHit },
		{ MoveEffect::RecoilThird,           AIScoreTag::RecoilOnHit },
		{ MoveEffect::Gust,                  AIScoreTag::Gust },
		{ MoveEffect::Earthquake,            AIScoreTag::Earthquake },
		{ MoveEffect::AlwaysHit,             AIScoreTag::AlwaysHit },
		{ MoveEffect::RazorWind,             AIScoreTag::ChargeMove },
		{ MoveEffect::Fly,                   AIScoreTag::ChargeMove },
		{ MoveEffect::SolarBeam,             AIScoreTag::ChargeMove },
		{ MoveEffect::Dig,                   AIScoreTag::ChargeMove },
		{ MoveEffect::SkullBash,             AIScoreTag::ChargeMove },
		{ MoveEffect::SkyAttack,             AIScoreTag::SkyAttack },
		{ MoveEffect::RechargeAttack,        AIScoreTag::RechargeMove },
		{ MoveEffect::Leech,                 AIScoreTag::Leech },
		{ MoveEffect::DreamEater,            AIScoreTag::DreamEater },
		{ MoveEffect::Explosion,             AIScoreTag::SelfFaintingDamage },
		{ MoveEffect::SuperFang,             AIScoreTag::SuperFang },
		{ MoveEffect::HealHalfHP,            AIScoreTag::Healing },
		{ MoveEffect::Rest,                  AIScoreTag::Rest },
		{ MoveEffect::SleepMove,             AIScoreTag::Sleep },
		{ MoveEffect::SleepPowder,           AIScoreTag::Sleep },
		{ MoveEffect::Paralyze,              AIScoreTag::Paralyze },
		{ MoveEffect::PoisonGas,             AIScoreTag::Poison },
		{ MoveEffect::PoisonPowder,          AIScoreTag::Poison },
		{ MoveEffect::Toxic,                 AIScoreTag::Poison },
		{ MoveEffect::Confuse,               AIScoreTag::Confuse },
		{ MoveEffect::Disable,               AIScoreTag::Disable },
		{ MoveEffect::LeechSeed,             AIScoreTag::Seed },
		{ MoveEffect::ForceSwitch,           AIScoreTag::ForceSwitch },
		{ MoveEffect::AttackUp2,             AIScoreTag::SelfAttackBoost },
		{ MoveEffect::AttackUp,              AIScoreTag::SelfAttackBoost },
		{ MoveEffect::DefenseUp2,            AIScoreTag::SelfDefenseBoost },
		{ MoveEffect::DefenseUp,             AIScoreTag::SelfDefenseBoost },
		{ MoveEffect::Growth,                AIScoreTag::Growth },
		{ MoveEffect::SpecialDefenseUp2,     AIScoreTag::SelfSpDBoost },
		{ MoveEffect::SpeedUp2,              AIScoreTag::SelfSpeedBoost },
		{ MoveEffect::EvasionUp,             AIScoreTag::SelfEvasionBoost },
		{ MoveEffect::Minimize,              AIScoreTag::SelfEvasionBoost },
		{ MoveEffect::AttackDown,            AIScoreTag::TargetAttackDecrease },
		{ MoveEffect::DefenseDown2,          AIScoreTag::TargetDefenseDecrease },
		{ MoveEffect::DefenseDown,           AIScoreTag::TargetDefenseDecrease },
		{ MoveEffect::SpeedDown2,            AIScoreTag::TargetSpeedDecrease },
		{ MoveEffect::AccuracyDown,          AIScoreTag::TargetAccuracyDecrease },
		{ MoveEffect::FocusEnergy,			 AIScoreTag::FocusEnergy },
		{ MoveEffect::Mist,                  AIScoreTag::Mist },
		{ MoveEffect::LightScreen,           AIScoreTag::LightScreen },
		{ MoveEffect::Reflect,               AIScoreTag::Reflect },
		{ MoveEffect::Haze,                  AIScoreTag::Haze },
		{ MoveEffect::Mimic,                 AIScoreTag::Mimic },
		{ MoveEffect::Metronome,             AIScoreTag::Metronome },
		{ MoveEffect::MirrorMove,            AIScoreTag::MirrorMove },
		{ MoveEffect::Transform,             AIScoreTag::Transform },
		{ MoveEffect::Conversion,            AIScoreTag::Conversion },
		{ MoveEffect::Substitute,            AIScoreTag::Substitute }
	};
};
