#include "Game.hpp"
#include <iostream>
using namespace sf;

const sf::Vector2f Ball::DEFAULT_BALL_SPEED = sf::Vector2f(3.7f, 3.7f);
const int Ball::RANDOM_REFLECT_CHANCE = 10;


Ball::Ball(float rad, const sf::Vector2f& pos,const sf::Color& col) {
	
	setRadius(rad);
	setPosition(pos);
	setFillColor(col);
	_stick_to_board = false;
	_reflects_bottom = false;
	_random_reflection = false;
	_start_coord = pos;
	_speed = DEFAULT_BALL_SPEED;
	_move_dir = { -1,-1 };
}


Vector2f Ball::GetTopCoord() const {
	auto pos = getPosition();
	return { pos.x + getRadius(),pos.y  };
}

Vector2f Ball::GetLeftCoord() const {
	auto pos = getPosition();
	return { pos.x ,pos.y + getRadius() };
}

Vector2f Ball::GetRightCoord() const {
	auto pos = getPosition();
	auto rad = getRadius();
	return { pos.x + 2 * rad, pos.y +  rad};
}


Vector2f Ball::GetBottomCoord() const {
	auto pos = getPosition();
	auto rad = getRadius();
	return { pos.x + rad,pos.y + 2 * rad};
}



void Ball::Move() {
	if (_stick_to_board)
		MoveWithBar();
	else {
		if (_random_reflection)
			RandomlyReflect();

		move({ _move_dir.x * _speed.x, _move_dir.y * _speed.y });
	}
	
}

void Ball::RandomlyReflect() {
	if (rand() % 100 < RANDOM_REFLECT_CHANCE) {
		if (rand() % 2) 
			_move_dir.y *= -1;
		else	
			_move_dir.x *= -1;
		setFillColor(sf::Color::Blue);
		_random_reflection = false;
	}
}


void Ball::ReflectBar(std::shared_ptr<Bar> bar) {
	auto ball_pos = getPosition();
	auto rad = getRadius();
	auto bar_pos = bar->GetCoord();
	auto bar_size = bar->GetSize();

	bool y_lower_bars_top = abs(ball_pos.y + 2 * rad - bar_pos.y) < _speed.y;
	bool x_to_right_from_bar_left = ball_pos.x + 2 * rad >= bar_pos.x;
	bool x_to_left_from_bar_right = ball_pos.x <= bar_pos.x + bar->GetSize().x;

	bool right_upper_angle = fabs(ball_pos.x - (bar_pos.x + bar_size.x)) < 0 &&
		fabs(ball_pos.y + 2 * rad - bar_pos.y) < 0;
	bool left_upper_angle = fabs(ball_pos.x + 2 * rad - bar_pos.x) < 0 &&
		fabs(ball_pos.y + 2 * rad - bar_pos.y) < 0;

	if (y_lower_bars_top && x_to_right_from_bar_left && x_to_left_from_bar_right) {
		_move_dir.y *= -1;
		if (left_upper_angle || right_upper_angle) {
			_move_dir.x *= -1;
		}
		if (bar->IsBallStick()) {
			_stick_to_board = true;
			setPosition(ball_pos.x, _start_coord.y);
			bar->SetColor(sf::Color::Blue);
		}
	}


}


