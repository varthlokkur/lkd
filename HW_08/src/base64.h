#ifndef __CLKD_MODULE_BASE64_OPERATIONS_H__
#define __CLKD_MODULE_BASE64_OPERATIONS_H__

#include <linux/types.h>

// encode base64 string
// to calculate size send output == NULL
int base64_encode(const unsigned char *input, int input_len, char *output, int output_len);

// decode base64 string
// to calculate size send output == NULL
int base64_decode(const char *input, int input_len, unsigned char *output, int output_len);

#endif // __CLKD_MODULE_BASE64_OPERATIONS_H__