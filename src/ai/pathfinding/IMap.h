#pragma once

namespace ai {

class IMap {
public:
	virtual ~IMap ()
	{
	}

	virtual bool isBlocked (const Vector3f& pos) const = 0;
};

}
