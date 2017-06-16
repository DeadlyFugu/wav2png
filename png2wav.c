/*
	Copyright (c) 2017 Matthew Turner
	This software is provided 'as-is', without any express or implied warranty.
	In no event will the authors be held liable for any damages arising from
	the use of this software.
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
	     claim that you wrote the original software. If you use this software
	     in a product, an acknowledgment in the product documentation would be
	     appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not
	     be misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
*/

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SK_WAV_WRITE_IMPLEMENTATION
#include "sk_wav_write.h"

void p2w_print_usage() {
	printf(
			"usage:\n"
					"png2wav <mode> <source.png> [samplerate]\n\n"
					"  <mode> is either:\n"
					"    bw: black/white, use average pixel intensity as sample value\n"
					"    rgb: store 3 samples per pixel, one per channel\n\n"
					"  [height] is the sample rate of the audio file in hertz, defaults to 44100Hz\n"
	);
}

#define WP_MODE_BW 1
#define WP_MODE_RGB 2

int main(int argc, char** argv) {
	// check correct amount of args
	if (argc == 1) {
		p2w_print_usage();
		return 0;
	}
	if (argc == 2 || argc > 4) {
		printf("error: invalid usage");
		p2w_print_usage();
		return 0;
	}

	// get mode
	int mode = 0;
	int channels = 0;
	if (strcmp(argv[1], "bw") == 0) {
		mode = WP_MODE_BW;
		channels = 1;
	} else if (strcmp(argv[1], "rgb") == 0) {
		mode = WP_MODE_RGB;
		channels = 3;
	} else {
		printf("error: unknown mode '%s'\n", argv[1]);
		p2w_print_usage();
		return 0;
	}

	// get path
	char* path = argv[2];

	// get sample rate
	int sample_rate = 44100;
	if (argc > 3) {
		sample_rate = atoi(argv[3]);
		if (sample_rate == -1) {
			printf("error: invalid sample rate '%s'\n", argv[3]);
			return 0;
		}
	}

	// open file
	int img_width = 0;
	int img_height = 0;
	int img_channels = 0;
	unsigned char* img_data = stbi_load(path, &img_width, &img_height, &img_channels, 3);
	if (!img_data) {
		printf("error: could not open file '%s'\n", path);
		return 0;
	}

	int sample_count = img_width * img_height; // todo: handle other modes

	printf("info: image dimensions are %dx%d\n", img_width, img_height);

	// allocate sample data
	unsigned char* samples = malloc(sample_count);

	// fill image data
	for (int i = 0; i < sample_count; i++) {
		if (mode == WP_MODE_BW) {
			samples[i] = ((int) img_data[i*3] + img_data[i*3+1] + img_data[i*3+2]) / 3;
		} else if (mode == WP_MODE_RGB) {
		}
	}

	// close file
	stbi_image_free(img_data);

	// write wav
	sk_wav_write_file("output.wav", 2, sample_rate, sample_count, samples);

	printf("done\n");

	return 0;
}