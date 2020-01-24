//
// Advent of Code 2018, day 24, part one
//

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <list>
#include <numeric>

enum AttackType : unsigned int {
	cold = 1U,
	radiation = 1U << 1,
	slashing = 1U << 2,
	fire = 1U << 3,
	bludgeoning = 1U << 4
};

struct Group {
	const std::string name;
	bool infection = false;
	int units = 0;
	int hp = 0;
	int ap = 0;
	int initiative = 0;
	unsigned int attack_type = 0;
	unsigned int immune = 0;
	unsigned int weak = 0;
	std::list<Group>::iterator targeting;
};

constexpr int effective_power(const Group &group)
{
	return group.units * group.ap;
}

constexpr bool targets_before(const Group &lhs, const Group &rhs) noexcept
{
	int lhs_ep = effective_power(lhs);
	int rhs_ep = effective_power(rhs);

	return lhs_ep != rhs_ep ? lhs_ep > rhs_ep : lhs.initiative > rhs.initiative;
}

constexpr bool attacks_before(const Group &lhs, const Group &rhs) noexcept
{
	return lhs.initiative > rhs.initiative;
}

void select_targets_for_army(std::list<Group> &attackers, std::list<Group> &defenders)
{
	attackers.sort(targets_before);

	std::list<Group> targets;
	targets.swap(defenders);

	for (auto &grp : attackers) {
		int max_damage = std::numeric_limits<int>::min();
		auto target = defenders.end();

		for (auto it = targets.begin(); it != targets.end(); ++it) {
			int damage = effective_power(grp);

			if (grp.attack_type & it->immune) {
				damage = 0;
			}

			if (grp.attack_type & it->weak) {
				damage *= 2;
			}

			if (damage > max_damage
			 || (damage == max_damage
			  && (effective_power(*it) > effective_power(*target)
			   || (effective_power(*it) == effective_power(*target)
			    && it->initiative > target->initiative)))) {
				max_damage = damage;
				target = it;
			}
		}

		if (max_damage == 0) {
			target = defenders.end();
		}

		grp.targeting = target;

		if (target != defenders.end()) {
			defenders.splice(defenders.begin(), targets, target);
		}
	}

	defenders.splice(defenders.begin(), targets);
}

void attack_target(Group &grp, Group &target)
{
	int damage = effective_power(grp);

	if (grp.attack_type & target.immune) {
		damage = 0;
	}

	if (grp.attack_type & target.weak) {
		damage *= 2;
	}

	target.units -= damage / target.hp;

	if (target.units < 0) {
		target.units = 0;
	}
}

void attack_targets(std::list<Group> &attackers, std::list<Group> &defenders)
{
	attackers.sort(attacks_before);
	defenders.sort(attacks_before);

	auto attack_grp = attackers.begin();
	auto defend_grp = defenders.begin();

	while (attack_grp != attackers.end() || defend_grp != defenders.end()) {
		if (attack_grp != attackers.end() && defend_grp != defenders.end()) {
			if (attacks_before(*attack_grp, *defend_grp)) {
				if (attack_grp->targeting != defenders.end()) {
					attack_target(*attack_grp, *attack_grp->targeting);
				}
				++attack_grp;
			}
			else {
				if (defend_grp->targeting != attackers.end()) {
					attack_target(*defend_grp, *defend_grp->targeting);
				}
				++defend_grp;
			}

			continue;
		}

		if (attack_grp != attackers.end()) {
			if (attack_grp->targeting != defenders.end()) {
				attack_target(*attack_grp, *attack_grp->targeting);
			}
			++attack_grp;

			continue;
		}

		if (defend_grp->targeting != attackers.end()) {
			attack_target(*defend_grp, *defend_grp->targeting);
		}
		++defend_grp;
	}
}

bool has_groups_alive(const std::list<Group> &side)
{
	for (const auto &grp : side) {
		if (grp.units > 0) {
			return true;
		}
	}

	return false;
}

int units_alive(const std::list<Group> &side)
{
	return std::accumulate(side.begin(), side.end(), 0,
		[](int acc, const Group &grp) {
			return acc + grp.units;
		});
}

int main()
{
	std::list<Group> immune_system = {
		{ "Imm A", false, 8808, 5616, 5, 10, AttackType::bludgeoning, AttackType::cold, AttackType::radiation },
		{ "Imm B", false, 900, 13511, 107, 20, AttackType::radiation, 0, AttackType::radiation },
		{ "Imm C", false, 581, 10346, 140, 14, AttackType::fire, AttackType::slashing, AttackType::radiation },
		{ "Imm D", false, 57, 9991, 1690, 4, AttackType::fire, AttackType::slashing | AttackType::radiation | AttackType::fire, AttackType::bludgeoning },
		{ "Imm E", false, 4074, 6549, 15, 2, AttackType::radiation, 0, AttackType::fire },
		{ "Imm F", false, 929, 5404, 45, 16, AttackType::fire, AttackType::bludgeoning | AttackType::radiation, 0 },
		{ "Imm G", false, 2196, 3186, 10, 11, AttackType::fire, AttackType::radiation, AttackType::fire },
		{ "Imm H", false, 4420, 9691, 21, 7, AttackType::fire, AttackType::fire, AttackType::radiation },
		{ "Imm I", false, 3978, 2306, 4, 12, AttackType::fire, 0, AttackType::cold | AttackType::radiation },
		{ "Imm J", false, 1284, 4487, 32, 19, AttackType::slashing, 0, AttackType::radiation | AttackType::bludgeoning }
	};

	std::list<Group> infection = {
		{ "Inf A", true, 4262, 23427, 9, 8, AttackType::slashing, AttackType::fire, AttackType::slashing },
		{ "Inf B", true, 217, 9837, 73, 1, AttackType::bludgeoning, 0, AttackType::bludgeoning },
		{ "Inf C", true, 5497, 33578, 11, 17, AttackType::slashing, 0, AttackType::radiation | AttackType::cold },
		{ "Inf D", true, 866, 41604, 76, 15, AttackType::radiation, 0, AttackType::cold },
		{ "Inf E", true, 1823, 19652, 20, 13, AttackType::slashing, 0, AttackType::fire | AttackType::cold },
		{ "Inf F", true, 2044, 23512, 22, 9, AttackType::slashing, 0, AttackType::cold },
		{ "Inf G", true, 373, 40861, 215, 18, AttackType::slashing, AttackType::cold, 0 },
		{ "Inf H", true, 5427, 43538, 15, 5, AttackType::slashing, AttackType::radiation, AttackType::bludgeoning },
		{ "Inf I", true, 3098, 19840, 12, 3, AttackType::radiation, 0, AttackType::bludgeoning | AttackType::cold },
		{ "Inf J", true, 785, 14669, 30, 6, AttackType::fire, 0, 0 }
	};

	while (has_groups_alive(immune_system) && has_groups_alive(infection)) {
		select_targets_for_army(immune_system, infection);
		select_targets_for_army(infection, immune_system);

		attack_targets(immune_system, infection);

		immune_system.remove_if([](const Group &grp) { return grp.units <= 0; });
		infection.remove_if([](const Group &grp) { return grp.units <= 0; });
	}

	std::cout << units_alive(immune_system) << ' ' << units_alive(infection) << '\n';

	return 0;
}
