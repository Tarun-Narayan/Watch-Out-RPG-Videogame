#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>

/*

Making changes to the structure of the components will require changes to the following files:
game_save_manager.cpp (serialize and deserialize the attributes you are adding or removing)

*/


// PlayerComponents 
struct Player {
	unsigned int health = 100;
	bool isRunning;     // Indicates if the player is currently running
	bool isRolling;     // Indicates if the player is currently rolling
	bool goingUp;		// Key for going up is held down
	bool goingDown;		// Key for going down is held down
	bool goingLeft;		// Key for going left is held down
	bool goingRight;	// Key for going right is held down
	bool tryingToJump;	// Key for jumping is held down
	bool isMoving;		// Indicates if any movement keys are pressed
};

//Stamina
struct Stamina {
	float stamina = 100;
	float max_stamina = 100;     
    float stamina_loss_rate = 50;
	float stamina_recovery_rate = 10;
	float timer = 3000;
};

//Dashing
struct Dash {
    bool isDashing;				// Indicates if the entity is currently dashing
	vec2 dashStartPosition;		// Start position for dash
    vec2 dashTargetPosition;	// Target position for dash
    float dashTimer = 0.0f;     // Timer to track dash progress
	float dashDuration = 0.2f;  // Duration of Dash
};

struct Enemy
{
	int health = 100;
	int maxHealth = 100;
	int damage = 10;
	std::string type;
	unsigned int cooldown = 0;
	float pathfindTime = 0;
	int points = 1;
};

struct Trappable {
	bool isTrapped = false;
	float originalSpeed = 0;
};

struct Explosion {
	float duration = 500;
};

struct Damaging {
	std::string type = "arrow"; // default type
	unsigned int damage = 10;
	Entity excludedEntity;
};

enum class PROJECTILE_TYPE {
	TRAP,
	PHANTOM_TRAP,
	ARROW,
	BOMB_FUSED
};

struct Projectile {
	float sticksInGround = 3000; // ms it lasts on the ground
	PROJECTILE_TYPE type = PROJECTILE_TYPE::ARROW;
};

struct HomingProjectile{
	Entity targetEntity;
	HomingProjectile(Entity& targetEntity) { 
		this->targetEntity = targetEntity; 
	}
	float speed;
};

struct Bounceable {
	int numBounces = 1;
};

struct HealthBar {
	Entity meshEntity;
	Entity frameEntity;
	Entity textEntity;
	float width;
	float height;
	HealthBar(Entity& meshEntity, Entity& frameEntity) { 
		this->meshEntity = meshEntity; 
		this->frameEntity = frameEntity; 
	}
};

struct StaminaBar {
	Entity meshEntity;
	Entity frameEntity;
	Entity textEntity;
	float width;
	float height;
	StaminaBar(Entity& meshEntity, Entity& frameEntity) { 
		this->meshEntity = meshEntity; 
		this->frameEntity = frameEntity; 
	};
};

// Collectible Component
struct Collectible
{
	float duration = 5000.f; // 5 seconds until it disappears
	float timer = 0; 
	vec2 position = { 0, 0 };
	vec2 scale = { 3, 3 };
	std::string type;

};

struct Collected {
	float duration = 2500;
};

// Trap Component
struct BaseTrap {
	vec2 position = { 0, 0 };
	vec2 scale = { 3, 3 };
	float duration = 10000;
};

// Inheritance
struct Trap : public BaseTrap
{
	// fixed position and scale once set
	unsigned int damage = 15.0;
	float slowFactor = 0.1f;
};

struct PhantomTrap : public BaseTrap
{
	using BaseTrap::BaseTrap;
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec3 position = { 0, 0, 0 };
	float angle = 0;
	vec3 velocity = { 0, 0, 0 };
	float speed = 0;			// max voluntary speed
	vec2 scale = { 10, 10 };	// only for rendering
	vec2 facing = { 0, 0 };		// direction the entity is facing

	// Hitbox
	vec3 hitbox = { 0, 0, 0 };
	float gravity = 1.0;			// 1 means affected by gravity normally, 0 is no gravity
	bool solid = false;
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// Collision Cooldown
struct Cooldown
{
	float remaining = 0.0f;
};


// Entity was damaged recently
struct Damaged
{
	float timer = 400;
};

// DeathTimer Component
struct DeathTimer
{
	float timer = 3000;
};

struct Invulnerable
{
	float timer = 1000;
};

struct Knockable
{
	bool knocked = false;
};

struct Knocker
{
	float strength = 1.f;
};

struct TrapsCounter {
	// <trap type, <number of traps, trap text entity>>
	std::unordered_map<std::string, std::pair<int, Entity>> trapsMap;

	void reset() {
		// reset trap counts to 0
		//for (auto& trap : trapsMap) {
		//	trap.second.first = 0;
		//}
		trapsMap.clear();
	}
};

struct MapTile {
};

struct Obstacle {

};

struct TargetArea {
};

enum class TEXT_ALIGNMENT {
	LEFT,
	CENTER,
	RIGHT
};

struct Text {
	std::string value;
	Entity anchoredWorldEntity;
	vec2 anchoredWorldOffset;
	TEXT_ALIGNMENT alignment = TEXT_ALIGNMENT::LEFT;
	float lineSpacing = 1.3f;
};

struct SlideUp {
	float animationLength = 1500;
	float slideUpDuration = 200;
	float screenDistanceY = 50;
	float screenStartY = 0;
	float elapsedMs = 0;
	bool fadeIn = false;
};

struct TextChar {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

struct FPSTracker {
	int fps = 0;
	int counter = 0;
	float elapsedTime = 0;
	Entity textEntity;
	bool toggled = false;
	void update(float elapsed_ms) {
		elapsedTime += elapsed_ms;
		counter += 1;

    	if(elapsedTime >= 1000) {
        	fps = counter;
        	counter = 0;
        	elapsedTime = 0;
    	}
	}
};

struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = { 1,1 };
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Entity can jump
struct Jumper
{
	float speed = 0;
	bool isJumping = false;
};

// Enemy types
struct Boar {
	float cooldownTimer = 0;        
    float prepareTimer = 0;         
    float chargeTimer = 0;          
    bool preparing = false;         
    bool charging = false;          

    vec2 chargeDirection = vec2(0);
};
struct Barbarian {};
struct Archer {
	float drawArrowTime = 0;
	bool aiming = false;
};
struct Bird {
	float swarmSpeed = 0.6f;
	float swoopSpeed = 0.5f;
	bool isSwooping = false;
	float swoopTimer = 500;
	vec2 swoopDirection = {0,0};
	float originalZ = 480;
	float swoopCooldown = 2000;
};

enum WizardState { Moving, Aiming, Preparing, Shooting };
struct Wizard {
	WizardState state = WizardState::Moving;
	float shoot_cooldown = 0;
	float prepareLightningTime = 0;
	
	vec3 locked_target = vec3(0, 0, 0);
};

struct Troll {
	float desiredAngle = 0;
	float laughCooldown = 20000.f;
};

struct Bomber {
	float throwBombDelay = 0;
	float throwBombDelayTimer = 0;
	bool aiming = false;
};

// Collectible types
struct Heart { unsigned int health = 20; };
struct CollectibleTrap 
{ 
	std::string type = "trap"; 
};
struct Bow {};
struct CollectibleBomb {};
struct PauseMenuComponent {};
struct HelpMenuComponent {};
struct TutorialComponent{
	int tutorialStep = 1;
	int maxTutorialSteps = 4;
};
struct EnemyTutorialComponents{};
struct CollectibleTutorialComponents{};