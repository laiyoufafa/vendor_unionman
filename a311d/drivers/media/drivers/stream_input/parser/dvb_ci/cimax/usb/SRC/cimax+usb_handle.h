/*
 * Copyright (c) 2022 Unionman Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CIMAXPLUS_USB_HDLE_H
#define __CIMAXPLUS_USB_HDLE_H

#ifdef __KERNEL__

struct cimaxusb_priv_ops_t {
	int (*write_ctrl_message)(
		struct usb_device *dev, int addr, void *data, int size);

	int (*read_ctrl_message)(
		struct usb_device *dev, int addr, void *data, int size);

	int (*init_fw)(
		struct usb_device *dev);

	int (*write_ep6_message)(
		struct usb_device *dev, void *data, int size);

	int (*read_ep5_message)(
		struct usb_device *dev, void *data, int size);
};

#endif

#endif
