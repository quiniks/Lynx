#include "Lynxpch.h"
#include "Importer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Utility/Util.h"
#include <iostream>
#include <fstream>

namespace Lynx {
	XRAW Importer::XRawImport(const std::string& file)
	{
		LX_PROFILE_FUNCTION();
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
			d.index.emplace_back(0);
			fs.read((char*)&d.index.back(), sizeOfIndices);
		}
		
		unsigned int paletteSize = h.num_palette_clr * h.num_color_channels;
		unsigned int sizeofColors = h.bits_per_channel / 8;
		for (unsigned int i = 0; i < paletteSize; ++i) {
			d.palette.emplace_back(0);
			fs.read((char*)&d.palette.back(), sizeofColors);
		}

		fs.close();

		LX_CORE_INFO("Done");

		return xraw;
	}

	void Importer::XRAWToVoxel(const XRAW& xraw, World& world)
	{
		LX_PROFILE_FUNCTION();
		const XRAWHeader& header = xraw.header;
		const XRAWData& data = xraw.data;

		unsigned int XtotalChunks = (unsigned int)std::ceilf((float)header.width / Chunk::SIZE);
		unsigned int YtotalChunks = (unsigned int)std::ceilf((float)header.depth / Chunk::SIZE);
		unsigned int ZtotalChunks = (unsigned int)std::ceilf((float)header.height / Chunk::SIZE);
		unsigned int totalChunks = XtotalChunks * YtotalChunks * ZtotalChunks;
		world.m_Size.x = XtotalChunks;
		world.m_Size.y = YtotalChunks;
		world.m_Size.z = ZtotalChunks;

		{
			LX_PROFILE_SCOPE("reserve chunks");
			world.m_Chunks.clear();
			world.m_Chunks.reserve(totalChunks);
			auto initChunks = [&](const glm::uvec3& chunkPos) {
				world.m_Chunks.emplace_back(Chunk{ chunkPos });
			};
			//xraw height/y axis is the z axis
			LoopXYZ(initChunks, { XtotalChunks, YtotalChunks, ZtotalChunks });
		}

		auto convertData = [&](const glm::uvec3& voxelXRAWPos) {
			//for (int z = 0; z < header.height; ++z) {
			//	for (int y = 0; y < header.depth; ++y) {
			//		for (int x = 0; x < header.width; ++x) {
						//Position data
			glm::vec3 voxelWorldPos = { voxelXRAWPos.z, voxelXRAWPos.y, voxelXRAWPos.x };
			glm::ivec3 chunkPos = glm::floor(voxelWorldPos / (float)Chunk::SIZE);
			glm::uvec3 voxelLocalPos;
			voxelLocalPos.x = (unsigned int)voxelWorldPos.x % Chunk::SIZE;
			voxelLocalPos.y = (unsigned int)voxelWorldPos.y % Chunk::SIZE;
			voxelLocalPos.z = (unsigned int)voxelWorldPos.z % Chunk::SIZE;

			//unsigned int index = IndexLinear(chunkPos.z, chunkPos.y, chunkPos.x, world.m_Size.x, world.m_Size.y);
			unsigned int index = world.ChunkIndexFromPos(chunkPos);
			Chunk& chunk = world.m_Chunks.at(index);
			Voxel& voxel = chunk.m_Voxels.at(Chunk::VoxelIndexFromPos(voxelLocalPos));

			//Set voxel type
			size_t paletteIndex = data.index.at(PaletteIndexFromPos(voxelXRAWPos.x, voxelXRAWPos.z, voxelXRAWPos.y, header.width, header.height));
			if (paletteIndex != 0) {
				LX_PROFILE_SCOPE("set");
				voxel.SetType(Voxel::Type::Solid);
				//chunk.SetVoxelType(vcpX, vcpY, vcpZ, Voxel::Type::Solid, false);
			}

			//Set voxel color
			glm::vec4 color{ 0.0f };
			if (header.num_color_channels == 4) {
				float depth = pow(2, header.bits_per_channel);
				color.r = data.palette.at(4 * paletteIndex + 0) / (depth - 1);
				color.g = data.palette.at(4 * paletteIndex + 1) / (depth - 1);
				color.b = data.palette.at(4 * paletteIndex + 2) / (depth - 1);
				color.a = data.palette.at(4 * paletteIndex + 3) / (depth - 1);
			}
			else if (header.num_color_channels == 3) {
				float depth = pow(2, header.bits_per_channel);
				color.r = data.palette.at(3 * paletteIndex + 0) / (depth - 1);
				color.g = data.palette.at(3 * paletteIndex + 1) / (depth - 1);
				color.b = data.palette.at(3 * paletteIndex + 2) / (depth - 1);
				color.a = 1.0f;
			}
			voxel.SetColor(color);
			//chunk.SetVoxelColor(vcpX, vcpY, vcpZ, color);
//		}
//	}
//}
		};
		LoopXYZ(convertData, { header.height, header.depth, header.width });
	}

	/*
	int Importer::IndexLinear(int x, int y, int z, int size)
	{
		int a = size * size;	//Z * Y
		int b = size;			//Z
		return a * z + b * y + x;
	}
	*/

	int Importer::PaletteIndexFromPos(int x, int y, int z, int width, int height)
	{
		int a = width * height;	//Z * Y
		int b = width;			//Z
		return a * z + b * y + x;
	}
}
