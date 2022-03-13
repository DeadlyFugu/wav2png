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

/*
 * Simple library to write raw samples to a wav file on disk
 */

#ifndef SK_WAV_WRITE_H
#define SK_WAV_WRITE_H

#ifdef __cplusplus
extern "C" {
#endif

int sk_wav_write_file(char const* path, int channels, int sample_rate, int sample_count, const void* data);

#ifdef __cplusplus
}
#endif

// === IMPLEMENTATION ===

#ifdef SK_WAV_WRITE_IMPLEMENTATION
#include <stdio.h>
// #include <sandbox.h>
#include <stdint.h>

struct wav_header_t {
	char riff_str[4];
	uint32_t riff_size;
	char wave_str[4];

	char fmt_str[4];
	uint32_t fmt_size;
	uint16_t fmt_audio_format;
	uint16_t fmt_channels;
	uint32_t fmt_sample_rate;
	uint32_t fmt_byte_rate;
	uint16_t fmt_block_align;
	uint16_t fmt_bits_per_sample;

	char data_str[4];
	uint32_t data_size;
};

int sk_wav_write_file(char const* path, int channels, int sample_rate, int sample_count, const void* data) {
	struct wav_header_t wav_header;

	strncpy(wav_header.riff_str, "RIFF", 4);
	wav_header.riff_size = 4 + 8 + 16 + 8 + sample_count;
	strncpy(wav_header.wave_str, "WAVE", 4);

	strncpy(wav_header.fmt_str, "fmt ", 4);
	wav_header.fmt_size = 16;
	wav_header.fmt_audio_format = 1;
	wav_header.fmt_channels = channels;
	wav_header.fmt_sample_rate = sample_rate;
	wav_header.fmt_byte_rate = sample_rate * channels;
	wav_header.fmt_block_align = channels;
	wav_header.fmt_bits_per_sample = 8;

	strncpy(wav_header.data_str, "data", 4);
	wav_header.data_size = sample_count;

	FILE* file = fopen(path, "wb");
	size_t result;

	result = fwrite(&wav_header, sizeof(struct wav_header_t), 1, file);
	if (result != 1) return 0;

	result = fwrite(data, 1, sample_count, file);
	if (result != sample_count) return 0;

	fflush(file);
	fclose(file);

	return 1;
}

#endif

#endif