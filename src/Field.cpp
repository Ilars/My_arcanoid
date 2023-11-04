#include "Field.hpp"
#include <iostream>
#include "Block.hpp"
#include "Game.hpp"

const int Field::INDESTRCTABLE_BLOCK_SPAWN_CHANCE = 5;
const int Field::SPEEDUP_BLOCK_SPAWN_CHANCE = 18;
const int Field::BONUS_BLOCK_SPAWN_CHANCE = 80;

void Field::Draw(std::shared_ptr<sf::RenderWindow> window) {
	for (auto block : _grid) {
		window->draw(*block);
	}
}


void Field::ReduceHealth(unsigned pos) {
	int health_left = _grid[pos]->ReduceHealth();
	if (health_left == 0) {
		_grid.erase(_grid.begin() + pos);
		std::cout << _grid.size() << std::endl;
	}
}

bool Field::CheckGameEnd() const{
	for (const auto block : _grid) {
		if (!block->IsUndestr())
			return false;
	}
	return true;
}


Field::Field(Game& game, const sf::Vector2f& top,const sf::Vector2u& size,
	const sf::Vector2f& window_size, const sf::Vector2f& block_size): _game(game) {

	_size = size;
	_pos = top;
	float x = top.x;
	float y = top.y;
	 
	for (int i = 0; i < size.y; i++) {
		for (int j = 0; j < size.x; j++) {
			std::shared_ptr<Block> block;
			int block_type = rand() % 100;

			sf::Vector2f pos = { x,y };
			if (block_type < BONUS_BLOCK_SPAWN_CHANCE) {
				block = std::make_shared<BlockWithBonus>
					(BlockWithBonus(_game, block_size, pos));
			}
			else if (block_type < SPEEDUP_BLOCK_SPAWN_CHANCE) {
				block = std::make_shared <SpeedUpBlock>
					(SpeedUpBlock(_game, block_size, pos));
			}
			else if (block_type < INDESTRCTABLE_BLOCK_SPAWN_CHANCE) {
				block = std::make_shared <IndestructableBlock>
					(IndestructableBlock(_game, block_size, pos));
			}
			else {
				block = std::make_shared <Block>
					(Block(_game, block_size, pos));
			}

			_grid.push_back(block);
			x += (block_size.x );
		}
		y += (block_size.y);
		x = top.x;
	}
}



bool Field::CheckXForNewMoving(float x, float y) {
	for (auto block : _grid) {
		if (block->getPosition().y == y && 
			(x + block->getSize().x > block->getPosition().x &&
		 	 x < block->getPosition().x + block->getSize().x ||
			(x + block->getSize().x > Game::WINDOW_SIZE.x)))
				return false;
		
	}

	return true;
}


void Field::CheckCollisionsBetweenBlocks() {
	for (auto block1 : _grid) {
		float block1_left_x = block1->getPosition().x;
		float block1_right_x = block1_left_x + block1->getSize().x;
		float block1_top_y = block1->getPosition().y;
		float block1_bot_y = block1_top_y + block1->getSize().y;

		for (auto block2 : _grid) {

			float block2_left_x = block2->getPosition().x;
			float block2_right_x = block2_left_x + block2->getSize().x;
			float block2_top_y = block2->getPosition().y;
			float block2_bot_y = block1_top_y + block2->getSize().y;


			bool collision_block1_left =
				fabs(block1_left_x - block2_right_x) 
				<= 0;
			bool collision_block2_right = 
				fabs(block1_right_x - block2_left_x)
				<= 0;
			bool same_y_pos = (block1_top_y == block2_top_y);
			
			if (collision_block1_left && same_y_pos)
				block1->SetDirection(1);
			else if (collision_block2_right && same_y_pos )
				block1->SetDirection(-1);
		}
	}
}

void Field::SpawnMovingBlock() {
	float block_width = _grid[0]->getSize().x;
	float block_height = _grid[0]->getSize().y;
	float x_pos;

	float y_pos = _pos.y + 1.05*(_size.y) *  block_height;
	do
		x_pos = (float)(rand() % (int)(Game::WINDOW_SIZE.x - block_width));
	while (!CheckXForNewMoving(x_pos, y_pos));
	auto block = std::make_shared <MovingBlock>
		(_game, sf::Vector2f(block_width, block_height), sf::Vector2f(x_pos, y_pos));
	_grid.push_back(block);
}



void Field::MoveMovingBlocks() {
	for (auto block : _grid)
		block->Move();
}




