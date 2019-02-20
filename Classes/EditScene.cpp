
#if defined(_DEBUG)

#include "Defines.h"

#include "EditScene.h"
#include "GridNode.h"
#include "LevelData.h"
#include "ContainerNode.h"
#include "ScreenFormat.h"

#include "ui\CocosGUI.h"
#include "UISpacePack.hpp"
#include "UISpriteSheet.hpp"

USING_NS_CC;
using namespace ui;
using namespace TexturePacker;

bool EditScene::init()
{
	if (!this->initWithPhysics())
		return false;

	this->getPhysicsWorld()->setGravity(Vec2(0, -1000));

	auto director = Director::getInstance();
	director->setDisplayStats(false);

	auto gameZone = ScreenFormat::getGameZoneRect();
	auto bottomZone = ScreenFormat::getBottomRect();
	

	grid = GridNode::create();
	grid->setContentSize(gameZone.size);
	grid->setAnchorPoint({ 0,0 });
	grid->setPosition(gameZone.origin);
	grid->setSizeGrid(COLL_GRID, ROW_GRID);
	grid->showDebug();


	this->addChild(grid);

	if (!initLevel())
		return false;

	if (!initPanelObject())
		return false;

	if (!initStatusObject())
		return false;

	if (!initMouseEvent())
		return false;

	return true;
}

