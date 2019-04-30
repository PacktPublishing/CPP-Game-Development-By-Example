#include "Hero.h"


Hero::Hero(){}


Hero::~Hero(){}

void Hero::init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position, float mass) {

	m_position = position;
	m_mass = mass;
	m_grounded = false;

	m_frameCount = frameCount;
	m_animDuration = animDuration;

	// Load a Texture
	m_texture.loadFromFile(textureName.c_str());

	m_spriteSize = sf::Vector2i(92, 126);

	// Create Sprite and Attach a Texture
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_spriteSize.x, m_spriteSize.y));

	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y / 2);

}

void Hero::update(float dt) {


	// Animate Sprite

	m_elapsedTime += dt;

	int animFrame = static_cast<int> ((m_elapsedTime / m_animDuration) * m_frameCount) % m_frameCount;

	m_sprite.setTextureRect(sf::IntRect(animFrame * m_spriteSize.x, 0, m_spriteSize.x, m_spriteSize.y));

	// Update Position

	m_velocity -= m_mass * m_gravity * dt;

	m_position.y -= m_velocity * dt;

	m_sprite.setPosition(m_position);


	if (m_position.y >= 768 * 0.75) {

		m_position.y = 768 * 0.75;
		m_velocity = 0;
		m_grounded = true;
		jumpCount = 0;
	}

}
void Hero::jump(float velocity){

	if (jumpCount < 2) {
		jumpCount++;

		m_velocity = velocity;
		m_grounded = false;
	}

}

sf::Sprite Hero::getSprite(){

	return m_sprite;
}

