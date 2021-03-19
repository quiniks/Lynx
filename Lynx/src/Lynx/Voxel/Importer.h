#pragma once
#include "Lynx/Voxel/Formats.h"
#include "Lynx/Voxel/Voxel.h"

namespace Lynx {
	class Importer {
	public:
		static XRAW XRawImport(const std::string& file);
		static std::vector<Voxel2> XRAWToVoxel(const XRAW& xrawData);
	};
}