#pragma once


#include <numeric>
#include <typeinfo>
#include <limits>


template<class EnumT, typename NumT>
struct MaskTpl : public EnumT
{
	using enum_t = EnumT;
	using num_t = NumT;

	union value_t
	{
		enum_t vEnum;
		num_t vNum;
	};

	value_t value;

	/*
		конструкторы
	*/
	MaskTpl(const MaskTpl &copy)
	{
		value.vNum = copy.value.vNum;
	}
	MaskTpl(MaskTpl &&move)
	{
		value.vNum = move.value.vNum;
	}
	MaskTpl(num_t num)
	{
		value.vNum = num;
	}
	MaskTpl(enum_t en)
	{
		value.vEnum = en;
	}

	/*
		функции
	*/
	num_t toNum() { return value.vNum; }
	enum_t toEnum() { return value.vEnum; }


	/*
		операторы присвоения
	*/
	MaskTpl& operator=(const MaskTpl &mask)
	{
		value.vNum = mask.value.vNum;
		return *this;
	}

	MaskTpl& operator=(MaskTpl &&mask)
	{
		value.vNum = mask.value.vNum;
		return *this;
	}

	MaskTpl& operator=(num_t num)
	{
		value.vNum = num;
		return *this;
	}

	MaskTpl& operator=(enum_t en)
	{
		value.vEnum = en;
		return *this;
	}

	/*
		преобразования
	*/
	operator num_t()
	{
		return value.vNum;
	}

	operator enum_t()
	{
		return value.vEnum;
	}

};



template<class EnumT, typename NumT>
MaskTpl<EnumT, NumT> operator&(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum & b.value.vNum);
}

template<class EnumT, typename NumT>
MaskTpl<EnumT, NumT> operator|(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum | b.value.vNum);
}

template<class EnumT, typename NumT>
MaskTpl<EnumT, NumT> operator^(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum ^ b.value.vNum);
}

template<class EnumT, typename NumT>
NumT operator&(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum & b.value.vNum);
}

template<class EnumT, typename NumT>
NumT operator|(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum | b.value.vNum);
}

template<class EnumT, typename NumT>
NumT operator^(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum ^ b.value.vNum);
}

/*
 ===
*/

template<class EnumT, typename NumT>
bool operator==(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum == b.value.vNum);
}

template<class EnumT, typename NumT>
bool operator==(MaskTpl<EnumT, NumT> a, NumT b)
{
	return (a.value.vNum == b);
}

template<class EnumT, typename NumT>
bool operator==(NumT b, MaskTpl<EnumT, NumT> a)
{
	return (a.value.vNum == b);
}


template<class EnumT, typename NumT>
bool operator==(MaskTpl<EnumT, NumT> a, EnumT b)
{
	return (a.value.vEnum == b);
}


template<class EnumT, typename NumT>
bool operator==(EnumT b, MaskTpl<EnumT, NumT> a)
{
	return (a.value.vEnum == b);
}

/*
	!=
*/


template<class EnumT, typename NumT>
bool operator!=(MaskTpl<EnumT, NumT> a, MaskTpl<EnumT, NumT> b)
{
	return (a.value.vNum != b.value.vNum);
}

template<class EnumT, typename NumT>
bool operator!=(MaskTpl<EnumT, NumT> a, NumT b)
{
	return (a.value.vNum != b);
}

template<class EnumT, typename NumT>
bool operator!=(NumT b, MaskTpl<EnumT, NumT> a)
{
	return (a.value.vNum != b);
}


template<class EnumT, typename NumT>
bool operator!=(MaskTpl<EnumT, NumT> a, EnumT b)
{
	return (a.value.vEnum != b);
}


template<class EnumT, typename NumT>
bool operator!=(EnumT b, MaskTpl<EnumT, NumT> a)
{
	return (a.value.vEnum != b);
}