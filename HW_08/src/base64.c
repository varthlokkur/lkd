#include "base64.h"

#include <linux/string.h>

// Base64 encoding table
static const char base64_table[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int calculate_encoding_size(int input_len)
{
	return ((input_len + 2) / 3) * 4 + 1;
}

int calculate_decoding_size(int input_len)
{
	return (((input_len / 4) * 3) - 2) + 1;
}

int base64_encode(const unsigned char *input, int input_len, char *output,
		  int output_len)
{
	int i, j;
	unsigned char a, b, c;

	pr_info("Encoding %d input bytes to %d output bytes\n", input_len,
		output_len);

	if (!output) {
		output_len = calculate_encoding_size(input_len);
		pr_info("Calculate output bytes by input bytes %d - %d\n",
			input_len, output_len);
		return output_len;
	}

	if (output_len < ((input_len + 2) / 3) * 4 + 1)
		return -1;

	j = 0;
	for (i = 0; i < input_len; i += 3) {
		a = input[i];
		b = (i + 1 < input_len) ? input[i + 1] : 0;
		c = (i + 2 < input_len) ? input[i + 2] : 0;

		output[j++] = base64_table[a >> 2];
		output[j++] = base64_table[((a & 0x03) << 4) | (b >> 4)];
		output[j++] =
			(i + 1 < input_len) ?
				base64_table[((b & 0x0F) << 2) | (c >> 6)] :
				'=';
		output[j++] = (i + 2 < input_len) ? base64_table[c & 0x3F] :
						    '=';
	}
	output[j] = '\0';
	return j;
}

int base64_decode(const char *input, int input_len, unsigned char *output,
		  int output_len)
{
	int i, j;
	unsigned char a, b, c, d;
	const char *pos;

	pr_info("Decoding %d input bytes to %d output bytes\n", input_len,
		output_len);

	if (input_len % 4 != 0) {
		return -1;
	}

	if (!output) {
		pr_info("Calculate output bytes by input bytes %d\n",
			input_len);
		return calculate_decoding_size(input_len);
	}

	j = 0;
	for (i = 0; i < input_len; i += 4) {
		// Convert characters to values
		pos = strchr(base64_table, input[i]);
		if (!pos)
			return -1;
		a = pos - base64_table;

		pos = strchr(base64_table, input[i + 1]);
		if (!pos)
			return -1;
		b = pos - base64_table;

		if (input[i + 2] == '=') {
			c = 0;
		} else {
			pos = strchr(base64_table, input[i + 2]);
			if (!pos)
				return -1;
			c = pos - base64_table;
		}

		if (input[i + 3] == '=') {
			d = 0;
		} else {
			pos = strchr(base64_table, input[i + 3]);
			if (!pos)
				return -1;
			d = pos - base64_table;
		}

		if (j < output_len)
			output[j++] = (a << 2) | (b >> 4);
		if (j < output_len && input[i + 2] != '=')
			output[j++] = (b << 4) | (c >> 2);
		if (j < output_len && input[i + 3] != '=')
			output[j++] = (c << 6) | d;
	}
	return j;
}