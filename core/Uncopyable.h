#pragma once

class Uncopyable
{
public:
	virtual ~Uncopyable() = default;
protected:
	Uncopyable() {}
private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator =(const Uncopyable&) = delete;
};
