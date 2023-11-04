#pragma once
#include "SFML/Graphics.hpp"


//enum class BONUS_TYPE {
//	BAR_CHANGE,
//	SPEED_CHANGE,
//	STICK_BAR,
//	BOTTOM_REFLECT,
//	CHANGE_TRAJECTORY,
//	MOVING_BLOCK,
//	DEFAULT
//};

class Field;
class Ball;
class Bar;

class Bonus: public sf::Sprite
{
private:
	float _speed;
	sf::Vector2u _size;
private:
	const static float DEFAULT_BONUS_SPEED;
	const static int BONUS_SPAWN_CHANCE;
	const static sf::Vector2u DEFAULT_SPRITE_SIZE;
public:
	const static int NUM_BONUS_TYPES;
public:
	Bonus(const sf::Vector2f& pos, const sf::Texture& texture);
	
	void Move() { move({ 0,_speed }); };
	virtual void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
						  std::shared_ptr<Bar> bar) = 0;
	bool IsContactedWithBar(const Bar& bar) const;
	void Draw(std::shared_ptr<sf::RenderWindow> window) { window->draw(*this); };

	virtual ~Bonus() {};

};

class LongerBarBonus : public Bonus {
private:
	float _size_multiplier;
private:
	const static float DEFAULT_SIZE_MULTIPLIER;
public:
	LongerBarBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture),
		_size_multiplier(DEFAULT_SIZE_MULTIPLIER) {};
	~LongerBarBonus() {};

	void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);

};

class BallSpeedUpBonus : public Bonus {
private:
	float _speed_multiplier;
private:
	const static float DEFAULT_SPEED_MULTIPLIER;
public:
	BallSpeedUpBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture),
		_speed_multiplier(DEFAULT_SPEED_MULTIPLIER) {};
	~BallSpeedUpBonus() {};

	void SetSpeedMultiplier(float multiplier) { _speed_multiplier = multiplier; };
	
	void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);

};

class StickToBarBonus : public Bonus {
public:
	StickToBarBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture) {};
	~StickToBarBonus() {};

	void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);

};


class BotReflectBonus : public Bonus {
public:	
	BotReflectBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture) {};
	~BotReflectBonus() {};

	virtual void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);
};


class ChangeBallDirBonus : public Bonus {
public:
	ChangeBallDirBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture) {};
	~ChangeBallDirBonus() {};

	virtual void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);
};


class MovingBlockBonus: public Bonus {
public:
	MovingBlockBonus(const sf::Vector2f& pos, const sf::Texture& texture) :
		Bonus(pos, texture) {};
	~MovingBlockBonus() {};

	void Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
		std::shared_ptr<Bar> bar);
};

