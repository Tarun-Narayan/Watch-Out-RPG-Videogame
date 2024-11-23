#pragma once

#include "tiny_ecs_registry.hpp"
#include "sound_system.hpp"

#include <random>

class AISystem {
public:
	AISystem(std::default_random_engine& rng, SoundSystem* sound);
	void step(float elapsed_ms);
	void boarReset(Entity boar);

private:
	const float LIGHTNING_RADIUS = 200.f;

	bool decideToPathfind(Entity enemy, float baseThinkingTime, float elapsed_ms);
	void moveTowardsPlayer(Entity enemy, vec3 playerPosition, float elapsed_ms);
	vec2 chooseDirection(Motion& motion, vec3 playerPosition);
	bool pathClear(Motion& motion, vec2 direction, float howFar, const std::vector<Entity> &obstacles, float& clearDistance);
	
	void boarBehaviour(Entity boar, vec3 playerPosition, float elapsed_ms);
	void barbarianBehaviour(Entity barbarian, vec3 playerPosition, float elapsed_ms);
	void trollBehaviour(Entity troll, vec3 playerPosition, float elapsed_ms);

	void bomberBehaviour(Entity entity, vec3 playerPosition, float elapsed_ms);
	void throwBomb(Entity thrower, vec3 targetPos);

	// Archer functions
	void archerBehaviour(Entity entity, vec3 playerPosition, float elapsed_ms);
	void shootArrow(Entity shooter, vec3 targetPos);

	// Bird functions
	void birdBehaviour(Entity bird, vec3 playerPosition, float elapsed_ms);
	void swoopAttack(Entity bird, vec3 playerPosition, float elapsed_ms, const std::vector<Motion>& flockMates);
	
	// Wizard functions
	void wizardBehaviour(Entity entity, vec3 playerPosition, float elapsed_ms);
	void shootFireball(Entity shooter, vec3 targetPos);
	void triggerLightning(vec3 targetPos);

	// Wizard State Processing
	void processWizardMoving(Entity wizard, vec3 playerPosition, float elapsed_ms);
	void processWizardPreparing(Entity wizard, vec3 playerPosition, float elapsed_ms);
	void processWizardAiming(Entity wizard, vec3 playerPosition, float elapsed_ms);
	void processWizardShooting(Entity wizard, vec3 playerPosition, float elapsed_ms);

	vec2 randomDirection();

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1

	SoundSystem* sound;
};