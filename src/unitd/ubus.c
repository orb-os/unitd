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

#include "unitd.h"

#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


static struct ubus_context *ctx;
static struct uloop_timeout ubus_timer;

static void
ubus_reconnect_cb(struct uloop_timeout *timeout)
{
	if (!ubus_reconnect(ctx, NULL))
		ubus_add_uloop(ctx);
	else
		uloop_timeout_set(timeout, 2000);
}

static void
ubus_disconnect_cb(UNUSED struct ubus_context *ctx)
{
	ubus_timer.cb = ubus_reconnect_cb;
	uloop_timeout_set(&ubus_timer, 2000);
}

static void
ubus_connect_cb(UNUSED struct uloop_timeout *timeout)
{
	ctx = ubus_connect(NULL);

	if (!ctx) {
		DEBUG(4, "Connection to ubus failed\n");
		uloop_timeout_set(&ubus_timer, 1000);
		return;
	}

	ctx->connection_lost = ubus_disconnect_cb;
	ubus_init_service(ctx);
	ubus_init_system(ctx);

	DEBUG(2, "Connected to ubus, id=%08x\n", ctx->local_id);
	ubus_add_uloop(ctx);
	unitd_state_ubus_connect();
}

void
unitd_connect_ubus(void)
{
	ubus_timer.cb = ubus_connect_cb;
	uloop_timeout_set(&ubus_timer, 1000);
}
