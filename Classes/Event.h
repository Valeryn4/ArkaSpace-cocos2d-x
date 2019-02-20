#pragma once

#include <cocos2d.h>
#include <string>
#include <functional>
#include <typeinfo>

template<auto ID, typename TData>
class EventTpl :public cocos2d::Event
{
	static_assert(std::is_same<decltype(ID), std::string> || std::is_same<decltype(ID), const char*>);
public:
	EventTpl() :
		cocos2d::Event(cocos2d::Event::Type::CUSTOM)
	{}

	EventTpl(const TData &data) :
		cocos2d::Event(cocos2d::Event::Type::CUSTOM)
		_data(data)
	{}

	EventTpl(const EventTpl &copy)
	{
		_data = copy._data;
	}

	EventTpl(EventTpl &&move)
	{
		_data = move._data;
	}

	EventTpl& operator=(const EventTpl &copy)
	{
		this->_data = copy._data;
		return *this;
	}

	inline TData getData() { return _data; }
	inline void setData(const TData &data) { data_ = data; }
protected:
	TData _data;
};

template<auto ID, typename TData>
class EventListenerTpl : public cocos2d::EventListener
{
	static_assert(std::is_same<decltype(ID), std::string> || std::is_same<decltype(ID), const char*>);
public:
	using EventType = EventTpl<ID, TData>;

	EventListenerTpl() :
		onEvent(nullptr)
	{ }

	static EventListenerTpl* create()
	{
		auto ret = new (std::nothrow) EventListenerTpl();
		if (ret && ret->init())
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
		}
		return ret;
	}

	bool init()
	{
		auto listener = [this](cocos2d::Event *event_)
		{
			auto event = static_cast<EventType*>(event_);
			if (this->onEvent != nullptr)
				this->onEvent(event);
		};

		if (cocos2d::EventListener::init(cocos2d::Event::Type::CUSTOM, ID, listener))
		{
			return true;
		}
	}

	std::function<void(EventType*)> onEvent;
};


template<auto ID, typename TData>
class EventFabricTpl
{
public:

	static_assert(std::is_same<decltype(ID), std::string> || std::is_same<decltype(ID), const char*>);

	using ListenerType = EventListenerTpl<ID, TData>;
	using EventType = ListenerType::EventType;
	static ListenerType* createLister()
	{
		auto ret = new (std::nothrow) ListenerType();
		if (ret && ret->init())
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
		}
		return ret;
	}

	static EventType* createEvent(const TData &data)
	{
		//auto event = new (std::nothrow) EventType(data);
		auto ret = new (std::nothrow) EventType(data);
		if (ret)
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
		}
		return ret;
	}
};