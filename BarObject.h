#include <cstddef>

class BarObject
{
public:
    enum class Color
    {
        BULE,
        RED,
        GRAY,
        NumofColor
    };

    enum class Status
    {
        HIGHLIGHTED,
        SORTED,
        NORMAL
    };

    struct ColorData
    {
        Color color;
        float r, g, b; // RGB values
    };

private:
    /* data */
    unsigned barHeight;
    unsigned barWidth;
    unsigned barX;
    unsigned barY;  

    unsigned value; // 柱状图的值

    Status status; // 柱状图的状态

    Color barColor; // 柱状图颜色 

public:

    static ColorData colorData[(int)Color::NumofColor];

    // Getters and Setters for bar properties
    BarObject& setBarHeight(unsigned height) { barHeight = height; return *this; }
    unsigned getBarHeight() const { return barHeight; }

    BarObject& setBarWidth(unsigned width) { barWidth = width; return *this; }
    unsigned getBarWidth() const { return barWidth; }

    BarObject& setBarX(unsigned x) { barX = x; return *this; }
    unsigned getBarX() const { return barX; }

    BarObject& setBarY(unsigned y) { barY = y; return *this; }  
    unsigned getBarY() const { return barY; }

    BarObject& setValue(unsigned val) { value = val; return *this; }
    unsigned getValue() const { return value; }

    BarObject& setStatus(Status newStatus) { status = newStatus; return *this; }
    Status getStatus() const { return status; }

    BarObject& setBarColor(Color color) { barColor = color; return *this; }
    Color getBarColor() const { return barColor; }


public:

    void * operator new(size_t size)
    {
        return ::operator new(size);
    }

    void* operator new[](size_t size)
    {
        return ::operator new[](size);
    }

    void operator delete(void *ptr)
    {
        ::operator delete(ptr);
    }

    void operator delete[](void *ptr)
    {
        ::operator delete[](ptr);
    }

    bool operator < (BarObject &other)
    {
        // Define the less than operator for BarObject based on value
        return this->value < other.value;
    }

    bool operator > (BarObject &other)
    {
        // Define the greater than operator for BarObject based on value
        return this->value > other.value;
    }

    // Static function to switch two BarObject instances
    static void SwitchBars(BarObject &a, BarObject &b);

    BarObject(BarObject const &other);
    BarObject(/* args */);
    ~BarObject();
};

