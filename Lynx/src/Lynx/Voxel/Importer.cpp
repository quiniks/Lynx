#include "Lynxpch.h"
#include "Importer.h"
#include "Lynx/Detail/glm.h"
#include <iostream>
#include <fstream>

namespace Lynx {
	XRAW Importer::XRawImport(const std::string& file)
	{
		LX_CORE_INFO("Importing XRAW {0}", file);
		XRAW xraw;
		/////////  XRAW Header	/////////
		XRAWHeader& h = xraw.header;
		
		std::ifstream fs(file, std::fstream::in | std::fstream::binary);

		if (!fs) {
			LX_CORE_ASSERT(false, "Could not open file");
		}

		fs.read((char*)&h.type, sizeof(h.type));
		fs.read((char*)&h.color_channel_data_type, sizeof(h.color_channel_data_type));
		fs.read((char*)&h.num_color_channels, sizeof(h.num_color_channels));
		fs.read((char*)&h.bits_per_channel, sizeof(h.bits_per_channel));
		fs.read((char*)&h.bits_per_index, sizeof(h.bits_per_index));
		fs.read((char*)&h.width, sizeof(h.width));
		fs.read((char*)&h.height, sizeof(h.height));
		fs.read((char*)&h.depth, sizeof(h.depth));
		fs.read((char*)&h.num_palette_clr, sizeof(h.num_palette_clr));

		/////////	XRAW Data	/////////
		XRAWData& d = xraw.data;

		unsigned int indexBufferSize = h.width * h.height * h.depth;
		unsigned int sizeOfIndices = h.bits_per_index / 8;
		for (unsigned int i = 0; i < indexBufferSize; ++i) {
			d.index.push_back(0);
			fs.read((char*)&d.index.back(), sizeOfIndices);
		}
		
		unsigned int paletteSize = h.num_palette_clr * h.num_color_channels;
		unsigned int sizeofColors = h.bits_per_channel / 8;
		for (unsigned int i = 0; i < paletteSize; ++i) {
			d.palette.push_back(0);
			fs.read((char*)&d.palette.back(), sizeofColors);
		}

		fs.close();

		LX_CORE_INFO("Done");

		return xraw;
	}

	//TODO
	std::vector<Voxel2> Importer::XRAWToVoxel(const XRAW& xraw)
	{
		const XRAWHeader& header = xraw.header;
		const XRAWData& data = xraw.data;

		unsigned int size = data.index.size();
		std::vector<Voxel2> voxels;

		voxels.reserve(size);
		for (size_t i = 0; i < size; ++i) {
			Voxel2 voxel;
			if (data.index.at(i) != 0)
				voxel.m_Type = Voxel::Type::Solid;
			if (header.num_color_channels == 4) {
				glm::vec4 color;
				color.r = data.palette.at(4 * i + 0) / header.;
				color.g = data.palette.at(4 * i + 1);
				color.b = data.palette.at(4 * i + 2);
				color.a = data.palette.at(4 * i + 3);
			}
		}
		return std::vector<Voxel2>();
	}
}
