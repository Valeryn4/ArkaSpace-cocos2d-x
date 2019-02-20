#pragma once


#include <cocos2d.h>

class HeaderNode : public cocos2d::Node
{
public:
	bool init(const cocos2d::Size &size);
	
	static HeaderNode* create(const cocos2d::Size &size)
	{
		HeaderNode *pRet = new(std::nothrow) HeaderNode();
		if (pRet && pRet->init(size))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

	inline cocos2d::Sprite* getSprite() { return sprite; }
	inline void addScore(int score_) { score += score_; }
	inline void setScore(int score_) { score = score_; }
	inline int getScore() { return score; }

	void update(float delta);
	void update();

	//CREATE_FUNC(HeaderNode);
#if defined(_DEBUG)
	void showDebug();
	void hideDebug();
#endif
private:
	cocos2d::Sprite* sprite;
	cocos2d::Label* scoreText;
	int score;

#if defined(_DEBUG)
	cocos2d::Node* debugRectDraw;
#endif
};