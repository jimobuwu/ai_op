#include "pch.h"
#include "Conv.h"
#include <iostream>

std::vector<float> cnn::getConv_simple(std::vector<float>input, int input_width, int input_height,
	std::vector<float>conv, int kernel_width, int kernel_height) {

	const int out_w = input_width - kernel_width + 1;
	const int out_h = input_height - kernel_height + 1;

	const int conv_len = kernel_width * kernel_height;
	const int input_len = input_width * input_height;

	std::vector<float> tmp(out_w * out_h * conv_len, 0);

	int wh = 0;
	for (int i = 0; i < out_h; ++i) {
		for (int j = 0; j < out_w; ++j) {
			wh = (i * out_w + j) * conv_len;

			int start = 0;
			for (int m = 0; m < kernel_height; ++m) {
				start = ( i + m ) * input_width + j;
				for (int n = 0; n < kernel_width; ++n) {
					tmp[wh + m * kernel_width + n] = input[start + n];
				}
			}
		}
	}

	int sum = 0;
	for (int i = 0, j = 0; i < out_w * out_h * conv_len; ++i, ++j) {
		if (j == conv_len) {
			j = 0;
			std::cout << " sum: " << sum << std::endl;
			sum = 0;
		}
		std::cout << tmp[i] << ' ';
		sum += tmp[i];
	}
	std::cout << " sum: " << sum << std::endl;
	std::cout << std::endl;

	std::vector<float> res;
	for (int i = 0; i < out_w; ++i) {
		for (int j = 0; j < out_h; ++j) {
			int conv_value = 0;
			wh = (i * out_h + j) * conv_len;
			for (int m = 0; m < conv_len; ++m) {
				conv_value += tmp[wh + m] * conv[m];
			}
			res.push_back(conv_value);
		}
	}

	return res;
}