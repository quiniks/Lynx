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

	std::vector<Chunk> Importer::XRAWToVoxel(const XRAW& xraw)
	{
		const XRAWHeader& header = xraw.header;
		const XRAWData& data = xraw.data;

		std::vector<Chunk> chunks;
		unsigned int XtotalChunks = std::ceilf((float)header.width / Chunk::SIZE);
		unsigned int YtotalChunks = std::ceilf((float)header.height / Chunk::SIZE);
		unsigned int ZtotalChunks = std::ceilf((float)header.depth / Chunk::SIZE);
		unsigned int totalChunks = XtotalChunks * YtotalChunks * ZtotalChunks;

		//TODO: chunks need a reference to world/refactor chunk to be non existent
		chunks.resize(totalChunks);

		for (int z = 0; z < header.depth; z++) {
			for (int y = 0; y < header.height; y++) {
				for (int x = 0; x < header.width; x++) {

					//Position data
					glm::vec3 voxelWorldPos = { x, y, z };
					glm::ivec3 chunkPos = glm::floor(voxelWorldPos / (float)Chunk::SIZE);
					Chunk& chunk = chunks.at(IndexLinear(x, y, z, Chunk::SIZE));
					int vcpX = (int)voxelWorldPos.x % Chunk::SIZE;
					int vcpY = (int)voxelWorldPos.y % Chunk::SIZE;
					int vcpZ = (int)voxelWorldPos.z % Chunk::SIZE;

					//Set voxel type
					size_t paletteIndex = data.index.at(IndexLinear(x, y, z, header.width, header.height));
					if (paletteIndex != 0)
						chunk.SetVoxelType(vcpX, vcpY, vcpZ, Voxel::Type::Solid);//voxel.m_Type = Voxel::Type::Solid;
					
					//Set voxel color
					glm::vec4 color;
					if (header.num_color_channels == 4) {
						int depth = pow(2, header.bits_per_channel);
						color.r = data.palette.at(4 * paletteIndex + 0) / depth;
						color.g = data.palette.at(4 * paletteIndex + 1) / depth;
						color.b = data.palette.at(4 * paletteIndex + 2) / depth;
						color.a = data.palette.at(4 * paletteIndex + 3) / depth;
					}
					else if (header.num_color_channels == 3) {
						int depth = pow(2, header.bits_per_channel);
						color.r = data.palette.at(3 * paletteIndex + 0) / depth;
						color.g = data.palette.at(3 * paletteIndex + 1) / depth;
						color.b = data.palette.at(3 * paletteIndex + 2) / depth;
						color.a = 1.0f;
					}
					chunk.SetVoxelColor(vcpX, vcpY, vcpZ, color);
				}
			}
		}

		return chunks;

		/*
		unsigned int size = data.index.size();
		std::vector<Voxel2> voxels;

		voxels.reserve(size);
		for (size_t i = 0; i < size; ++i) {
			Voxel2 voxel;
			glm::vec4 color;
			size_t paletteIndex = data.index.at(i);

			if (paletteIndex != 0)
				voxel.m_Type = Voxel::Type::Solid;

			if (header.num_color_channels == 4) {
				int depth = pow(2, header.bits_per_channel);
				color.r = data.palette.at(4 * paletteIndex + 0) / depth;
				color.g = data.palette.at(4 * paletteIndex + 1) / depth;
				color.b = data.palette.at(4 * paletteIndex + 2) / depth;
				color.a = data.palette.at(4 * paletteIndex + 3) / depth;
			}
			else if (header.num_color_channels == 3) {
				int depth = pow(2, header.bits_per_channel);
				color.r = data.palette.at(3 * paletteIndex + 0) / depth;
				color.g = data.palette.at(3 * paletteIndex + 1) / depth;
				color.b = data.palette.at(3 * paletteIndex + 2) / depth;
				color.a = 1.0f;
			}

			voxel.SetColor(color);
			voxels.push_back(voxel);
		}

		//return voxels;
		*/
	}
	int Importer::IndexLinear(int x, int y, int z, int size)
	{
		int a = size * size;	//Z * Y
		int b = size;			//Z
		return a * z + b * y + x;
	}

	int Importer::IndexLinear(int x, int y, int z, int width, int height)
	{
		int a = width * height;	//Z * Y
		int b = width;			//Z
		return a * z + b * y + x;
	}
}
