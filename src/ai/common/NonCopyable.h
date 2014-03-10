#pragma once

class NonCopyable {
public:
	NonCopyable ()
	{
	}
private:
	NonCopyable (const NonCopyable&);
	NonCopyable& operator= (const NonCopyable&);
};
