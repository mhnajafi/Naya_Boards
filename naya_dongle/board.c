/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <hal/nrf_power.h>

static int board_naya_dongle_init(void)
{

	/* if the naya_dongle board is powered from USB
	 * (high voltage mode), GPIO output voltage is set to 1.8 volts by
	 * default and that is not enough to turn the green and blue LEDs on.
	 * Increase GPIO voltage to 3.3 volts.
	 */

	// NRF_CLOCK->TASKS_LFCLKSTOP = 1UL;

	// // Use Internal OSC to compatible with all boards
	// NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_RC;
	// NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

	NRF_POWER->DCDCEN0 = 0;
	NRF_POWER->DCDCEN = 0;


	if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) != (UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos)){
		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
		NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
							(UICR_REGOUT0_VOUT_3V0 << UICR_REGOUT0_VOUT_Pos);

		NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
		while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

		NVIC_SystemReset();
	}


	// if ((nrf_power_mainregstatus_get(NRF_POWER) ==
	//      NRF_POWER_MAINREGSTATUS_HIGH) &&
	//     ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) ==
	//      (UICR_REGOUT0_VOUT_DEFAULT << UICR_REGOUT0_VOUT_Pos))) {

	// 	NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
	// 	while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {
	// 		;
	// 	}

	// 	NRF_UICR->REGOUT0 =
	// 	    (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
	// 	    (UICR_REGOUT0_VOUT_1V8 << UICR_REGOUT0_VOUT_Pos);

	// 	NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
	// 	while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {
	// 		;
	// 	}

	// 	/* a reset is required for changes to take effect */
	// 	NVIC_SystemReset();
	// }

	return 0;
}

SYS_INIT(board_naya_dongle_init, PRE_KERNEL_1,
	 CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
