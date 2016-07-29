#include "TextureLoader.h"

#include <gli\gli.hpp>

void
TextureLoader::load(std::string fileName, VkFormat format)
{
	gli::texture2d tex2D(gli::load(fileName));
}