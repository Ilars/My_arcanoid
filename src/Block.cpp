#include "Block.hpp"
#include "Game.hpp"

using namespace sf;

const int Block::MAX_HEALTH = 3;
const float MovingBlock::DEFAULT_BLOCK_SPEED = 1.f;

Block::Block(Game& game, const sf::Vector2f& size, const sf::Vector2f& pos,sf::Color c): 
	_game(game) {
	setSize(size);
	setFillColor(c);
	setOutlineColor(sf::Color::Black);
	setOutlineThickness(0.5f);
	setPosition(pos);
	_move_dir = -1;
	_health = MAX_HEALTH;
}

int Block::ReduceHealth() {
	_health--;
	if (_health == 0) setFillColor(Color::White);
	if (_health == MAX_HEALTH - 1) setFillColor(Color::Yellow);
	else if (_health == MAX_HEALTH - 2) setFillColor(Color::Red);

	return _health;
}

void Block::Hitted() {
	_game.IncreaseScore();
}

void BlockWithBonus::Hitted() {
	_game.SpawnRandomBonus(*this);
	_game.IncreaseScore();
}

void SpeedUpBlock::Hitted() {
	_game.BallSpeedUp();
	_game.IncreaseScore();
}

void MovingBlock::Move() {
	if (getPosition().x <= 0 || getPosition().x + getSize().x >= Game::WINDOW_SIZE.x)
		SetDirection(GetDirection() * -1);

	move({ GetDirection() * _speed , 0 });
}

