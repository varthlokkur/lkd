#!/bin/bash

DEVICE_NAME="clkd"
DECODED_MSG="Hello World!"
ENCODED_MSG="SGVsbG8gV29ybGQh"

if [ ! -e /dev/${DEVICE_NAME} ]; then
    echo "Device /dev/${DEVICE_NAME} not found!"
    exit 1
fi

device_mode=$(sudo cat /sys/module/${DEVICE_NAME}/parameters/device_mode)
if [ "$device_mode" != "0" ]; then
    echo "Device /dev/${DEVICE_NAME} is not running in global mode!"
    exit 1
fi

# Encode some data
echo "[TEST] Encoding test"
echo "0" | sudo tee /sys/module/${DEVICE_NAME}/parameters/processing_mode > /dev/null
echo "Encoding '${DECODED_MSG}':"
echo -n ${DECODED_MSG} | sudo tee /dev/${DEVICE_NAME} > /dev/null
encoded_value=$(sudo cat /dev/${DEVICE_NAME})
echo ${encoded_value}

if [ "$encoded_value" == "$ENCODED_MSG" ]; then
    echo "[OK] Encode test success"
else
	echo "[FAIL] Encode test failed"
    exit 1
fi

# Set processing mode to decode
echo "[TEST] Decoding test"
echo "1" | sudo tee /sys/module/${DEVICE_NAME}/parameters/processing_mode > /dev/null
echo "Decoding '${ENCODED_MSG}':"
echo -n ${ENCODED_MSG} | sudo tee /dev/${DEVICE_NAME} > /dev/null
decoded_value=$(sudo cat /dev/${DEVICE_NAME})
echo ${decoded_value}

if [ "$decoded_value" == "$DECODED_MSG" ]; then
    echo "[OK] Decode test success"
else
	echo "[FAIL] Decode test failed"
    exit 1
fi