#pragma once
#include "SFML/Graphics.hpp"

//
//enum class TYPE {
//	NORMAL,
//	INDESTRUCTABLE,
//	SPEED_UP,
//	HAS_BONUS,
//	MOVING,
//	DEFAULT
//};

class Game;

class Block: public sf::RectangleShape
{
private:
	int _health;
	int _move_dir;

private:
	const static int MAX_HEALTH;

protected:
	Game& _game;

protected:
	bool _is_undestructible = false;

public:
	Block(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos,
		sf::Color c = sf::Color::Green);
	virtual ~Block() {};

	virtual int ReduceHealth();
	int GetHealth() const { return _health; };

	virtual void Move() {};
	virtual void Hitted();
	
	void SetDirection(int dir) { if (dir == 1 || dir == -1) _move_dir = dir; };
	int GetDirection() const { return _move_dir; };
	
	bool IsUndestr() { return _is_undestructible; };

};


class BlockWithBonus : public Block {
public:
	BlockWithBonus(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos) :
		Block(game, size, pos) {};
	~BlockWithBonus() {};

	virtual void Hitted() override;

};

class MovingBlock : public Block {
private:
	float _speed;
public:
	static const float DEFAULT_BLOCK_SPEED;
public:
	MovingBlock(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos) :
		Block(game, size, pos), _speed(DEFAULT_BLOCK_SPEED) {};
	~MovingBlock() {};

	virtual void Move();

};

class SpeedUpBlock : public Block{
public:
	static const sf::Vector2f BLOCK_SPEED_INCREMENT;
public:
	SpeedUpBlock(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos) :
		Block(game, size, pos) {};
	~SpeedUpBlock() {};

	virtual void Hitted() override;
};


class IndestructableBlock : public Block {
public:
	IndestructableBlock(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos) :
		Block(game, size, pos, sf::Color::Magenta) {
		_is_undestructible = true;
	};
	~IndestructableBlock() {};

	virtual void Hitted() override {};
	virtual int ReduceHealth() { return -1; }
};

