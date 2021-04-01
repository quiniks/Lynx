#pragma once
#include "Lynx/Voxel/Formats.h"
#include "Lynx/Voxel/VoxelStructure.h"

namespace Lynx {
	class Importer {
	public:
		static XRAW XRawImport(const std::string& file);
		static void XRAWToVoxel(const XRAW& xrawData, World& world);
	private:
		//static int IndexLinear(int x, int y, int z, int size);
		static int PaletteIndexFromPos(int x, int y, int z, int width, int height);
	};
}