/*
 * include/linux/amlogic/meson_uvm_core.h
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef __MESON_UVM_H
#define __MESON_UVM_H

#include <linux/device.h>
#include <linux/dma-direction.h>
#include <linux/miscdevice.h>
#include <linux/kref.h>
#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/ioctl.h>
#include <linux/types.h>

#include <linux/amlogic/media/vfm/vframe.h>

#define MAX_BUFF_NUMS	4
#define UVM_FAKE_SIZE 4096

enum uvm_alloc_flag {
	UVM_IMM_ALLOC,
	UVM_DELAY_ALLOC,
	UVM_FAKE_ALLOC,
	UVM_SECURE_ALLOC,
};

/**
 * struct uvm_handle - video dmabuffer wrap vframe
 *
 * @ref:	reference count
 * @lock:	protects the uvm_handle fields
 * @uvm_alloc:	buffer alloc info
 * @dma_buf:	dmabuf structure
 * @alloc:		non-alloc or allocated
 * @size:		dmabuf size
 * @align:		dmabuf align size
 * @flags:		alloc flag and usage flag
 * @vf:			builtin vframe
 * @vfp:		pointer the origin vframe
 * @mod_attached:	list of attached module
 * @n_mod_attached:	num of attached module
 * @mod_attached_mask:	mask of attached module
 */
struct uvm_handle {
	struct kref ref;
	struct mutex lock;

	struct uvm_alloc *ua;
	struct dma_buf *dmabuf;
	bool alloc;
	size_t size;
	size_t align;
	unsigned long flags;

	struct vframe_s vf;
	struct vframe_s *vfp;

	struct list_head mod_attached;
	size_t n_mod_attached;
	unsigned long mod_attached_mask;
};

/**
 * struct uvm_buf_obj - uvm buffer object
 * This structure define the uvm buffer object. uvm do not allocate buffer.
 * device that allocate buffer, wrap this structure.
 *
 * @arg:	uvm dmabuf exporter device priv structure
 * @dev:	uvm dmabuf exporter device
 */
struct uvm_buf_obj {
	void *arg;
	struct device *dev;
	struct dma_buf *dmabuf;
};

/**
 * struct uvm_alloc - uvm buffer alloc struct
 *
 * @sgt:	dmabuf sg_table
 * @pages:	dmabuf page array
 * @vaddr:	dmabuf kernel address
 * @size:	dmabuf real size
 * @obj:	uvm buffer object
 * @free:	free uvm_buf_obj related
 */
struct uvm_alloc {
	struct sg_table *sgt;
	struct page **pages;
	void *vaddr;
	size_t size;
	uint64_t flags;
	struct uvm_buf_obj *obj;
	void (*free)(struct uvm_buf_obj *obj);
};

struct uvm_alloc_info {
	size_t size;
	uint64_t flags;
	struct sg_table *sgt;
	struct uvm_buf_obj *obj;
	void (*free)(struct uvm_buf_obj *obj);
};

enum uvm_hook_mod_type {
	VF_SRC_DECODER,
	VF_SRC_VDIN,
	VF_PROCESS_V4LVIDEO,
	VF_PROCESS_DI,
	VF_PROCESS_VIDEOCOMPOSER,
};

/**
 * struct uvm_hook_mod - uvm hook module
 *
 * @ref:	reference count
 * @type:	module type
 * @arg:	module private data
 * @free:	module free function
 * @acquire_fence:	module acquire fence
 * @head:	module link node
 */
struct uvm_hook_mod {
	struct kref ref;
	enum uvm_hook_mod_type type;
	void *arg;
	void (*free)(void *arg);
	struct sync_fence *acquire_fence;
	struct list_head list;
};

struct uvm_hook_mod_info {
	enum uvm_hook_mod_type type;
	void *arg;
	void (*free)(void *arg);
	struct sync_fence *acquire_fence;
};

struct dma_buf *uvm_alloc_dmabuf(size_t len, size_t align, ulong flags);

bool dmabuf_is_uvm(struct dma_buf *dmabuf);

int dmabuf_bind_uvm_alloc(struct dma_buf *dmabuf, struct uvm_alloc_info *info);

/**
 * uvm vframe interface
 */
int dmabuf_set_vframe(struct dma_buf *dmabuf, struct vframe_s *vf);

struct vframe_s *dmabuf_get_vframe(struct dma_buf *dmabuf);

int dmabuf_put_vframe(struct dma_buf *dmabuf);

/**
 * uvm hook module interface
 */
int uvm_attach_hook_mod(struct dma_buf *dmabuf,
					struct uvm_hook_mod_info *info);
int uvm_detach_hook_mod(struct dma_buf *dmabuf,
					int type);

struct uvm_hook_mod *uvm_get_hook_mod(struct dma_buf *dmabuf,
						     int type);

int uvm_put_hook_mod(struct dma_buf *dmabuf, int type);

#endif
