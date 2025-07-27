#include "BattleContext.h"
#include "RandomEngine.h"
#include "../ui/interfaces/IStatusEffectUI.h"
#include "../data/Move.h"
#include "../moves/MoveEffectEnums.h"

#include "StatusEffectProcessor.h"

StatusEffectProcessor::StatusEffectProcessor(BattleContext& context, RandomEngine& rng, IStatusEffectUI& statusEffectUI)
	: m_context(context), m_rng(rng), m_statusEffectUI(statusEffectUI) {}

bool StatusEffectProcessor::CheckPerformativeStatus()
{
	bool canPerform{ true };

	if (m_context.attackingPokemon->IsRecharging())
	{
		return true;
	}

	switch (m_context.attackingPokemon->currentStatus)
	{
	case Status::Sleeping:
		canPerform = SleepStatus();
		break;

	case Status::Frozen:
		canPerform = FrozenStatus();
		break;
	}

	if (m_context.attackingPokemon->IsFlinched() && canPerform == true)
	{
		canPerform = FlinchStatus();
	}

	if (m_context.attackingPokemon->IsConfused() && canPerform == true)
	{
		canPerform = ConfusedStatus();
	}

	if (m_context.attackingPokemon->currentStatus == Status::Paralyzed && canPerform == true)
	{
		canPerform = ParalysisStatus();
	}

	if (m_context.currentMove->b_isDisabled && canPerform == true)
	{
		canPerform = false;
		m_statusEffectUI.DisplayMoveIsDisabledMsg();
	}

	if (m_context.attackingPokemon->IsCharging() && canPerform == false)
	{
		m_context.attackingPokemon->SetCharging(false);
	}

	if (m_context.attackingPokemon->IsThrashing() && canPerform == false)
	{
		m_context.attackingPokemon->IncrementThrashCounter();

		if (m_context.attackingPokemon->GetThrashCounter() == m_context.attackingPokemon->GetThrashTurnCount() && !m_context.attackingPokemon->IsConfused())
		{
			ThrashConfuse();
		}

		ThrashStop();
		ThrashReset();
	}

	return canPerform;
}

bool StatusEffectProcessor::SleepStatus()
{
	// Bulbapedia mentions only sleep fully disrupts bide, however on Showdown bide is fully disrupted after flinch, and a full paralysis as well
	// If someone could test on a Gen 7 cart how bide interacts with certain status effects that would be greatly appreciated
	if (m_context.attackingPokemon->IsBiding())
	{
		BideStop();
		BideReset();
	}

	if (m_context.attackingPokemon->GetSleepCounter() >= m_context.attackingPokemon->GetSleepTurnCount())
	{
		m_context.attackingPokemon->ChangeStatus(Status::Normal);
		m_context.attackingPokemon->ResetSleepCounter();
		m_context.attackingPokemon->SetSleepTurnCount(0);

		m_statusEffectUI.DisplayWokenUpMsg();
		return true;
	}
	else
	{
		m_context.attackingPokemon->IncrementSleepCounter();
		m_statusEffectUI.DisplayIsAsleepMsg();
		return false;
	}
}

bool StatusEffectProcessor::FrozenStatus()
{
	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));

	if (randomMod <= 80)
	{
		m_statusEffectUI.DisplayFrozenSolidMsg();
		return false;
	}
	else
	{
		m_context.attackingPokemon->ChangeStatus(Status::Normal);
		m_statusEffectUI.DisplayThawedMsg();
		return true;
	}
}

bool StatusEffectProcessor::FlinchStatus()
{
	m_statusEffectUI.DisplayFlinchMsg();
	m_context.attackingPokemon->SetIsFlinched(false);

	return false;
}

bool StatusEffectProcessor::ConfusedStatus()
{
	if (m_context.attackingPokemon->GetConfusedCounter() >= m_context.attackingPokemon->GetConfusedTurnCount())
	{
		m_context.attackingPokemon->SetConfusedStatus(false);
		m_context.attackingPokemon->ResetConfusedCounter();
		m_context.attackingPokemon->SetConfusedTurnCount(0);

		m_statusEffectUI.DisplayNoLongerConfusedMsg();
		return true;
	}
	else
	{
		m_context.attackingPokemon->IncrementConfusedCounter();
		m_statusEffectUI.DisplayIsConfusedMsg();

		std::uniform_int_distribution<int> randomModDistributor(1, 100);
		int randomMod(randomModDistributor(m_rng.GetGenerator()));

		if (randomMod > 33)
		{
			return true;
		}
		else
		{
			m_statusEffectUI.DisplayHurtItselfConfuseMsg();

			std::uniform_int_distribution<int> damagemoddistributor(85, 100);
			double damagemod{ static_cast<double>(damagemoddistributor(m_rng.GetGenerator())) };
			double random{ (damagemod / 100.0) };

			// Confused damage does not take into account Pokemon's stat boosts, burn status, stab, nor critical, and is a typeless physical move
			double damage = floor(floor(floor(floor(2 * m_context.attackingPokemon->GetLevel() / 5 + 2) * 40 * (static_cast<double>(m_context.attackingPokemon->GetAttack()) / static_cast<double>(m_context.attackingPokemon->GetDefense())) / 50) + 2) * random);

			m_context.attackingPokemon->DamageCurrentHP(static_cast<int>(damage));

			if (m_context.attackingPokemon->IsBiding())
			{
				m_context.attackingPokemon->AddBideDamage(static_cast<int>(damage));
			}

			ResetPokemonTurnStatuses();

			return false;
		}
	}
}

