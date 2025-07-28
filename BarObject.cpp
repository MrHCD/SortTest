#include "BarObject.h"

BarObject::ColorData BarObject::colorData[(int)Color::NumofColor] = {
    { Color::BULE, 0.2f, 0.6f, 0.8f },  // Blue color
    { Color::RED,  8.0f, 0.6f, 0.2f },  // Red color
    { Color::GRAY, 0.8f, 0.8f, 0.8f }   // Gray color
};

void BarObject::SwitchBars(BarObject &a, BarObject &b)
{
    static BarObject temp ;
    temp = a; // Create a temporary copy of 'a'
    a = b;              // Assign 'b' to 'a'
    b = temp;          // Assign the temporary copy to 'b'
    // After this, 'a' and 'b' have been switched
}

BarObject::BarObject(BarObject const &other)
    : barHeight(other.barHeight), barWidth(other.barWidth), barX(other.barX), barY(other.barY),
      value(other.value)
{
    // Copy constructor implementation
}

BarObject::BarObject()
    : barHeight(0), barWidth(0), barX(0), barY(0), status(Status::NORMAL), barColor(Color::GRAY)
{
}

BarObject::~BarObject()
{
}
