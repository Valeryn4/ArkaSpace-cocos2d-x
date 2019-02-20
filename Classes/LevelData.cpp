#include "LevelData.h"
#include "GridNode.h"
#include "ContainerNode.h"
#include "Defines.h"

USING_NS_CC;

bool LevelData::toLevelFromMap(const cocos2d::ValueMap &map, GridNode *grid)
{
	CCASSERT(!map.empty(), "map is empty");
	CCASSERT(map.at("type").asString() == "GridMap", "map is invalid");

	auto colls_grid = map.at("colls").asInt();
	auto rows_grid = map.at("rows").asInt();

	grid->setSizeGrid(colls_grid, rows_grid);

	auto objectVector = map.at("objectList").asValueVector();

	for (auto &&obj : objectVector)
	{
		auto obj_map = obj.asValueMap();
		
		auto coll = obj_map.at("coll").asInt();
		auto row = obj_map.at("row").asInt();

		auto coll_size = obj_map.at("coll_size").asInt();
		auto row_size = obj_map.at("row_size").asInt();

		auto pos_x = obj_map.at("pos_x").asFloat();
		auto pos_y = obj_map.at("pos_y").asFloat();

		auto rot = obj_map.at("rotation").asFloat();

		auto type_id = obj_map.at("type_id").asString();

		ObjectNode *objectNode = nullptr;

		if (type_id == CONTAINER_NAME)
		{

			auto hp = obj_map.at("hp").asInt();
			auto exp = obj_map.at("exp").asInt();
			auto rotCont = ContainerNode::RotationContainer(obj_map.at("rotationContainer").asInt());
			auto spriteID = ContainerNode::SpriteID( obj_map.at("spriteID").asInt());
			ContainerNode::COLOR colorID = ContainerNode::COLOR::BLUE;
			if (obj_map.find("colorID") != obj_map.end())
				colorID = ContainerNode::COLOR(obj_map.at("colorID").asInt());
			auto contNode = ContainerNode::create(exp, hp, spriteID, rotCont, colorID);
			objectNode = contNode;
		}
		else
		{
			objectNode = ObjectNode::create();
		}

		if (objectNode != nullptr)
		{
			objectNode->setSizeFromGrid(coll_size, row_size);
			objectNode->setRotation(rot);
			grid->addObjectFromGrid(coll, row, objectNode);
		}
	}


	return true;
}

cocos2d::ValueMap LevelData::toMapFromLevel(GridNode * grid)
{
	int colls = grid->getColls();
	int rows = grid->getRows();
	auto vecObjects = grid->getObjectListFromGrid();

	ValueMap gridMap;
	gridMap.emplace("type", std::string("GridMap"));
	gridMap.emplace("colls", colls);
	gridMap.emplace("rows", rows);

	ValueVector objectVector;
	objectVector.resize(vecObjects.size(), Value::Null);

	for (auto &&obj : vecObjects)
	{
		ValueMap objMap;
		auto posFromGrid = obj->getPositionFromGrid();
		auto sizeFromGrid = obj->getSizeFromGrid();

		objMap.emplace("coll", posFromGrid.col);
		objMap.emplace("row", posFromGrid.row);

		objMap.emplace("coll_size", sizeFromGrid.col_size);
		objMap.emplace("row_size", sizeFromGrid.row_size);

		objMap.emplace("pos_x", obj->getPositionX());
		objMap.emplace("pos_y", obj->getPositionY());

		objMap.emplace("rotation", obj->getRotation());

		auto typeId = obj->getTypeObjectId();
		objMap.emplace("type_id", typeId);

		if (typeId == CONTAINER_NAME)
		{
			auto cont = reinterpret_cast<ContainerNode*>(obj);
			auto hp = cont->getHP();
			auto exp = cont->getExp();
			auto rotationCont = cont->getRotationContainer();
			auto spriteID = cont->getSpriteID();
			auto colorID = cont->getColorID();

			objMap.emplace("hp", hp);
			objMap.emplace("exp", exp);
			objMap.emplace("rotationContainer", (int)rotationCont);
			objMap.emplace("spriteID", (int)spriteID);
			objMap.emplace("colorID", (int)colorID);

		}

		objectVector.push_back(Value(objMap));
	}

	gridMap.emplace("objectList", Value(objectVector));

	return gridMap;
}

bool LevelData::loadMapFromFile(const std::string &file, GridNode *grid)
{
	auto map = FileUtils::getInstance()->getValueMapFromFile(file);
	auto res = toLevelFromMap(map, grid);

	return res;
}

bool LevelData::saveMapToFile(GridNode * grid, const std::string &file)
{
	auto map = toMapFromLevel(grid);
	auto res = FileUtils::getInstance()->writeValueMapToFile(map, file);
	
	return res;
}

