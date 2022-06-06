/* Copyright (c) Kuba Szczodrzyński 2022-06-06. */

#include "LibreTuyaClass.h"

/**
 * @brief Get LibreTuya version string.
 */
const char *LibreTuya::getVersion() {
	return LT_VERSION_STR;
}

/**
 * @brief Get board name.
 */
const char *LibreTuya::getBoard() {
	return LT_BOARD_STR;
}

/**
 * @brief Get CPU family ID.
 */
ChipFamily LibreTuya::getChipFamily() {
	return FAMILY;
}

/**
 * @brief Get CPU family name as string.
 */
const char *LibreTuya::getChipFamilyName() {
	return STRINGIFY_MACRO(FAMILY);
}

static char *deviceName = NULL;

/**
 * @brief Get device friendly name in format "LT-<board>-<chip id>".
 * Can be used as hostname.
 */
const char *LibreTuya::getDeviceName() {
	if (deviceName)
		return deviceName;
	uint32_t chipId = getChipId();
	uint8_t *id		= (uint8_t *)&chipId;

	const char *board = getBoard();
	uint8_t boardLen  = strlen(board);
	deviceName		  = (char *)malloc(3 + boardLen + 1 + 6 + 1);

	sprintf(deviceName, "LT-%s-%02x%02x%02x", board, id[0], id[1], id[2]);
	return deviceName;
}

/**
 * @brief Get CPU frequency in MHz.
 */
uint32_t LibreTuya::getCpuFreqMHz() {
	return getCpuFreq() / 1000000;
}

static uint8_t otaRunningIndex = 0;

/**
 * @brief Get the currently running firmware OTA index.
 */
uint8_t LibreTuya::otaGetRunning() {
	if (otaRunningIndex)
		return otaRunningIndex;
	// otaRunningIndex will be correct even after switchOta()
	return otaRunningIndex = otaGetStoredIndex();
}

/**
 * @brief Get the OTA index for updated firmware.
 *
 * Note: returns 1 for chips without dual-OTA.
 */
uint8_t LibreTuya::otaGetTarget() {
	if (!otaSupportsDual())
		return 1;
	return otaGetRunning() ^ 0b11;
}

/**
 * @brief Perform OTA rollback.
 *
 * @return false if no second image to run, writing failed or dual-OTA not supported
 */
bool LibreTuya::otaRollback() {
	if (!otaCanRollback())
		return false;
	if (otaGetRunning() != otaGetStoredIndex())
		// force switching back to current image
		return otaSwitch(true);
	return true;
}

/**
 * @brief Check if OTA rollback is supported and available (there is another image to run).
 * @return false if no second image to run or dual-OTA not supported
 */
bool LibreTuya::otaCanRollback() {
	if (!otaSupportsDual())
		return false;
	if (otaGetRunning() == otaGetStoredIndex())
		return true;
	if (otaGetRunning() == 1 && otaHasImage1())
		return true;
	if (otaGetRunning() == 2 && otaHasImage2())
		return true;
	return false;
}

__attribute__((weak)) void LibreTuya::gpioRecover() {
	// nop by default
}