bool StatusEffectProcessor::ParalysisStatus()
{
	std::uniform_int_distribution<int> randomModDistributor(1, 100);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));

	if (randomMod <= 25)
	{
		m_statusEffectUI.DisplayCantMoveParalysisMsg();
		ResetPokemonTurnStatuses();

		if (m_context.attackingPokemon->IsBiding())
		{
			if (m_context.attackingPokemon->GetBideCounter() < 2)
			{
				m_context.attackingPokemon->IncrementBideCounter();
			}
			else
			{
				BideStop();
				BideReset();
			}
			
		}

		return false;
	}
	else
	{
		return true;
	}

}

void StatusEffectProcessor::ThrashStop()
{
	m_context.attackingPokemon->SetThrashing(false);
	m_context.attackingPlayer->SetCanSwitch(true);
}

void StatusEffectProcessor::ThrashConfuse()
{
	m_statusEffectUI.DisplayThrashConfusionMsg();

	m_context.attackingPokemon->SetConfusedStatus(true);

	std::uniform_int_distribution<int> randomModDistributor(2, 4);
	int randomMod(randomModDistributor(m_rng.GetGenerator()));
	m_context.attackingPokemon->SetConfusedTurnCount(randomMod);
	m_context.attackingPokemon->ResetConfusedCounter();
}

void StatusEffectProcessor::ThrashReset()
{
	m_context.attackingPokemon->ResetThrashCounter();
	m_context.attackingPokemon->SetThrashTurnCount(0);
}

void StatusEffectProcessor::BideStop()
{
	m_context.attackingPokemon->SetBide(false);
	m_context.attackingPlayer->SetCanSwitch(true);
}

void StatusEffectProcessor::BideReset()
{
	m_context.attackingPokemon->ResetBideCounter();
	m_context.attackingPokemon->SetBideTurnCount(0);
	m_context.attackingPokemon->ResetBideDamage();
}

void StatusEffectProcessor::CheckSubstituteCondition(Player* targetPlayer, BattlePokemon* targetPokemon)
{
	if (targetPokemon->GetSubstituteHP() <= 0 && targetPokemon->HasSubstitute())
	{
		m_statusEffectUI.DisplaySubstituteFadedMsg();
		targetPokemon->SetSubstitute(false);
	}
}

void StatusEffectProcessor::RageCheck()
{
	if (!m_context.attackingPokemon->IsRaging() && !m_context.defendingPokemon->IsRaging())
	{
		return;
	}

	if ((m_context.defendingPokemon->IsRaging() && (m_context.damageTaken > 0 && !m_context.defendingPokemon->HasSubstitute()))
		|| m_context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Disable) // Target took damage or was targeted by Disable while raging
	{
		int attackStage = m_context.defendingPokemon->GetAttackStage();

		if (attackStage >= 6)
		{
			m_statusEffectUI.DisplayStatRaiseFailMsg("attack");
		}
		else
		{
			++attackStage;
			m_context.defendingPokemon->SetAttackStage(attackStage);
			m_statusEffectUI.DisplayStatRaised1Msg("attack");
		}
	}

	if (m_context.attackingPokemon->IsRaging() && m_context.currentMove->mp_move->GetMoveEffectEnum() != MoveEffect::Rage)
	{
		m_context.attackingPokemon->SetRaging(false);
	}
	else if (m_context.attackingPokemon->IsRaging() && m_context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Rage && m_context.currentMove->b_isDisabled)
	{
		m_context.attackingPokemon->SetRaging(false);
	}
	else if (m_context.attackingPokemon->IsRaging() && m_context.currentMove->mp_move->GetMoveEffectEnum() == MoveEffect::Rage && !m_context.currentMove->b_isDisabled)
	{
		m_statusEffectUI.DisplayRageStartedMsg();
	}
}

// If paralyze or confusion disrupts their charge (hyper beam, fly, dig, solar beam etc)
void StatusEffectProcessor::ResetPokemonTurnStatuses()
{
	if (m_context.attackingPlayer->CanSwitch() == true || m_context.attackingPokemon->IsCharging() == false || m_context.attackingPokemon->IsRecharging() == false || m_context.attackingPokemon->IsSemiInvulnerable() == false)
	{
		return;
	}

	m_context.attackingPlayer->SetCanSwitch(true);

	m_context.attackingPokemon->SetCharging(false);
	m_context.attackingPokemon->SetRecharging(false);
	m_context.attackingPokemon->SetSemiInvulnerableDig(false);
	m_context.attackingPokemon->SetSemiInvulnerableFly(false);
}

void StatusEffectProcessor::CheckFaintCondition(Player* sourcePlayer, Player* targetPlayer, BattlePokemon* source, BattlePokemon* target)
{
	if ((target->GetCurrentHP() <= 0) && (!target->IsFainted()))
	{
		target->SetFainted(true);
		m_statusEffectUI.DisplayFaintedMsg(*targetPlayer, *target);
		targetPlayer->IncrementFaintedCount();
	}

	if ((source->GetCurrentHP() <= 0) && (!source->IsFainted()))
	{
		source->SetFainted(true);
		m_statusEffectUI.DisplayFaintedMsg(*sourcePlayer, *source);
		sourcePlayer->IncrementFaintedCount();
	}
}