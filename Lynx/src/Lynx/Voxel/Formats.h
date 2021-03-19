#pragma once
#include <vector>

namespace Lynx {
	struct XRAWHeader {
		char type[4];
		unsigned char color_channel_data_type;
		unsigned char num_color_channels;
		unsigned char bits_per_channel;
		unsigned char bits_per_index;
		int width;
		int height;
		int depth;
		int num_palette_clr;
	};

	struct XRAWData {
		std::vector<unsigned int> index;
		std::vector<unsigned int> palette;
	};

	struct XRAW {
		XRAWHeader header;
		XRAWData data;
	};
}