#include <cstring>
#include <assert.h>
#include <iostream>

template <class T>
class MaxQueue
{
public:
	MaxQueue(int size, bool startRight = false)
	{
		_max_size = size;
		_array = new T[size * 2];
		_startRight = startRight;
		memset(_array, 0, sizeof(T) * _max_size * 2);
		if (startRight)
			_current = size;
	}

	~MaxQueue()
	{
		delete[] _array;
	}

	void add(T value)
	{
		if (_current < _max_size) {
			_array[_current++] = value;
		}
		else if (_current >= _max_size && _current < _max_size * 2) {
			_array[_current++] = value;
			++_offset;
		}
		else {
			_current = _max_size - 1;
			_offset = 0;
			memcpy(_array, &_array[_max_size + 1], sizeof(T) * _current);
			_array[_current++] = value;
		}
	}

	void clear()
	{
		memset(_array, 0, sizeof(T) * _max_size * 2);
		_offset = 0;
		if(_startRight)
			_current = _max_size;
		else
			_current = 0;
	}

	T getMax()
	{
		T max = 0;
		for (int i = 0; i < _max_size; ++i)
		{
			if (get(i) > max)
				max = get(i);
		}
		return max;
	}

	T* getStart()
	{
		return &_array[_offset];
	}

	T get(int index)
	{
		assert(index >= 0 || index < _max_size);
		return _array[_offset + index];
	}

	T getLast()
	{
		return get(_max_size - 1);
	}

	int size()
	{
		return _max_size;
	}

private:
	int _current = 0;
	int _max_size = 0;
	int _offset = 0;
	bool _startRight;
	T* _array;
};