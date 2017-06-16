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


#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

void w2p_print_usage() {
	printf(
			"usage:\n"
			"wav2png <mode> <source.wav> [height]\n\n"
			"  <mode> is either:\n"
			"    bw: black/white, use average pixel intensity as sample value\n"
			"    rgb: store 3 samples per pixel, one per channel\n\n"
			"  [height] is how many pixels high the image should be\n"
	);
}

#define WP_MODE_BW 1
#define WP_MODE_RGB 2

int main(int argc, char** argv) {
	// check correct amount of args
	if (argc == 1) {
		w2p_print_usage();
		return 0;
	}
	if (argc == 2 || argc > 4) {
		printf("error: invalid usage");
		w2p_print_usage();
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
		w2p_print_usage();
		return 0;
	}

	// get path
	char* path = argv[2];

	// get height
	int height = -1;
	if (argc > 3) {
		height = atoi(argv[3]);
		if (height == -1) {
			printf("error: invalid height '%s'\n", argv[3]);
			return 0;
		}
	}

	// open file
	drwav wav;
	if (!drwav_init_file(&wav, path)) {
		printf("error: could not open file '%s'\n", path);
		return 0;
	}

	dr_int32* samples = malloc(wav.totalSampleCount * sizeof(dr_int32));
	size_t samples_count = drwav_read_s32(&wav, wav.totalSampleCount, samples);

	printf("info: sample rate is %d\n", wav.sampleRate);

	// calc image dimensions
	int img_width = 0;
	int img_height = 0;

	if (height == -1) {
		float s = sqrtf((float) samples_count);
		img_width = img_height = 8 * ((int) s / 8 + 1);
	} else {
		img_height = height;
		img_width = (int) ceilf(samples_count / (float) height);
	}

	printf("info: image dimensions will be %dx%d (%d channel/s)\n", img_width, img_height, channels);

	// allocate image data
	unsigned char* img_data = malloc(img_width * img_height * channels);

	// fill image data
	int i = 0;
	int end = samples_count * channels;
	for (int iy = 0; iy < img_height; iy++) {
		for (int ix = 0; ix < img_width; ix++) {
			if (mode == WP_MODE_BW) {
				img_data[i] = i < end ? samples[i] / 16777216 + 128 : 0;
				i++;
			} else if (mode == WP_MODE_RGB) {
				img_data[i] = i < end ? samples[i] / 16777216 + 128 : 0;
				i++;
				img_data[i] = i < end ? samples[i] / 16777216 + 128 : 0;
				i++;
				img_data[i] = i < end ? samples[i] / 16777216 + 128 : 0;
				i++;
			}
		}
	}

	// close file
	drwav_uninit(&wav);

	// write image
	stbi_write_png("output.png", img_width, img_height, channels, img_data, img_width * channels);

	printf("done\n");

	return 0;
}