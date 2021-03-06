/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.text.TextUtils;
import android.util.Log;

import java.util.HashMap;
import java.util.UUID;

/**
 * Gatt attributes store
 */
public class GattAttributes {
    private static HashMap<String, String> attributes = new HashMap();
    private static int[] ACC_SERVICE_UUID = { 0x01,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };
    private static int[] ACC_UUID = { 0x03,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };

    private static int[] LED_SERVICE_UUID = { 0x0b,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };
    private static int[] LED_UUID = { 0x0c,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };

    private static int[] TEMP_UUID = { 0x05,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };
    private static int[] ENV_SERVICE_UUID = { 0x04,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };

    private static int[] DOUBLE_TAP_UUID = { 0x09,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };
    private static int[] DOUBLE_TAP_SERVICE_UUID = { 0x08,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b };

    final static String ACC_SERVICE_UUID_STRING = convertHexUUIDToString(ACC_SERVICE_UUID);
    final static String ACC_UUID_STRING = convertHexUUIDToString(ACC_UUID);
    final static String LED_SERVICE_UUID_STRING = convertHexUUIDToString(LED_SERVICE_UUID);
    final static String LED_UUID_STRING = convertHexUUIDToString(LED_UUID);
    final static String TEMP_UUID_STRING = convertHexUUIDToString(TEMP_UUID);
    final static String ENV_SERVICE_UUID_STRING = convertHexUUIDToString(ENV_SERVICE_UUID);
    final static String DOUBLE_TAP_UUID_STRING = convertHexUUIDToString(DOUBLE_TAP_UUID);
    final static String DOUBLE_TAP_SERVICE_UUID_STRING = convertHexUUIDToString(DOUBLE_TAP_SERVICE_UUID);
    final static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";


    static {
        attributes.put(ACC_SERVICE_UUID_STRING, "Acceleration Service");
        attributes.put(ACC_UUID_STRING, "Acceleration Characteristic");
        attributes.put(TEMP_UUID_STRING, "Temperature Characteristic");
        attributes.put(ENV_SERVICE_UUID_STRING, "Environment Service");
        attributes.put(LED_UUID_STRING, "LED Characteristic");
        attributes.put(LED_SERVICE_UUID_STRING, "LED Service");
        attributes.put(DOUBLE_TAP_SERVICE_UUID_STRING, "Double Tap Service");
        attributes.put(DOUBLE_TAP_UUID_STRING, "Double Tap Characteristic");
    }

    // Utility method to convert from Hex UUID to String representation
    private static String convertHexUUIDToString(int[] components) {
        String[] hex_strings = new String[16];
        for (int i = 0; i < 16; i++) {
            // Bit of bit fiddling to make sure "2" is printed as "002"
            hex_strings[i] = Integer.toHexString(0x100 | components[i]).substring(1);
        }
        String hyphened_uuid = null;
        try {
            hyphened_uuid = TextUtils.join("", hex_strings).replaceAll(
                    "(\\w{8})(\\w{4})(\\w{4})(\\w{4})(\\w{12})",
                    "$1-$2-$3-$4-$5");
        } catch (Exception e) {
            Log.d("GattAttributes", "Encountered exception regexing on UUID components: ", e);
        }

        return UUID.fromString(hyphened_uuid).toString();
    }

    // Look up in HashMap
    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
