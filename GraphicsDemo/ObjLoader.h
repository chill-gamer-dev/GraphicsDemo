// ObjLoader.h

#pragma once

#include "RenderObject.h"

namespace ObjLoader
{
	bool LoadObjFile(std::filesystem::path const & filepath, RenderObject & render_object);
}