bool EditScene::initMouseEvent()
{
	auto sizeBlock = grid->getSizeBlock();
	auto drawCursor = DrawNode::create();
	grid->addChild(drawCursor);

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = [sizeBlock, drawCursor, this](EventMouse* event)
	{
		auto pos = Point(event->getCursorX(), event->getCursorY());
		
		auto gameZone = ScreenFormat::getGameZoneRect();
		if (gameZone.containsPoint(pos))
		{
			auto pos_grid = grid->getColRowFromPos(this->grid->convertToNodeSpace(pos));
			auto pos_node = grid->getPositionFromColRow(pos_grid.x, pos_grid.y);

			drawCursor->clear();
			drawCursor->drawSolidRect(pos_node, Vec2(sizeBlock.size.width, sizeBlock.size.height * -1) + pos_node, Color4F(0.3f, 1.0f, 0.3f, 0.5f));
			this->mousePointFromGrid = pos_node;

			if (this->fakeContainer != nullptr)
			{
				auto width = this->fakeContainer->getContentSize().width;
				auto height = this->fakeContainer->getContentSize().height;
				auto offsetX = this->memoryContainerCfg.coll == 1 ? width / 2 : width;
				auto offsetY = 0; //this->memoryContainerCfg.row == 1 ? height / 2 : height;

				if (this->memoryContainerCfg.rotation == ContainerNode::RotationContainer::HORIZONTAL)
				{
					offsetX = this->memoryContainerCfg.row == 1 ? width / 2 : width;
					offsetY = this->memoryContainerCfg.coll == 1 ? height / 2 : height;
				}

				if (this->memoryContainerCfg.row == 1 && this->memoryContainerCfg.coll == 1)
				{
					offsetY = height / 2;
				}

				this->fakeContainer->setPosition(pos_node + Vec2(offsetX, offsetY));
			}

			if (this->labelGridInfo != nullptr)
			{
				auto cont = this->grid->getObjectFromGrid(pos_grid.x, pos_grid.y);

				if (cont != nullptr)
				{
					if (cont->getTypeObjectId() == CONTAINER_NAME)
					{
						auto container = reinterpret_cast<ContainerNode*>(cont);
						auto hp = container->getHP();
						auto exp = container->getExp();
						this->labelGridInfo->setString("CURSOR:\nhp: " + std::to_string(hp) +
							"\nexp:" + std::to_string(exp));

					}
				}
				else
				{
					this->labelGridInfo->setString("CURSOR:\nEMPTY");
				}
			}
		}
	};

	listener->onMouseDown = [sizeBlock, this](EventMouse* event)
	{
		auto pos = Point(event->getCursorX(), event->getCursorY());

		auto gameZone = ScreenFormat::getGameZoneRect();
		if (gameZone.containsPoint(pos))
		{
			auto pos_grid = grid->getColRowFromPos(this->grid->convertToNodeSpace(pos));
			auto pos_node = grid->getPositionFromColRow(pos_grid.x, pos_grid.y);

			if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
				if (memoryContainerCfg.init && fakeContainer != nullptr && spriteMouse != nullptr && !blockBuild)
				{
					auto cont = ContainerNode::create(
						memoryContainerCfg.exp,
						memoryContainerCfg.hp,
						memoryContainerCfg.spriteID,
						memoryContainerCfg.rotation,
						memoryContainerCfg.colorID
					);

					if (!grid->addObjectFromGrid(pos_grid.x, pos_grid.y, cont))
					{

						CCLOG("ERROR!");
					}
				}

			if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
			{
				this->grid->removeObjectFromGrid(pos_grid.x, pos_grid.y);
			}
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool EditScene::initClickedEvent()
{
	return false;
}

bool EditScene::initKeyEvent()
{
	return false;
}

//Суть этого гавнокода!
//Оно создает интерфейс в шапке, с выбором блока. Нажатие кнопки сохраняет в памяти выбранный тип блока.
//А еще я ленивое говно, я не хочу пилить анимацию, т.к. этот код не для продакашена
bool EditScene::initPanelObject()
{
	using SpriteID = ContainerNode::SpriteID;
	using ColorID = ContainerNode::COLOR;
	using ContRot = ContainerNode::RotationContainer;


	auto headerZone = ScreenFormat::getHeaderRect();

	auto scrollHeader = ScrollView::create();
	scrollHeader->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollHeader->setContentSize(headerZone.size);
	scrollHeader->setAnchorPoint({ 0,0 });
	scrollHeader->setPosition(headerZone.origin);
	
	auto drawHeader = DrawNode::create();
	drawHeader->drawRect(Vec2(0.f,0.f), scrollHeader->getContentSize(), Color4F::GREEN);
	scrollHeader->addChild(drawHeader);

	auto initButtonContColor = [headerZone, this](SpriteID spriteID, ColorID color) -> Button*
	{
		auto button = Button::create();
		auto sizeH = headerZone.size.height * 0.8f;
		button->setScale9Enabled(false);
		button->setContentSize(Size(sizeH, sizeH));
		button->ignoreContentAdaptWithSize(false);
		auto sprite = Sprite::createWithSpriteFrameName(ContainerNode::getPullSpriteName().at(spriteID));
		
		auto scale = sizeH / std::max(sprite->getContentSize().height, sprite->getContentSize().width);
		sprite->setScale(scale * 0.8f);
		sprite->setAnchorPoint({ 0.5f, 0.5f });
		//sprite->setPosition(button->getContentSize() / 2);
		sprite->setColor(ContainerNode::getMapColor().at(color));
		
		button->addChild(sprite);
		sprite->setPosition(Size(sizeH, sizeH) / 2);

		if (this->memoryContainerCfg.rotation == ContRot::HORIZONTAL)
			sprite->setRotation(90.f);

		auto drawBorder = DrawNode::create();
		drawBorder->drawRect(Vec2(0.f, 0.f), button->getContentSize(), Color4F::MAGENTA);
		button->addChild(drawBorder);

		button->addTouchEventListener([this, color](Ref* sender, Widget::TouchEventType type) -> void 
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				this->memoryContainerCfg.colorID = color;
				this->updateStatusBlock();
				CCASSERT(this->colorWidget != nullptr, "addTouchEventListener error! colorWidget is nullptr!");

				this->colorWidget->removeFromParentAndCleanup(true);
				this->colorWidget = nullptr;
				blockBuild = false;
				
			}
			break;
			default:
				break;
			}
		});

		return button;
	};

	auto initColorWidget = [headerZone, initButtonContColor, this](SpriteID spriteID) -> ScrollView*
	{
		auto scrollWidget = ScrollView::create();
		this->blockBuild = true;
		scrollWidget->setDirection(ScrollView::Direction::HORIZONTAL);
		scrollWidget->setContentSize(headerZone.size);
		scrollWidget->setAnchorPoint({ 0,0 });
		scrollWidget->setPosition(headerZone.origin - Point(0, headerZone.size.height));
		auto sizeH = headerZone.size.height;

		if (sizeH * int(ColorID::COUNT) > headerZone.size.width)
		{
			scrollWidget->setInnerContainerSize(Size(sizeH * int(ColorID::COUNT), headerZone.size.height));
		}

		for (int i = 0; i < int(ColorID::COUNT); ++i)
		{
			auto button = initButtonContColor(spriteID, ColorID(i));
			button->setAnchorPoint({ 0,0 });
			auto pos_x = (i * sizeH) + (sizeH * 0.1f);
			auto pos_y = sizeH * 0.1f;
			auto posButton = Point(pos_x, pos_y);

			button->setPosition(posButton);
			scrollWidget->addChild(button);
		}


		CCLOG("Create color widget");

		return scrollWidget;

	};

	auto initButtonContSprite = [initColorWidget, headerZone, this](SpriteID spriteID, ContRot rot, int pos) -> Button*
	{
		auto button = Button::create();
		auto sizeH = headerZone.size.height * 0.8f;
		button->setScale9Enabled(false);
		button->setContentSize(Size(sizeH, sizeH));
		button->ignoreContentAdaptWithSize(false);

		auto sprite = Sprite::createWithSpriteFrameName(ContainerNode::getPullSpriteName().at(spriteID));

		auto scale = sizeH / std::max(sprite->getContentSize().height, sprite->getContentSize().width);
		sprite->setScale(scale * 0.8f);
		sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		
		button->addChild(sprite);

		sprite->setPosition(Size(sizeH, sizeH) / 2);

		if (rot == ContRot::HORIZONTAL)
			sprite->setRotation(90.f);

		button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		auto pos_x = (pos * sizeH) + (sizeH * 0.1f);
		auto pos_y = sizeH * 0.1f;
		auto posButton = Point(pos_x, pos_y);

		button->setPosition(posButton);
		
		
		button->addTouchEventListener([this, spriteID, initColorWidget, rot](Ref* sender, Widget::TouchEventType type) {

			CCLOG("TAP button in initButtonContSprite");
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				if (this->colorWidget != nullptr)
					this->colorWidget->removeFromParentAndCleanup(true);
				this->memoryContainerCfg.spriteID = spriteID;
				this->memoryContainerCfg.rotation = rot;
				this->colorWidget = initColorWidget(spriteID);

				switch (spriteID)
				{
				case SpriteID::BLUE_BIG:

					this->memoryContainerCfg.coll = 2;
					this->memoryContainerCfg.row = 2;
					break;
				case SpriteID::BLUE_MEDIUM:
					this->memoryContainerCfg.coll = 1;
					this->memoryContainerCfg.row = 2;

					break;
				case SpriteID::BLUE_SMALL:
				case SpriteID::BLUE_MK2_SMALL:

					this->memoryContainerCfg.coll = 1;
					this->memoryContainerCfg.row = 1;
					break;

				default:
					break;
				}

				this->memoryContainerCfg.init = true;
				this->updateStatusBlock();
				this->addChild(this->colorWidget);
			}
				break;
			default:
				break;
			}
		});

		auto drawBorder = DrawNode::create();
		drawBorder->drawRect(Vec2(0.f, 0.f), button->getContentSize(), Color4F::MAGENTA);
		button->addChild(drawBorder);

		CCLOG("Create button %i", pos);
		return button;
	};

	int pos = 0;
	scrollHeader->addChild(initButtonContSprite(SpriteID::BLUE_SMALL, ContRot::VERTICAL, pos++));
	scrollHeader->addChild(initButtonContSprite(SpriteID::BLUE_MK2_SMALL, ContRot::VERTICAL, pos++));
	scrollHeader->addChild( initButtonContSprite(SpriteID::BLUE_MEDIUM, ContRot::VERTICAL,  pos++) );
	scrollHeader->addChild( initButtonContSprite(SpriteID::BLUE_MEDIUM, ContRot::HORIZONTAL,pos++) );
	scrollHeader->addChild( initButtonContSprite(SpriteID::BLUE_BIG,    ContRot::VERTICAL,  pos++) );
	


	//TEST
	auto heightElem =  headerZone.size.height * 0.8f;
	if (pos * heightElem > headerZone.size.width)
	{
		scrollHeader->setInnerContainerSize(Size(pos * heightElem, headerZone.size.height));
	}

	this->addChild(scrollHeader);

	return true;
}

