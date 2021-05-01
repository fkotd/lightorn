#pragma once

#include <SFML/Graphics/Color.hpp>
#include <map>

enum Feeling {
    Neutral,
    Compassion,
    Fear,
    Rage,
    FEELINGS_NUMBER
};

class FeelingColor {
public:
    sf::Color GetFeelingColor(Feeling feeling) const;

private:
    static std::map<Feeling, sf::Color> feelingsColor;
};
