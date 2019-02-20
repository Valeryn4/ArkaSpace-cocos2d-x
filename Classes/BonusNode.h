#pragma once

#include <cocos2d.h>

enum class BonusType : int
{
	None,
	Fire,
	Small,
	Big,
	ShortWidth,
	LongWodth,
	Split,
	Slow,
	Fast
};

class BonusNode : public cocos2d::Node
{
public:
	bool init() override;

	void activate();
	void deactivate();
	void destroy();

	inline bool isActivated() { return m_activate; }
	inline BonusType getType() { return m_type; }

	std::function<void()> onActivated = nullptr;
	std::function<void()> onDeactivated = nullptr;

	static BonusNode* create(BonusType type);
private:
	CREATE_FUNC(BonusNode);

	BonusType m_type;
	bool m_activate = false;
};
