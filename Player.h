#pragma once
#include "settings.h"
#include "laser.h"
#include <list>
#include "textObj.h"

class Player {
private:
	sf::Sprite sprite;
	sf::Texture texture;
	float speedX;
	std::list<Laser*> laserSprites;
	sf::Clock timer;
	int hp;
	int point;
	Textobj hpText;
	Textobj pointText;

public:
	Player() : hpText(std::to_string(hp), sf::Vector2f{ 0.f,0.f }, FONT_SIZE), {
		texture.loadFromFile(SHIP_FILENAME);
		sprite.setTexture(texture);
		sprite.setPosition(START_POS);
		timer.restart();
		hp = 100;
		point = 0;
	}
	void fire() {
		int now = timer.getElapsedTime().asMilliseconds();
		if (now > FIRE_COOLDOWN) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				sf::Vector2f pos = sprite.getPosition();
				sf::FloatRect bounds = sprite.getGlobalBounds();
				sf::Vector2f posCenter = sf::Vector2f{ pos.x + bounds.width / 2,pos.y + bounds.height / 2 };
				auto laser = new Laser(posCenter);
				laserSprites.push_back(laser);
				timer.restart();
			}
		}
	}

	void update() {
		speedX = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			speedX = -SPEED_X;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			speedX = SPEED_X;
		}
		if (sprite.getPosition().x < 0) {
			sprite.setPosition(0.f, sprite.getPosition().y);
		}
		else if (sprite.getPosition().x > WINDOW_WIDTH - sprite.getGlobalBounds().width) {
			sprite.setPosition(WINDOW_WIDTH - sprite.getGlobalBounds().width, sprite.getPosition().y);
		}
		sprite.move(speedX, 0.f);
		fire();
		for (auto laser : laserSprites) {
			laser->update();
		}
		hpText.update("HP:" + std::to_string(hp));
	}
	void draw(sf::RenderWindow& window) {
		for (auto laser : laserSprites) {
			window.draw(laser->getSprite());
		}
		window.draw(sprite);
		hpText.draw(window);
	}
	sf::FloatRect getHitBox() { return sprite.getGlobalBounds(); }
	std::list<Laser*>* getLaserSprites() { return &laserSprites; }

	int getHp() { return hp; }

	void hpChange(int damage) {
		hp = hp - damage;
	}

	void pointChange(int damage) {
		point = point + damage;
	}

	bool isAlife() { return hp > 0; }
	
};