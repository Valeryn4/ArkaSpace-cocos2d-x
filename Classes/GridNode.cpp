#include "GridNode.h"
#include "ObjectNode.h"

#include <algorithm>

USING_NS_CC;


bool GridNode::init()
{
	setSizeGrid(1, 1);
	
	return true;
}

void GridNode::setSizeGrid(int colls, int rows)
{
	
	if (!grid.empty())
	{
		std::vector<ObjectNode*> findValideObject;

		for (auto &&v : grid)
		{
			if (v == nullptr)
				continue;

			
			if (std::find(findValideObject.begin(), findValideObject.end(), v) == findValideObject.end());
			{
				findValideObject.push_back(v);
			}
		}

		for (auto &&v : findValideObject)
			v->removeFromParentAndCleanup(true);

	}

	
	grid.clear();
	grid.resize(colls * rows, nullptr);

	coll = colls;
	row = rows;

}

bool GridNode::isPositionFree(int col_, int row_)
{
	if (row_ * coll + col_ >= grid.size())
		return false;

	return grid.at(row_ * coll + col_) == nullptr;
}

bool GridNode::addObjectFromGrid(const int coll_, const int row_, ObjectNode * obj)
{
	auto index = row_ * coll + coll_;
	if (index >= grid.size())
		return false;

	if (grid.at(index) != nullptr)
		return false;

	if (obj == nullptr)
		return false;


	auto size = obj->getSizeFromGrid();

	if (size.col_size == 0 || size.row_size == 0)
		return false;

	if ((size.row_size * row_) * coll + (size.col_size + coll_)  >= grid.size())
		return false;

	ObjectNode::PosGrid pos = { coll_, row_ };

	//chek size;
	int coll_max = coll_ + (size.col_size);
	int row_max = row_ + (size.row_size);
	for (int x = coll_; x < coll_max; x++)
	{
		for (int y = row_; y < row_max; y++)
		{
			if (grid.at(y * coll + x) != nullptr)
				return false;
		}
	}

	//fill size;
	for (int x = coll_; x < coll_max; x++)
	{
		for (int y = row_; y < row_max; y++)
		{
			grid.at(y * coll + x) = obj;
		}
	}
	

	/*SET POSITION*/
	auto conteintSize = this->getContentSize();

	auto top = conteintSize.height;
	auto left = 0;

	auto blockRect = getSizeBlock();

	auto left_interval = pos.col * blockRect.size.width + (blockRect.size.width * (size.col_size) * 0.5f);
	auto top_interval = pos.row * blockRect.size.height + (blockRect.size.height * (size.row_size) * 0.5f);

	auto pos_x = left + left_interval;
	auto pos_y =  top - top_interval;

	auto position = Point(pos_x, pos_y);

	auto obj_size = obj->getContentSize();

	/*SCALED*/
	if (obj_size.width > blockRect.size.width * size.col_size)
	{
		auto scaleX = blockRect.size.width * size.col_size / obj_size.width;
		obj->setScaleX(scaleX);
	}

	if (obj_size.height > blockRect.size.height * size.row_size)
	{
		auto scaleY = blockRect.size.height * size.row_size / obj_size.height;
		obj->setScaleY(scaleY);
	}

	

	obj->setPosition(position);
	obj->fixed_position = position;
	obj->setPositionFromGrid(coll_, row_);
	obj->setGrid(this);
	//obj->setAnchorPoint(-Vec2::ANCHOR_MIDDLE);

	this->addChild(obj); // ADD CHILD

#if defined(_DEBUG)

	/*
	auto draw = DrawNode::create();
	draw->drawPoint(position, 10, Color4F::WHITE);
	draw->drawPoint(position, 8, Color4F::BLACK);
	this->addChild(draw, 10);
	this->debugDraw.push_back(draw);
	*/

#endif

	return true;
}

