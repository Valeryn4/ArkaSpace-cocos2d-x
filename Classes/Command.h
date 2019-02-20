#pragma once

#include <functional>
#include <cocos2d.h>

class Command
{
public:
	Command()
	{}
	virtual ~Command() {}
	virtual void execute() = 0;
	
};

class CommandNull : public Command
{
public:
	void execute() override
	{}
};

template<typename T>
class CommandTpl
{
public:
	virtual ~CommandTpl() {}
	virtual void execute(T obj) = 0;
};


template<typename T>
class CommandTplNull : public CommandTpl<T>
{
public:
	void execute(T obj) override {};
};

class CommandNode
{
public:
	CommandNode() {}
	virtual ~CommandNode() {}
	virtual void execute(cocos2d::Node *) = 0;
};

class CommandNodeNull : public CommandNode
{
public:
	void execute(cocos2d::Node*) override
	{}

};