#include "world_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"


WorldSystem::WorldSystem() {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

void WorldSystem::init(RenderSystem* renderer, GLFWwindow* window)
{
	this->renderer = renderer;
	this->window = window;

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);

	createJeff(renderer, vec2(100, 100));
	createBarbarian(renderer, vec2(200, 200));
	createBoar(renderer, vec2(400, 400));
	createArcher(renderer, vec2(100, 500));
}

WorldSystem::~WorldSystem() {
	// Destroy all created components
	registry.clear_all_components();
}

bool WorldSystem::step(float elapsed_ms)
{
	return false;
}

void WorldSystem::handle_collisions()
{
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;
		
		// If the entity is a player
		if (registry.players.has(entity)) {
			// If the entity is colliding with a collectible
			if (registry.collectibles.has(entity_other)) {
				// destroy the collectible
				registry.remove_all_components_of(entity_other);
				// increase collectible count in player
				Player& player = registry.players.get(entity);
				player.trapsCollected++;
				printf("Player collected a trap\n");
			}
			else if (registry.traps.has(entity_other)) {
				// destroy the trap
				registry.remove_all_components_of(entity_other);
				printf("Player hit a trap\n");

				// reduce player health
				Player& player = registry.players.get(entity);
				Trap& trap = registry.traps.get(entity_other);
				player.health -= trap.damage;
				printf("Player health reduced by trap from %d to %d\n", player.health + trap.damage, player.health);
			}
			else if (registry.enemies.has(entity_other)) {
				// player takes the damage
				Player& player = registry.players.get(entity);
				Enemy& enemy = registry.enemies.get(entity_other);
				player.health -= enemy.damage;
				printf("Player health reduced by enemy from %d to %d\n", player.health + enemy.damage, player.health);

				// TODO M1 [WO-13] - Change player color to (red) for a short duration
			}
		}
		else if (registry.enemies.has(entity)) {
			if (registry.traps.has(entity_other)) {
				// destroy the trap
				registry.remove_all_components_of(entity_other);
				printf("Enemy hit a trap\n");

				// reduce enemy health
				Enemy& enemy = registry.enemies.get(entity);
				Trap& trap = registry.traps.get(entity_other);
				enemy.health -= trap.damage;
				printf("Enemy health reduced from %d to %d\n", enemy.health + trap.damage, enemy.health);
			}
			else if (registry.enemies.has(entity_other)) {
				// Reduce health of both enemies
				Enemy& enemy1 = registry.enemies.get(entity);
				Enemy& enemy2 = registry.enemies.get(entity_other);

				enemy1.health -= enemy2.damage;
				enemy2.health -= enemy1.damage;
				printf("Enemy 1's health reduced from %d to %d\n", enemy1.health + enemy2.damage, enemy1.health);
				printf("Enemy 2's health reduced from %d to %d\n", enemy2.health + enemy1.damage, enemy2.health);
			}
		}
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

void WorldSystem::on_key(int key, int, int action, int mod)
{
	printf("clack\n");

	// Place a trap
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		if (registry.players.entities.size() == 0) {
			printf("No player entity found\n");
			return;
		}
		// Get player entity and its position
		Entity player = registry.players.entities[0];
		Motion& playerMotion = registry.motions.get(player);
		vec2 playerPos = playerMotion.position;

		// TODO LATER - Think about where exactly to place the trap
		// Currently, it is placed at the player's position
		// MAYBE - Place it behind the player in the direction they are facing

		// Reduce player's trap count
		Player& playerComponent = registry.players.get(player);
		if (playerComponent.trapsCollected == 0) {
			printf("Player has no traps to place\n");
			// TODO LATER - Do something to indicate that player has no traps [Milestone AFTER]
			return;
		}
		playerComponent.trapsCollected--;

		// Create a trap at player's position
		createDamageTrap(renderer, playerPos);
		printf("Trap placed at (%f, %f)\n", playerPos.x, playerPos.y);
	}
}

void WorldSystem::on_mouse_move(vec2 mouse_position)
{
	printf("vroom\n");
}

void WorldSystem::restart_game()
{
}
