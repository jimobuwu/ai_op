#pragma once

#ifndef _CONV_H_
#define _CONV_H_

#include <stdio.h>
#include <vector>

namespace cnn {
	enum class padding {
		same,
		valid
	};

	std::vector<float> getConv_simple(std::vector<float>input, int input_width, int input_height,
		std::vector<float>conv, int kernel_width, int kernel_height);

	std::vector<float> getConv(const std::vector<float> &input, float input_width, float input_height,
		const std::vector<float> &conv, float kernel_width, float kernel_height,
		float stride_row = 1, float stride_col = 1, padding pad_type = cnn::padding::valid, float padding_w = 0, float padding_h = 0);
}


#endif /* _CONV_H_ */