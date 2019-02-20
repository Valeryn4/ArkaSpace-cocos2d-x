#include "ObjectNode.h"
#include "Defines.h"
#include "GridNode.h"
USING_NS_CC;

bool ObjectNode::init()
{
	setPositionFromGrid(0, 0);
	setSizeFromGrid(1, 1);
	return true;
}

void ObjectNode::setPositionFromGrid(int col, int row)
{
	pointFromGrid.col = col;
	pointFromGrid.row = row;
}

void ObjectNode::setSizeFromGrid(int col_size, int row_size)
{
	sizeFromGrid.col_size = col_size;
	sizeFromGrid.row_size = row_size;
}

void ObjectNode::removeFromGridAndCleanup(ObjectNode* obj)
{
	obj->grid->removeObjectFromGrid(obj);
}
