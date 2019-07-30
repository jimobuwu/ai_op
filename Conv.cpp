// Conv.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Conv.h"
#include <cmath>
#include <cassert>

std::vector<float> cnn::getConv(const std::vector<float> &input, float input_width, float input_height,
	const std::vector<float> &conv, float kernel_width, float kernel_height,
	float stride_row, float stride_col, cnn::padding pad_type, float padding_w, float padding_h) {

	assert(input_width > kernel_width, "input_width must equal or greater than kernel_width");
	assert(input_height > kernel_height, "input_height must equal or greater than kernel_height");

	int input_w_new = input_width;
	int input_h_new = input_height;

	int out_w = 0;
	int out_h = 0;	
	
	int padding_w_l = 0;
	int padding_w_r = 0;
	int padding_h_l = 0;
	int padding_h_r = 0;

	std::vector<float> input_new(input);

	if (pad_type == cnn::padding::same) {
		if (1 == stride_row) {
			padding_w_l = padding_w_r = std::ceil((kernel_width - 1) / 2);
			padding_w = padding_w_l + padding_w_r;
			out_w = input_width;
		}
		else {
			// 可以整除步长的最小整数padding
			padding_w = std::ceil(std::ceil((input_width - kernel_width) / stride_row) * stride_row - (input_width - kernel_width));
			
			if (0 == padding_w & 1) {
				// 偶数padding，左右一半
				padding_w_l = padding_w_r = padding_w / 2;
			}
			else {
				// 奇数padding，左奇右偶
				padding_w_l = std::ceil(padding_w / 2);
				padding_w_r = padding_w - padding_w_l;
			}
			
			out_w = (input_width - kernel_width + padding_w) / stride_row + 1;
		}

		if (1 == stride_col) {
			padding_h = std::ceil((kernel_height - 1) / 2);
			padding_h = padding_h_l + padding_h_r;		
			out_h = input_height;
		}
		else {
			padding_h = std::ceil(std::ceil((input_height - kernel_height) / stride_col) * stride_col - (input_height - kernel_height));
			if (0 == padding_h & 1) {
				padding_h_l = padding_h_r = padding_h / 2;
			}
			else {
				padding_h_l = std::ceil(padding_h / 2);
				padding_h_r = padding_h - padding_h_l;
			}
			out_h = (input_height - kernel_height + padding_h) / stride_col + 1;
		}

		input_w_new += padding_w;
		input_h_new += padding_h;

		int len_new = input_w_new * input_h_new;
		input_new = std::vector<float>(len_new, 0);
		int index = 0;
		for (int i = padding_h_l; i < input_h_new - padding_h_r; ++i) {
			for (int j = padding_w_l; j < input_w_new - padding_w_r; ++j) {
				input_new[i * input_w_new + j] = input[index];
				++index;
			}
		}
	} else {
		// N = ( W - F + 2P ) / S + 1
		out_w = (input_width - kernel_width + 2 * padding_w) / stride_row + 1;
		out_h = (input_height - kernel_height + 2 * padding_h) / stride_col + 1;
	}

	const int conv_len = kernel_width * kernel_height;
	std::vector<float> tmp(out_w * out_h * conv_len, 0);

	// flatten and save
	int wh = 0;
	for (int i = 0; i < out_h; ++i) {
		for (int j = 0; j < out_w; ++j) {
			wh = (i * out_w + j) * conv_len;

			int start = 0;
			for (int m = 0; m < kernel_height; ++m) {
				start = (i * stride_col + m) * input_w_new + j * stride_row;

				for (int n = 0; n < kernel_width; ++n) {
					if(pad_type == cnn::padding::same) 
						tmp[wh + m * kernel_width + n] = input_new[start + n];
					else
						tmp[wh + m * kernel_width + n] = input[start + n];
				}
			}
		}
	}

	// print
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
	
	printf("shape of input: (%d, %d) \n", (int)input_width, (int)input_height);
	printf("shape of ouput: (%d, %d) \n", out_w, out_h);

	// dot product

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


int main()
{
	std::vector<float> input = { 1,1,1,1,2,1,
								1,3,1,1,2,1,
								1,3,1,1,2,1,
								1,3,1,1,1,1, 
								1,1,1,1,1,1 };

	std::vector<float> conv = { 1,1,1,
								1,1,1,
								1,1,1 };

	//std::vector<float> conv = { 1,1,1 };
	

	int input_w = 6;
	int input_h = 5;
	int conv_w = 3;
	int conv_h = 3;
	int stride_row = 2;
	int stride_col = 2;
	cnn::padding pad_type = cnn::padding::same;	

	std::vector<float> output = cnn::getConv(input, 
											input_w, 
											input_h, 
											conv, 
											conv_w, 
											conv_h, 
											stride_row, 
											stride_col,
											pad_type
											);

	//std::vector<float> output = cnn::getConv_simple(input, input_w, input_h, conv, conv_w, conv_h);

	/*int output_w = (input_w - conv_w + 2 * padding) / stride_row + 1;
	int output_h = (input_h - conv_h + 2 * padding) / stride_col + 1;*/
	
	int output_w = input_w;
	int output_h = input_h;

	for (int i = 0; i < output.size(); ++i) {
		if (i % output_w == 0)
			std::cout << std::endl;
		std::cout << output[i] << ' ';
	}
}

