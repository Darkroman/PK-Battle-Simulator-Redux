#include "BattleStatusManager.h"

BattleStatusManager::BattleStatusManager(BattleContext& context, RandomEngine& rng) : m_context(context), m_rng(rng) {}

bool BattleStatusManager::CheckPerformativeStatus(BattleContext& context)
{
	bool canPerform{ true };

	if (context.attackingPokemon->IsRecharging())
	{
		return true;
	}

	switch (context.attackingPokemon->currentStatus)
	{
	case Status::Sleeping:
		canPerform = SleepStatus(context.attackingPokemon);
		break;

	case Status::Frozen:
		canPerform = FrozenStatus(context.attackingPokemon);
		break;
	}

	if (context.attackingPokemon->IsFlinched() && canPerform == true)
	{
		canPerform = FlinchStatus(context);
	}

	if (context.attackingPokemon->IsConfused() && canPerform == true)
	{
		canPerform = ConfusedStatus(context);
	}

	if (context.attackingPokemon->currentStatus == Status::Paralyzed && canPerform == true)
	{
		canPerform = ParalysisStatus(context);
	}

	if (context.currentMove->b_isDisabled && canPerform == true)
	{
		canPerform = false;
		std::cout << context.attackingPlayer->GetPlayerNameView() << "'s " << context.attackingPokemon->GetNameView() << "'s " << context.currentMove->mp_move->GetName() << " is disabled!\n";
	}

	if (canPerform == false)
	{
		context.attackingPokemon->SetCharging(false);
	}

	if (context.attackingPokemon->IsThrashing() && canPerform == false)
	{
		context.attackingPokemon->IncrementThrashCounter();

		if (context.attackingPokemon->GetThrashCounter() == context.attackingPokemon->GetThrashTurnCount() && !context.attackingPokemon->IsConfused())
		{
			ThrashConfuse();
		}

		ThrashStop();
		ThrashReset();
	}

	// Bulbapedia mentions only sleep fully disrupts bide, however on Showdown bide is fully disrupted after flinch, and a full paralysis as well
	// If someone could test on a Gen 7 cart how bide interacts with certain status effects that would be greatly appreciated
	if (context.attackingPokemon->IsBiding() && context.attackingPokemon->currentStatus == Status::Sleeping && canPerform == false)
	{
		BideStop();
		BideReset();
	}

	return canPerform;
}

bool BattleStatusManager::SleepStatus(BattlePokemon* source)
{
	if (source->GetSleepCounter() >= source->GetSleepTurnCount())
	{
		source->ChangeStatus(Status::Normal);
		source->ResetSleepCounter();
		source->SetSleepTurnCount(0);

		std::cout << source->GetNameView() << " has woken up!\n";
		return true;
	}
	else
	{
		source->IncrementSleepCounter();
		std::cout << source->GetNameView() << " is sleeping\n";
		return false;
	}
}

bool BattleStatusManager::FrozenStatus(BattlePokemon* source)
{
	std::uniform_int_distribution<int> randomModDistributor(1, 101);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));

	if (randomMod <= 80)
	{
		std::cout << source->GetNameView() << " is frozen solid!\n";
		return false;
	}
	else
	{
		source->ChangeStatus(Status::Normal);
		std::cout << source->GetNameView() << " thawed out!\n";
		return true;
	}
}

bool BattleStatusManager::FlinchStatus(BattleContext& context)
{
	std::cout << context.attackingPlayer->GetPlayerNameView() << "'s " << context.attackingPokemon->GetNameView() << " flinched and couldn't move!\n";
	context.attackingPokemon->SetIsFlinched(false);

	return false;
}

bool BattleStatusManager::ConfusedStatus(BattleContext& context)
{
	if (context.attackingPokemon->GetConfusedCounter() >= context.attackingPokemon->GetConfusedTurnCount())
	{
		context.attackingPokemon->SetConfusedStatus(false);
		context.attackingPokemon->ResetConfusedCounter();
		context.attackingPokemon->SetConfusedTurnCount(0);

		std::cout << context.attackingPlayer->GetPlayerNameView() << "'s " << context.attackingPokemon->GetNameView() << " is no longer confused!\n";
		return true;
	}
	else
	{
		context.attackingPokemon->IncrementConfusedCounter();
		std::cout << context.attackingPlayer->GetPlayerNameView() << "'s " << context.attackingPokemon->GetNameView() << " is confused!\n";

		std::uniform_int_distribution<int> randomModDistributor(1, 101);
		int randomMod(randomModDistributor(m_rng.GetGenerator()));

		if (randomMod > 33)
		{
			return true;
		}
		else
		{
			std::cout << "It hurt itself in its confusion!\n";

			std::uniform_int_distribution<int> damagemoddistributor(85, 100);
			double damagemod{ static_cast<double>(damagemoddistributor(m_rng.GetGenerator())) };
			double random{ (damagemod / 100.0) };

			// Confused damage does not take into account Pokemon's stat boosts, burn status, stab, nor critical, and is a typeless physical move
			double damage = floor(floor(floor(floor(2 * context.attackingPokemon->GetLevel() / 5 + 2) * 40 * (static_cast<double>(context.attackingPokemon->GetAttack()) / static_cast<double>(context.attackingPokemon->GetDefense())) / 50) + 2) * random);

			context.attackingPokemon->DamageCurrentHP(static_cast<int>(damage));

			if (context.attackingPokemon->IsBiding())
			{
				context.attackingPokemon->AddBideDamage(static_cast<int>(damage));
			}

			ResetPokemonTurnStatuses(context);

			return false;
		}
	}
}

