#include <conditions/False.h>

namespace ai {

False::Factory False::FACTORY;

std::ostream& False::print(std::ostream& stream, int level) const {
	stream << _name;
	return stream;
}

}
