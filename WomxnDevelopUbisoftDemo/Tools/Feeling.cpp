#include "stdafx.h"

#include "Feeling.hpp"

sf::Color FeelingColor::GetFeelingColor(Feeling feeling) const
{
    return FeelingColor::feelingsColor.at(feeling);
}

std::map<Feeling, sf::Color> FeelingColor::feelingsColor = [] {
    std::map<Feeling, sf::Color> feelingColors;

    feelingColors.emplace(Feeling::Neutral, sf::Color::Black);
    feelingColors.emplace(Feeling::Compassion, sf::Color::Green);
    feelingColors.emplace(Feeling::Fear, sf::Color::Blue);
    feelingColors.emplace(Feeling::Rage, sf::Color::Red);

    return feelingColors;
}();
