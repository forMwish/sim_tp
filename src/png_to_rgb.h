/*
 * png_to_rgb.h
 *
 *  Created on: May 15, 2020
 *      Author: root
 */

#ifndef LV_EXAMPLES_LV_TUTORIAL_90_LYTEST_PNG_TO_RGB_H_
#define LV_EXAMPLES_LV_TUTORIAL_90_LYTEST_PNG_TO_RGB_H_


typedef struct {
	int32_t width;
	int32_t height;
	uint8_t *rgb_data;
	int32_t rgb_data_len;
} png_convert_info;

int png_convert_2_rgb(const uint8_t *png_data, int32_t png_data_len, png_convert_info *info);
int32_t png_convert_2_rgb_filepath(const char *file_path, png_convert_info *info);
int32_t png_convert_rgb_2_png(char* png_file_name, char*  pixels, int32_t width,
                                            int32_t height, int32_t bit_depth);
#endif /* LV_EXAMPLES_LV_TUTORIAL_90_LYTEST_JPG_TO_RGB_H_ */
