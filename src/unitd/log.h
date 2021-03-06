/*
 * Copyright (C) 2015 Matthias Schiffer <mschiffer@universe-factory.net>
 *
 * Based on "procd" by:
 * Copyright (C) 2013 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2013 John Crispin <blogic@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include <libubox/ulog.h>

#include <stdlib.h>


#define DEBUG(level, fmt, ...) do { \
	if (debug >= level) { \
		ulog(LOG_DEBUG, fmt, ## __VA_ARGS__); \
	} } while (0)

#define LOG   ULOG_INFO
#define WARN  ULOG_WARN
#define ERROR ULOG_ERR

#define BUG(fmt, ...) do { \
		ERROR(fmt, ## __VA_ARGS__); \
		abort(); \
	} while (0)

extern unsigned int debug;
