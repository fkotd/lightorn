#include "stdafx.h"

#include "Feeling.hpp"

sf::Color FeelingColor::GetFeelingColor(Feeling feeling) const
{
    return FeelingColor::feelingsColor.at(feeling);
}

std::map<Feeling, sf::Color> FeelingColor::feelingsColor = [] {
    std::map<Feeling, sf::Color> feelingColors;

    feelingColors.emplace(Feeling::Neutral, sf::Color { 140, 130, 215, 100 });
    feelingColors.emplace(Feeling::Compassion, sf::Color { 130, 221, 191, 100 });
    feelingColors.emplace(Feeling::Fear, sf::Color { 130, 205, 221, 100 });
    feelingColors.emplace(Feeling::Rage, sf::Color { 221, 130, 197, 100 });

    return feelingColors;
}();
