#pragma once
#include "Lynx/Render/FreeCamera.h"
#include "Lynx/Utility/BitMask.h"
#include "Lynx/Detail/glm.h"

namespace Lynx {
	/////////////////////
	namespace VoxelBit {
		constexpr int SIX_SIDES = 63;
		constexpr int NO_SIDES = 0;
		constexpr int PX_SIDE = BIT(1);
		constexpr int NX_SIDE = BIT(0);
		constexpr int PY_SIDE = BIT(3);
		constexpr int NY_SIDE = BIT(2);
		constexpr int PZ_SIDE = BIT(5);
		constexpr int NZ_SIDE = BIT(4);
	}

	class Voxel {
	public:
		static constexpr float SIZE = 0.2f;
		enum class Type {
			Empty, Solid,
		};
		void SetColor(const glm::vec4& color) {
			m_Color = glm::packUnorm3x10_1x2(color);
		};
		uint32_t GetColor() {
			return m_Color;
		}
		void SetType(Voxel::Type type) {
			m_Type = type;
		}
		Voxel::Type GetType() {
			return m_Type;
		}
	private:
		Voxel::Type m_Type = Voxel::Type::Empty;
		uint32_t m_Color = 0;
	};

	///////////////////////

	class Chunk {
	public:
		Chunk(const glm::uvec3& chunkPos) {
			m_ChunkPosition = chunkPos;
		}
		static unsigned int VoxelIndexFromPos(const glm::uvec3& pos);

		static const unsigned int SIZE = 16;
		glm::uvec3 m_ChunkPosition{ 0 };
		std::vector<Voxel> m_Voxels;
	};

	///////////////////////

	class World {
	public:
		void LoadXRAW(const std::string& file);
		bool LookingAtVoxel(const FreeCamera& camera, glm::uvec3& vpos);
		void SetVoxel(const glm::uvec3 vpos, Voxel::Type type, const glm::vec4& color);
		bool ValidChunkPos(const glm::uvec3& pos);
		unsigned int ChunkIndexFromPos(const glm::uvec3& pos);

		glm::uvec3 m_Size{0};
		std::vector<Chunk> m_Chunks;
		std::vector<glm::uvec3> m_DirtyChunkPositions;
	};
}