bool BattleStatusManager::ParalysisStatus(BattleContext& context)
{
	std::uniform_int_distribution<int> randomModDistributor(1, 101);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));

	if (randomMod <= 25)
	{
		std::cout << context.attackingPokemon->GetNameView() << " couldn't move because it's paralyzed!\n";
		ResetPokemonTurnStatuses(context);
		return false;
	}
	else
	{
		return true;
	}
}

void BattleStatusManager::ThrashStop()
{
	m_context.attackingPokemon->SetThrashing(false);
	m_context.attackingPlayer->SetCanSwitch(true);
}

void BattleStatusManager::ThrashConfuse()
{
	std::cout << m_context.attackingPlayer->GetPlayerNameView() << "'s " << m_context.attackingPokemon->GetNameView() << " became confused due to fatigue!\n";

	m_context.attackingPokemon->SetConfusedStatus(true);

	std::uniform_int_distribution<int> randomModDistributor(2, 4);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));
	m_context.attackingPokemon->SetConfusedTurnCount(randomMod);
	m_context.attackingPokemon->ResetConfusedCounter();
}

void BattleStatusManager::ThrashReset()
{
	m_context.attackingPokemon->ResetThrashCounter();
	m_context.attackingPokemon->SetThrashTurnCount(0);
}

void BattleStatusManager::BideStop()
{
	m_context.attackingPokemon->SetBide(false);
	m_context.attackingPlayer->SetCanSwitch(true);
}

void BattleStatusManager::BideReset()
{
	m_context.attackingPokemon->ResetBideCounter();
	m_context.attackingPokemon->SetBideTurnCount(0);
	m_context.attackingPokemon->ResetBideDamage();
}

void BattleStatusManager::CheckSubstituteCondition(Player* targetPlayer, BattlePokemon* targetPokemon)
{
	if (targetPokemon->GetSubstituteHP() <= 0 && targetPokemon->HasSubstitute())
	{
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << targetPokemon->GetNameView() << " substitute faded!\n";
		targetPokemon->SetSubstitute(false);
	}
}

void BattleStatusManager::RageCheck(BattleContext& context)
{
	if (!context.attackingPokemon->IsRaging() && !context.defendingPokemon->IsRaging())
	{
		return;
	}

	if ((context.defendingPokemon->IsRaging() && (context.damageTaken > 0 && !context.defendingPokemon->HasSubstitute()))
		|| context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Disable) // Target took damage or was targeted by Disable while raging
	{
		int attackStage = context.defendingPokemon->GetAttackStage();

		if (attackStage >= 6)
		{
			std::cout << context.defendingPlayer->GetPlayerNameView() << "'s " << context.defendingPokemon->GetNameView() << " can't raise its attack any higher!\n";
		}
		else
		{
			++attackStage;
			context.defendingPokemon->SetAttackStage(attackStage);
			std::cout << context.defendingPlayer->GetPlayerNameView() << "'s " << context.defendingPokemon->GetNameView() << "'s attack rose!\n";
		}
	}

	if (context.attackingPokemon->IsRaging() && context.currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::Rage)
	{
		context.attackingPokemon->SetRaging(false);
	}
	else if (context.attackingPokemon->IsRaging() && context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Rage && context.currentMove->b_isDisabled)
	{
		context.attackingPokemon->SetRaging(false);
	}
	else if (context.attackingPokemon->IsRaging() && context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Rage && !context.currentMove->b_isDisabled)
	{
		std::cout << "(Rage started on " << context.attackingPlayer->GetPlayerNameView() << "'s " << context.attackingPokemon->GetNameView() << ")\n";
	}
}

// If paralyze or confusion disrupts their charge (hyper beam, fly, dig, solar beam etc)
void BattleStatusManager::ResetPokemonTurnStatuses(BattleContext& context)
{
	if (context.attackingPlayer->CanSwitch() == true || context.attackingPokemon->IsCharging() == false || context.attackingPokemon->IsRecharging() == false || context.attackingPokemon->IsSemiInvulnerable() == false)
	{
		return;
	}

	context.attackingPlayer->SetCanSwitch(true);

	context.attackingPokemon->SetCharging(false);
	context.attackingPokemon->SetRecharging(false);
	context.attackingPokemon->SetSemiInvulnerableDig(false);
	context.attackingPokemon->SetSemiInvulnerableFly(false);
}

void BattleStatusManager::CheckFaintCondition(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* source, BattlePokemon* target)
{
	if ((target->GetCurrentHP() <= 0) && (!target->IsFainted()))
	{
		target->SetFainted(true);
		std::cout << targetPlayer->GetPlayerNameView() << "'s " << target->GetNameView() << " has fainted!\n";
		targetPlayer->IncrementFaintedCount();
	}

	if ((source->GetCurrentHP() <= 0) && (!source->IsFainted()))
	{
		source->SetFainted(true);
		std::cout << sourcePlayer->GetPlayerNameView() << "'s " << source->GetNameView() << " has fainted!\n";
		sourcePlayer->IncrementFaintedCount();
	}
}