#include "BonusNode.h"

bool BonusNode::init()
{
	return true;
}

void BonusNode::activate()
{
	if (onActivated)
		onActivated();
	m_activate = true;
}

void BonusNode::deactivate()
{
	if (onDeactivated)
		onDeactivated();

	m_activate = false;
}

void BonusNode::destroy()
{
	if (m_activate)
		deactivate();

	this->removeFromParentAndCleanup(true);
}

BonusNode * BonusNode::create(BonusType type)
{
	auto node = create();
	node->m_type = type;
	return node;
}
