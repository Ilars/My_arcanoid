#pragma once
#include "Bonus.hpp"

class BonusManager
{

private:
    std::vector <std::shared_ptr <Bonus>> _bonuses; 

public:
    BonusManager() { LoadTexture(); };
    ~BonusManager() {};

    std::vector<sf::Texture> _textures;

    void LoadTexture();

    void AddRandomBonus(const sf::Vector2f& pos);

    void Draw(std::shared_ptr<sf::RenderWindow> window);

    void MoveAll(std::shared_ptr <Bar> bar, std::shared_ptr <Field> field,
        std::shared_ptr <Ball> ball);

};

