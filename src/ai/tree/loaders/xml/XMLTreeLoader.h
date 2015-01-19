#pragma once

#include "tree/loaders/ITreeLoader.h"

namespace ai {

/**
 * @brief Implementation of @c ITreeLoader that gets its data from a xml file
 */
class XMLTreeLoader: public ai::ITreeLoader {
public:
	XMLTreeLoader(const IAIFactory& aiFactory);
	/**
	 * @brief this will initialize the loader once with all the defined behaviours from the given xml data.
	 */
	bool init(const std::string& xmlData);
};

}