void Ball::MoveWithBar() {
	auto x_coord = getPosition().x;

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		if (x_coord >= 0)
			move({ -Bar::DEFAULT_BAR_SPEED, 0 });
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		if (x_coord + 2 * getRadius() <= Game::GetWindowSize().x)
			move({ Bar::DEFAULT_BAR_SPEED, 0 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
		_stick_to_board = false;

}

void Ball::Respawn() {
	setPosition(_start_coord.x, _start_coord.y);
	_speed = DEFAULT_BALL_SPEED;
	_move_dir = { -1,-1 };
}

void Ball::SpeedUp(const Vector2f& increment) {
		_speed += increment;
}

bool Ball::ReflectFromBlock(const Block& block) {
	bool hit = false;
	auto ball_pos = getPosition();
	auto block_pos = block.getPosition();
	auto radius = getRadius();
	auto block_size = block.getSize();
	auto ball_speed = GetSpeed();

	float max_dist = sqrt(_speed.y * _speed.y + _speed.x * _speed.x);

	//bool y_more_blocks_bot = fabs(ball_pos.y - (block_pos.y + block_size.y)) < max_dist;
	//bool y_less_blocks_top = fabs(ball_pos.y + 2 * radius - block_pos.y) < max_dist;
	//bool x_more_blocks_left = fabs(ball_pos.x + 2 * radius - block_pos.x) < max_dist;
	//bool x_less_blocks_right = fabs(ball_pos.x - (block_pos.x + block_size.x)) < max_dist;

	//bool y_more_blocks_bot_strict = ball_pos.y + 2 * radius >= block_pos.y;
	//bool y_less_blocks_top_strict = ball_pos.y <= block_pos.y + block_size.y;
	//bool x_more_blocks_left_strict = ball_pos.x >= block_pos.x;
	//bool x_less_blocks_right_strict = ball_pos.x + 2 * radius <= block_pos.x + block_size.x;

	bool right_lower_angle = fabs(ball_pos.x - (block_pos.x + block_size.x)) < max_dist &&
		fabs(ball_pos.y - (block_pos.y + block_size.y)) < max_dist;
	bool right_upper_angle = fabs(ball_pos.x - (block_pos.x + block_size.x)) < max_dist &&
		fabs(ball_pos.y + 2 * radius - block_pos.y) < max_dist;
	bool left_upper_angle = fabs(ball_pos.x + 2 * radius - block_pos.x) < max_dist &&
		fabs(ball_pos.y + 2 * radius - block_pos.y) < max_dist;
	bool left_lower_angle = fabs(ball_pos.x + 2 * radius - block_pos.x) < max_dist &&
		fabs(ball_pos.y - (block_pos.y + block_size.y)) < max_dist;

	bool lower_side = fabs(ball_pos.y - (block_pos.y + block_size.y)) < max_dist &&
		ball_pos.x + 2*radius > block_pos.x &&
		ball_pos.x < block_pos.x + block_size.x;
	bool upper_side = fabs(ball_pos.y + 2 * radius - block_pos.y) < max_dist &&
		ball_pos.x + 2 * radius > block_pos.x &&
		ball_pos.x < block_pos.x + block_size.x;
	bool left_side = fabs(ball_pos.x + 2 * radius - block_pos.x) < max_dist &&
		ball_pos.y + 2 * radius > block_pos.y &&
		ball_pos.y < block_pos.y + block_size.y;
	bool right_side = fabs(ball_pos.x - (block_pos.x + block_size.x)) < max_dist &&
		ball_pos.y + 2 * radius > block_pos.y &&
		ball_pos.y < block_pos.y + block_size.y;

	 if (lower_side) {
		_move_dir.y *= -1;
		hit = true;
	}

	else if (upper_side) {
		_move_dir.y *= -1;
		hit = true;
	}

	else if (left_side) {
		_move_dir.x *= -1;
		hit = true;
	}

	else if (right_side) {
		_move_dir.x *= -1;
		hit = true;
	}

	else if (right_lower_angle ) {
		_move_dir.x *= -1;
		_move_dir.y *= -1;
		hit = true;
	}

	else if (right_upper_angle) {
		_move_dir.x *= -1;
		_move_dir.y *= -1;
		hit = true;
	}

	else if (left_upper_angle) {
		_move_dir.x *= -1;
		_move_dir.y *= -1;
		hit = true;
	}

	else if (left_lower_angle) {
		_move_dir.x *= -1;
		_move_dir.y *= -1;
		hit = true;
	}


	return hit;
}


void Ball::ReflectWall() {
	auto pos = getPosition();

	bool fell_left = pos.x <= _speed.x;
	bool fell_right = fabs(pos.x + 2 * getRadius() - _speed.x) >= Game::GetWindowSize().x;
	bool fell_Top = pos.y <= _speed.y;
	bool fell_bot = fabs(pos.y + 2 * getRadius() - Game::GetWindowSize().y) <= _speed.y;

	if (fell_left || fell_right)
		_move_dir.x *= -1;
	if (fell_Top)
		_move_dir.y *= -1;
	if (_reflects_bottom && fell_bot) {
		_move_dir.y *= -1;
		_reflects_bottom = false;
		setFillColor(sf::Color::Blue);
	}

}