bool EditScene::initStatusObject()
{
	auto bottomRect = ScreenFormat::getBottomRect();
	auto leftRect = Rect(
		bottomRect.origin.x,
		bottomRect.origin.y,
		bottomRect.size.height,
		bottomRect.size.height
	);

	auto centerRect = Rect(
		leftRect.getMaxX(),
		bottomRect.origin.y,
		bottomRect.size.width - (bottomRect.size.height * 2.f) - leftRect.size.width,
		bottomRect.size.height
	);


	auto draw = DrawNode::create();
	draw->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	draw->drawRect(centerRect.origin, Vec2(centerRect.getMaxX(), centerRect.getMaxY()), Color4F::GRAY);
	draw->drawRect(leftRect.origin, Vec2(leftRect.getMaxX(), leftRect.getMaxY()), Color4F::RED);
	this->addChild(draw);

	auto sliderHP = Slider::create();
	sliderHP->loadBarTexture(UISpriteSheet::grey_sliderHorizontal, Widget::TextureResType::PLIST); // как будет выглядеть Slider
	sliderHP->loadSlidBallTextures(UISpacePack::dotWhite, UISpacePack::dotRed, UISpacePack::dot_shadow, Widget::TextureResType::PLIST);
	sliderHP->loadProgressBarTexture(UISpriteSheet::grey_sliderHorizontal, Widget::TextureResType::PLIST);
	sliderHP->setMaxPercent(40);

	sliderHP->addEventListener([this](Ref* sender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			auto slider = reinterpret_cast<Slider*>(sender);
			auto precent = slider->getPercent();

			this->memoryContainerCfg.hp = precent;
			if (this->labelInfo != nullptr)
				this->labelInfo->setString(
					"exp:" + std::to_string(memoryContainerCfg.exp) + "\n" +
					"hp :" + std::to_string(memoryContainerCfg.hp));
		}
	});

	sliderHP->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			this->updateStatusBlock();
			break;
		default:
			break;
		}
	});

	sliderHP->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	auto scale = centerRect.size.width * 0.8f / sliderHP->getContentSize().width;
	sliderHP->setScale(scale);
	sliderHP->setPosition(centerRect.origin + Vec2(centerRect.size.height * 0.2f, centerRect.size.height * 0.2f));
	this->addChild(sliderHP);

	//============================

	auto sliderEXP = Slider::create();
	sliderEXP->loadBarTexture(UISpriteSheet::grey_sliderHorizontal, Widget::TextureResType::PLIST); // как будет выглядеть Slider
	sliderEXP->loadSlidBallTextures(UISpacePack::dotWhite, UISpacePack::dotGreen, UISpacePack::dot_shadow, Widget::TextureResType::PLIST);
	sliderEXP->loadProgressBarTexture(UISpriteSheet::grey_sliderHorizontal, Widget::TextureResType::PLIST);
	sliderEXP->setMaxPercent(100);

	sliderEXP->addEventListener([this](Ref* sender, Slider::EventType type) {
		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			auto slider = reinterpret_cast<Slider*>(sender);
			auto precent = slider->getPercent();

			this->memoryContainerCfg.exp = precent * 10;

			if (this->labelInfo != nullptr)
				this->labelInfo->setString(
					"exp:" + std::to_string(memoryContainerCfg.exp) + "\n" +
					"hp :" + std::to_string(memoryContainerCfg.hp));
		}
	});

	sliderEXP->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			this->updateStatusBlock();
			break;
		default:
			break;
		}
	});

	sliderEXP->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//auto scale = centerRect.size.width * 0.8f / sliderEXP->getContentSize().width;
	sliderEXP->setScale(scale);
	sliderEXP->setPosition(Point(centerRect.origin.x + (centerRect.size.height * 0.2f), centerRect.getMaxY() - (centerRect.size.height * 0.2f)));

	this->addChild(sliderEXP);

	//============

	labelGridInfo = Label::createWithTTF("", FONT_CALCULATOR, 25);
	labelGridInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//labelGridInfo->setAlignment(TextHAlignment::LEFT);
	labelGridInfo->setContentSize(leftRect.size);
	labelGridInfo->setPosition(leftRect.getMinX(), leftRect.getMaxY());
	this->addChild(labelGridInfo);

	return true;
}

