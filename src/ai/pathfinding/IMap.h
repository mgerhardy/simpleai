#pragma once

namespace ai {

class IMap {
public:
	virtual ~IMap ()
	{
	}

	virtual bool isBlocked (const AIPosition& pos) const = 0;
};

}
