#pragma once

#include "GuiElement.h"
#include "Color.h"

//Size : 156
class Label : public GuiElement {
    public:
    std::string text;            //120
    char filler1[32];            //124
    
    public:
    Label(MinecraftClient&, std::string const&, Color const&, int, int, int, bool);
    virtual ~Label();
    virtual void render(MinecraftClient*, int, int);
    virtual void setupPositions();
    virtual void pointerPressed(MinecraftClient*, int, int);
    virtual void pointerReleased(MinecraftClient*, int, int);
    virtual std::string getText() const;
    virtual void setText(std::string const&);
    virtual void setTextRaw(std::string const&);
    virtual void setWidth(int);
    virtual void setColor(Color const&);
	void setCentered(bool);
};