ObjectNode * GridNode::getObjectFromGrid(int coll_, int row_)
{
	auto index = row_ * coll + coll_;

	if (index >= grid.size())
		return nullptr;

	auto obj = grid.at(index);

	return obj;
}

bool GridNode::removeObjectFromGrid(int coll_, int row_)
{
	auto index = row_ * coll + coll_;
	if (index >= grid.size())
		return false;

	auto obj = grid.at(index);

	if (obj == nullptr)
		return false;

	for (auto &&v : grid)
	{
		if (v == obj)
		{
			v = nullptr;
		}
	}

	obj->removeFromParentAndCleanup(true);
	return true;
}

bool GridNode::removeObjectFromGrid(ObjectNode * obj)
{
	if (obj == nullptr)
		return false;

	bool find = false;

	for (auto &&v : grid)
	{
		if (v == obj)
		{
			v = nullptr;
			find = true;
		}
	}

	if (find)
		obj->removeFromParentAndCleanup(true);

	return find;
}

cocos2d::Rect GridNode::getSizeBlock()
{
	auto conteintSize = this->getContentSize();
	auto width = conteintSize.width / coll;
	auto height = conteintSize.height / row;

	return cocos2d::Rect(Point(0,0), Size(width, height));
}

std::vector<ObjectNode*> GridNode::getObjectListFromGrid()
{

	std::vector<ObjectNode*> list;
	for (auto &&obj : grid)
	{
		if (obj != nullptr)
		{
			if (std::find(list.begin(), list.end(), obj) == list.end())
				list.push_back(obj);
		}
	}

	return list;
}

cocos2d::Vec2 GridNode::getColRowFromPos(const cocos2d::Point & point)
{
	auto sizeBlock = getSizeBlock();
	auto x_interval = sizeBlock.size.width;
	auto y_interval = sizeBlock.size.height;
	int current_coll = point.x / x_interval;
	int current_row = row - (point.y / y_interval);
	if (current_row < 0)
		current_row = 0;

	if (current_coll < 0)
		current_coll = 0;

	return cocos2d::Vec2(current_coll, current_row);
}

cocos2d::Point GridNode::getPositionFromColRow(int coll_, int row_)
{
	auto sizeBlock = getSizeBlock();
	auto x_interval = sizeBlock.size.width;
	auto y_interval = sizeBlock.size.height;
	auto pos_x = x_interval * coll_;
	auto pos_y = y_interval * (row - row_);

	return cocos2d::Point(pos_x, pos_y);
}

#if defined(_DEBUG)

void GridNode::showDebug()
{
	

	if (inited_debug)
	{
		for (auto &&v : debugDraw)
			v->setVisible(true);

		return;
	}


	auto drawing = [this](Point LB, Point RT) 
	{

		auto draw = DrawNode::create();
		draw->drawRect(LB, RT, Color4F(0.3f, 0.3f, 1.f, 1.0f));
		return draw;	
	};

	auto conteintSize = this->getContentSize();

	auto top = conteintSize.height;
	auto left = 0;

	auto blockRect = getSizeBlock();

	for (int x = 0; x < coll; x++)
	{
		for (int y = 0; y < row; y++)
		{

			auto left_interval = x * blockRect.size.width;// +(blockRect.size.width * size.col_size);
			auto top_interval = y * blockRect.size.height;//  +(blockRect.size.height * size.row_size);

			auto pos_x = left + left_interval;
			auto pos_y = top - top_interval;

			auto LB = Point(pos_x, pos_y - blockRect.size.height);
			auto RB = Point(pos_x + blockRect.size.width, pos_y);

			auto draw = drawing(LB, RB);
			this->addChild(draw);
			this->debugDraw.push_back(draw);

		}
	}

	inited_debug = true;
	
}

void GridNode::hideDebug()
{
	if (inited_debug)
	{
		for (auto &&v : debugDraw)
			v->setVisible(false);

		return;
	}
}

#endif