bool EditScene::initLevel()
{
	return true;
}

bool EditScene::updateStatusBlock()
{
	static auto bottomRect = ScreenFormat::getBottomRect();
	static auto rightRect = Rect(
		bottomRect.getMaxX() - (bottomRect.size.height * 2.f),
		bottomRect.origin.y,
		bottomRect.size.height * 2.f,
		bottomRect.size.height
	);

	if (statusNode != nullptr)
	{
		statusNode->removeFromParentAndCleanup(true);
		statusNode = nullptr;
	}

	auto node = Node::create();
	node->setContentSize(rightRect.size);
	node->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	node->setPosition(rightRect.origin);

	statusNode = node;
	this->addChild(node);


	auto draw = DrawNode::create();
	draw->drawRect(Vec2(0, 0), node->getContentSize(), Color4F::ORANGE);
	node->addChild(draw);


	auto sprite = Sprite::createWithSpriteFrameName(ContainerNode::getPullSpriteName().at(memoryContainerCfg.spriteID));
	node->addChild(sprite);

	auto scale = rightRect.size.height / std::max(sprite->getContentSize().width, sprite->getContentSize().height);
	sprite->setScale(scale * 0.8);

	if (memoryContainerCfg.rotation == ContainerNode::RotationContainer::HORIZONTAL)
		sprite->setRotation(90);

	sprite->setColor(ContainerNode::getMapColor().at(memoryContainerCfg.colorID));

	sprite->setPosition(
		rightRect.size.width - (rightRect.size.height / 2),
		rightRect.size.height / 2
	);

	//====================
	if (spriteMouse != nullptr)
		spriteMouse->removeFromParentAndCleanup(true);

	auto gridSizeBlock = grid->getSizeBlock();

	if (fakeContainer == nullptr)
	{
		fakeContainer = Node::create();
		fakeContainer->setContentSize(gridSizeBlock.size);
		fakeContainer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		grid->addChild(fakeContainer);
	}

	spriteMouse = Sprite::createWithSpriteFrameName(ContainerNode::getPullSpriteName().at(memoryContainerCfg.spriteID));
	auto spriteMouseScaleX = (gridSizeBlock.size.width * memoryContainerCfg.coll ) / spriteMouse->getContentSize().width;
	auto spriteMouseScaleY = (gridSizeBlock.size.height * memoryContainerCfg.row ) / spriteMouse->getContentSize().height;

	spriteMouse->setColor(ContainerNode::getMapColor().at(memoryContainerCfg.colorID));
	spriteMouse->setScaleX(spriteMouseScaleX);
	spriteMouse->setScaleY(spriteMouseScaleY);
	spriteMouse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	fakeContainer->addChild(spriteMouse);

	if (memoryContainerCfg.rotation == ContainerNode::RotationContainer::HORIZONTAL)
		spriteMouse->setRotation(90);

	if (labelInfo == nullptr)
	{
		labelInfo = Label::createWithTTF(
			"exp:" + std::to_string(memoryContainerCfg.exp) + "\n" +
			"hp :" + std::to_string(memoryContainerCfg.hp),
			FONT_CALCULATOR, 25
		);

		labelInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		labelInfo->setPosition(rightRect.origin);
		this->addChild(labelInfo);
	}
	

	return true;
}



#endif