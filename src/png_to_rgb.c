#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "png.h"
#include "png_to_rgb.h"

typedef struct {
	void *data_addr;
	int32_t data_len;
	int32_t offset;
} mem_data_t;

static void png_read_func(png_structp user_data, png_bytep buffer, png_size_t read_count)
{
	mem_data_t *data_pack = png_get_io_ptr(user_data);

    if ((ssize_t)read_count + data_pack->offset > data_pack->data_len) {
    	read_count = data_pack->data_len - data_pack->offset;
    }

    memcpy(buffer, (uint8_t *)data_pack->data_addr + data_pack->offset, read_count);
    data_pack->offset += read_count;

}

int png_convert_2_rgb(const uint8_t *png_data, int32_t png_data_len, png_convert_info *info)
{
	int m_width, m_height;
	uint8_t *rgb;
	png_structp png_ptr;
	png_infop info_ptr;
	int channels;
	png_bytepp row_pointers;

    mem_data_t data_pack = {0};

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);
    data_pack.data_addr = (void *)png_data;
    data_pack.data_len = png_data_len;
	png_set_read_fn(png_ptr, (void *)&data_pack, png_read_func);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, NULL);
	m_width = png_get_image_width(png_ptr, info_ptr);
	m_height = png_get_image_height(png_ptr, info_ptr);
	/*
	 * number of channels of info for thecolor type (valid values are
	 * 1 (GRAY,PALETTE), 2 (GRAY_ALPHA), 3 (RGB),4 (RGB_ALPHA or RGB + filler byte)
	 */
	channels = png_get_channels(png_ptr, info_ptr);
	if(channels != 1 && channels != 3 && channels != 4) {
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		printf("[png]convert_2_rgb unsupport channel:%d\n", channels);
		return -1;
	}

	info->rgb_data_len = m_height * m_width * 4;
	info->rgb_data = malloc(info->rgb_data_len);

	rgb= info->rgb_data;
	row_pointers = png_get_rows(png_ptr, info_ptr);

	if (channels == 1)
	{
		for(int i = 0; i < m_height; i++)
		{
			uint8_t *row_pointer = row_pointers[i];
			for(int j = 0; j < m_width; j ++)
			{
				rgb[0] = row_pointer[0]; // blue
				rgb[1] = row_pointer[0]; // green
				rgb[2] = row_pointer[0]; // red
				rgb[3] = -1;
				rgb += 4;
				row_pointer += channels;
			}
		}
	}
	else
	for(int i = 0; i < m_height; i++)
	{
		uint8_t *row_pointer = row_pointers[i];
		for(int j = 0; j < m_width; j ++)
		{
			rgb[0] = row_pointer[2]; // blue
			rgb[1] = row_pointer[1]; // green
			rgb[2] = row_pointer[0]; // red
			if(channels == 4)
				rgb[3] = row_pointer[3]; // alpha
			else
				rgb[3] = -1;
			rgb += 4;
			row_pointer += channels;
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	info->width = m_width;
	info->height = m_height;
	//printf("[png]width:%d height:%d\n", m_width, m_height);
    return 0;
}


int32_t png_convert_2_rgb_filepath(const char *file_path, png_convert_info *info)
{
	int fd = open(file_path, O_RDONLY);
	uint8_t *png_data;
	int png_data_len = 0, gif_buff_len = 0;
	int32_t rev;

	if (fd < 0) {
		printf("[gif]open gif file err\n");
		return -1;
	}
	png_data = NULL;
	while (1) {
		int ob;
		if (png_data_len + 4096 > gif_buff_len) {
			gif_buff_len = png_data_len + 4096;
			png_data = realloc(png_data, gif_buff_len);
		}
		ob = read(fd, png_data + png_data_len, 4096);
		if (ob  < 0) {
			printf("[gif]read gif file err\n");
			break;
		} else if (ob > 0) {
			png_data_len += ob;
		} else {
			break;
		}
	}
	close(fd);

	rev = png_convert_2_rgb(png_data, png_data_len, info);
	free(png_data);
	if (rev < 0) {
//		free(png_data);
		return -1;
	}
#if 0
	printf("png w:%d h:%d c:%d rgb:%p len:%fKB\n",
			info->width, info->height, 1,
			info->rgb_data, info->rgb_data_len / 1024.0);
#endif
	return 0;
}

void test_png(void) {
	png_convert_info info;
	png_convert_2_rgb_filepath("/root/workspace/fonts/test.png", &info);
}

int32_t png_convert_rgb_2_png(char* png_file_name, char*  pixels, int32_t width,
                                            int32_t height, int32_t bit_depth)
{
	png_structp png_ptr;  
	png_infop info_ptr;  
	FILE *png_file = fopen(png_file_name, "wb");  
	if (!png_file)
	{
		printf("ERROR:png open fail\n"); 
		return -1;
	}
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
	if(png_ptr == NULL)  
	{  
		printf("ERROR:png_create_write_struct\n"); 
		fclose(png_file);
		return -2;  
	}  
	info_ptr = png_create_info_struct(png_ptr);  
	if(info_ptr == NULL)  
	{  
		printf("ERROR:png_create_info_struct\n");  
		fclose(png_file);
		png_destroy_write_struct(&png_ptr, NULL);  
		return -3;  
	}  
	png_init_io(png_ptr, png_file);  
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
                    PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE); 
 
 
	png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
	if (!palette) {
		fclose(png_file);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return -4;
	}
	png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);  
	png_write_info(png_ptr, info_ptr);  
	png_set_packing(png_ptr);
	//这里就是图像数据了  
	png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
	for (int i = 0; i < height; ++i)
	{
		rows[i] = (png_bytep)(pixels + (i) * width * 4);
	}
 
	png_write_image(png_ptr, rows);  
	png_free(png_ptr, rows);  
	png_write_end(png_ptr, info_ptr);  
	png_free(png_ptr, palette);  
	palette=NULL;  
	png_destroy_write_struct(&png_ptr, &info_ptr);  
	fclose(png_file);  
	return 0;  
}

