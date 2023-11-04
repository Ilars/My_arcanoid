#include "Game.hpp"
#include "Bonus.hpp"
#include "iostream"

using namespace sf;

const float Bonus::DEFAULT_BONUS_SPEED = 1.f;
const int Bonus::BONUS_SPAWN_CHANCE = 10;
const sf::Vector2u Bonus::DEFAULT_SPRITE_SIZE = Vector2u(60U,60U);
const float LongerBarBonus::DEFAULT_SIZE_MULTIPLIER = 1.4f;
const float BallSpeedUpBonus::DEFAULT_SPEED_MULTIPLIER = 1.2f;
const int Bonus::NUM_BONUS_TYPES = 6;

Bonus:: Bonus(const sf::Vector2f& pos, const sf::Texture& texture): sf::Sprite(texture)
	{
	_size = DEFAULT_SPRITE_SIZE;
	_speed = DEFAULT_BONUS_SPEED;
	setPosition(pos);
	if (texture.getSize() !=_size)
		setScale(_size.x * 1.0 / texture.getSize().x, _size.y * 1.0 / texture.getSize().y);
	};

bool Bonus::IsContactedWithBar(const Bar& bar) const {

	auto bar_coord = bar.GetCoord();
	auto bar_size = bar.GetSize();
	
	auto bonus_size = _size;
	auto bonus_coord = getPosition();
	
	return ((bonus_coord.y + bonus_size.y) >= bar_coord.y) &&
		(bonus_coord.x + bonus_size.x >= bar_coord.x &&
		bonus_coord.x <= bar_coord.x + bar_size.x);
}

void LongerBarBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar) {
	
	auto cur_bar_size = bar->GetSize();
	Vector2f new_bar_size = { cur_bar_size.x * _size_multiplier, cur_bar_size.y };
	bar->SetSize(new_bar_size);

}

void BallSpeedUpBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar){
	
	auto cur_speed = ball->GetSpeed();
	Vector2f new_speed = { cur_speed.x * _speed_multiplier,
						 cur_speed.y * _speed_multiplier };
	ball->SpeedUp(new_speed - cur_speed);

}

void StickToBarBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar) {
	bar->SetColor(sf::Color::Magenta);
	bar->SetBallStick(true);
}


void BotReflectBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar) {

	ball->SetBottomReflection(true);
	ball->setFillColor(sf::Color::Cyan);
}

void ChangeBallDirBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar) {
	
	ball->SetRandReflection(true);
	ball->setFillColor(sf::Color::Red);

}

void MovingBlockBonus::Activate(std::shared_ptr<Field> field, std::shared_ptr<Ball> ball,
	std::shared_ptr<Bar> bar) {

	field->SpawnMovingBlock();
}