#include <conditions/True.h>

namespace ai {

True::Factory True::FACTORY;

std::ostream& True::print(std::ostream& stream, int level) const {
	stream << _name;
	return stream;
}

}
