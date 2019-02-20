#pragma once

template<typename TypeObj>
class StatusTpl
{
public:
	StatusTpl(int type_ = 0, StatusTpl *next_ = nullptr, StatusTpl *prev_ = nullptr) :
		type(type_), next(next_), prev(prev_)
	{}
	virtual ~StatusTpl() {}
	inline StatusTpl* nextStatus() { return next; }
	inline StatusTpl* prevStatus() { return prev; }

	virtual void enter(TypeObj obj) = 0;

	template<typename T>
	inline T getType() { return static_cast<T>(type); }

protected:
	StatusTpl *next = nullptr;
	StatusTpl *prev = nullptr;
	int type = 0;
};
