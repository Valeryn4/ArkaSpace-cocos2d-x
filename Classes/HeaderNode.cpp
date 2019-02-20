#include "HeaderNode.h"
#include "Defines.h"

USING_NS_CC;

bool HeaderNode::init(const cocos2d::Size & size)
{
	this->setContentSize(size);

	auto width_block  = size.width  / 3;
	auto height_block = size.height;
	auto size_block = Size(width_block, height_block);

	Vec2 left_bottom = { 0, 0 };//size / 2 * -1;
	Vec2 right_top = size; //size / 2;

	auto block_1 = Rect(left_bottom.x,						left_bottom.y, width_block, height_block);
	auto block_2 = Rect(left_bottom.x + width_block,		left_bottom.y, width_block, height_block);
	auto block_3 = Rect(left_bottom.x + (width_block * 2),	left_bottom.y, width_block, height_block);


	
	scoreText = Label::createWithTTF("0", FONT_CALCULATOR, block_3.size.height * 0.4, block_3.size);
	scoreText->setAnchorPoint({ 0,0 });
	scoreText->setHorizontalAlignment(TextHAlignment::RIGHT);
	scoreText->setPosition(block_3.origin + Vec2(0, block_3.size.height * 0.05));
	scoreText->setColor(Color3B::WHITE);
	scoreText->enableOutline(Color4B::BLUE, 1);
	this->addChild(scoreText);

#if defined(_DEBUG)

	auto draw = DrawNode::create();
	draw->drawSolidRect(left_bottom, right_top, Color4F(1, 0, 0, 0.3f));
	this->addChild(draw, -100);
	debugRectDraw = draw;
#endif

	return true;
}

void HeaderNode::update(float delta)
{
	this->update();
}

void HeaderNode::update()
{
	scoreText->setString(std::to_string(score));
}

#if defined(_DEBUG)
void HeaderNode::showDebug()
{
	debugRectDraw->setVisible(true);
}

void HeaderNode::hideDebug()
{
	debugRectDraw->setVisible(false);
}

#endif