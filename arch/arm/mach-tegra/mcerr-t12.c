/*
 * arch/arm/mach-tegra/mcerr-t12.c
 *
 * Tegra 12x SoC-specific mcerr code.
 *
 * Copyright (c) 2012, NVIDIA Corporation. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "mcerr.h"

#define dummy_client	client("dummy")

struct mc_client mc_clients[] = {
	client("ptc"),
	client("display0_wina"),	client("display1_wina"),
	client("display0_winb"),	client("display1_winb"),
	client("display0_winc"),	client("display1_winc"),
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	dummy_client,
	client("afi"),
	client("avpc"),
	client("display0_cursor"),	client("display1_cursor"),
	dummy_client,			dummy_client,
	dummy_client,
	client("hda"),			client("host1x_dma"),
	client("host1x_generic"),
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	client("msenc"),
	client("ahb_dma"),		client("ahb_slave"),
	client("sata"),
	dummy_client,			dummy_client,
	client("vde_bsev"),		client("vde_mbe"),
	client("vde_mce"),		client("vde_tpe"),
	client("cpu_lp"),		client("cpu"),
	dummy_client,			dummy_client,
	dummy_client,			client("msenc"),
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	dummy_client,
	client("afi"),			client("avpc"),
	dummy_client,			dummy_client,
	client("hda"),			client("host1x"),
	dummy_client,
	client("cpu_lp"),		client("cpu"),
	dummy_client,
	client("ahb_dma_wr"),		client("ahb_slave_wr"),
	client("sata"),			client("vde_bsev"),
	client("vde_dbg"),		client("vde_mbe"),
	client("vde_tpe"),
	dummy_client,			dummy_client,
	client("isp2_a"),
	dummy_client,
	client("isp2_a"),		client("isp2_b"),
	dummy_client,			dummy_client,
	client("xusb_host"),		client("xusb_host"),
	client("xusb_dev"),		client("xusb_dev"),
	client("isp2_a"),
	dummy_client,
	client("isp2_a"),		client("isp2_b"),
	dummy_client,			dummy_client,
	client("tsec"),			client("tsec"),
	client("cpu_lp"),		client("cpu"),
	client("gpu"),			client("gpu"),
	client("display0_wint"),
	dummy_client,
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	client("sdmmc1a"),		client("sdmmc2a"),
	client("sdmmc3a"),		client("sdmmc4a"),
	client("sdmmc1a"),		client("sdmmc2a"),
	client("sdmmc3a"),		client("sdmmc4a"),
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	client("vic"),			client("vic"),
	dummy_client,			dummy_client,
	dummy_client,			dummy_client,
	client("vi"),
	client("display0_wind"),
	client("unknown"),
};

int mc_client_last = ARRAY_SIZE(mc_clients) - 1;

static void mcerr_t12x_info_update(struct mc_client *c, u32 stat)
{
	if (stat & MC_INT_DECERR_EMEM)
		c->intr_counts[0]++;
	if (stat & MC_INT_SECURITY_VIOLATION)
		c->intr_counts[1]++;
	if (stat & MC_INT_INVALID_SMMU_PAGE)
		c->intr_counts[2]++;
	if (stat & MC_INT_INVALID_APB_ASID_UPDATE)
		c->intr_counts[3]++;
	if (stat & MC_INT_DECERR_VPR)
		c->intr_counts[4]++;
	if (stat & MC_INT_SECERR_SEC)
		c->intr_counts[5]++;
	if (stat & MC_INT_DECERR_MTS)
		c->intr_counts[6]++;

	if (stat & ~MC_INT_EN_MASK)
		c->intr_counts[7]++;
}

static int mcerr_t12x_debugfs_show(struct seq_file *s, void *v)
{
	int i, j;
	int do_print;

	seq_printf(s, "%-18s %-9s %-9s %-9s %-10s %-10s %-10s %-10s %-9s\n",
		   "client", "decerr", "secerr", "smmuerr",
		   "apberr", "decerr-VPR", "secerr-SEC",
		   "decerr_MST", "unknown");
	for (i = 0; i < ARRAY_SIZE(mc_clients); i++) {
		do_print = 0;
		if (strcmp(mc_clients[i].name, "dummy") == 0)
			continue;
		/* Only print clients who actually have errors. */
		for (j = 0; j < INTR_COUNT; j++) {
			if (mc_clients[i].intr_counts[j]) {
				do_print = 1;
				break;
			}
		}
		if (do_print)
			seq_printf(s, "%-18s %-9u %-9u %-9u %-10u "
					"%-10u %-10u %-10u %-9u\n",
				   mc_clients[i].name,
				   mc_clients[i].intr_counts[0],
				   mc_clients[i].intr_counts[1],
				   mc_clients[i].intr_counts[2],
				   mc_clients[i].intr_counts[3],
				   mc_clients[i].intr_counts[4],
				   mc_clients[i].intr_counts[5],
				   mc_clients[i].intr_counts[6],
				   mc_clients[i].intr_counts[7]);
	}
	return 0;
}

/*
 * Set up chip specific functions and data for handling this particular chip's
 * error decoding and logging.
 */
void mcerr_chip_specific_setup(struct mcerr_chip_specific *spec)
{
	spec->mcerr_info_update = mcerr_t12x_info_update;
	spec->mcerr_debugfs_show = mcerr_t12x_debugfs_show;
	spec->nr_clients = ARRAY_SIZE(mc_clients);
	return;
}
