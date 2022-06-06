/* Copyright (c) Kuba Szczodrzyński 2022-06-06. */

#pragma once

#ifdef __cplusplus

#include "LibreTuyaAPI.h"
#include <core/ChipType.h>

#include <Flash.h> // for flash inline methods

/**
 * @brief Main LibreTuya API class.
 *
 * This class contains all functions common amongst all families.
 * Implementations of these methods may vary between families.
 *
 * The class is accessible using the `LT` global object (defined by the family).
 */
class LibreTuya {
  public: /* Common methods - note: these are documented in LibreTuyaAPI.cpp */
	const char *getVersion();
	const char *getBoard();
	ChipFamily getChipFamily();
	const char *getChipFamilyName();
	const char *getDeviceName();
	uint32_t getCpuFreqMHz();
	uint8_t otaGetRunning();
	uint8_t otaGetTarget();
	bool otaRollback();
	bool otaCanRollback();

  public: /* Inline methods */
	inline uint32_t getFlashChipSize() {
		return Flash.getSize();
	}

	// inline bool flashEraseSector(uint32_t sector) {}
	// inline bool flashWrite(uint32_t offset, uint32_t *data, size_t size) {}
	// inline bool flashRead(uint32_t offset, uint32_t *data, size_t size) {}
	// inline bool partitionEraseRange(const esp_partition_t *partition, uint32_t offset, size_t size) {}
	// inline bool partitionWrite(const esp_partition_t *partition, uint32_t offset, uint32_t *data, size_t size) {}
	// inline bool partitionRead(const esp_partition_t *partition, uint32_t offset, uint32_t *data, size_t size) {}

  public: /* Family-defined methods */
	/**
	 * @brief Reboot the CPU.
	 */
	void restart();
	/**
	 * @brief Reconfigure GPIO pins used for debugging
	 * (SWD/JTAG), so that they can be used as normal I/O.
	 */
	void gpioRecover();

  public: /* CPU-related */
	/**
	 * @brief Get CPU model ID.
	 */
	ChipType getChipType();
	/**
	 * @brief Get CPU model name as string.
	 */
	const char *getChipModel();
	/**
	 * @brief Get CPU unique ID. This may be based on MAC, eFuse, etc.
	 */
	uint32_t getChipId();
	/**
	 * @brief Get CPU core count.
	 */
	uint8_t getChipCores();
	/**
	 * @brief Get CPU core type name as string.
	 */
	const char *getChipCoreType();
	/**
	 * @brief Get CPU frequency in Hz.
	 */
	uint32_t getCpuFreq();
	/**
	 * @brief Get CPU cycle count.
	 */
	uint32_t getCycleCount();

  public: /* Memory management */
	/**
	 * @brief Get total RAM size.
	 */
	uint32_t getRamSize();
	/**
	 * @brief Get total heap size.
	 */
	uint32_t getHeapSize();
	/**
	 * @brief Get free heap size.
	 */
	uint32_t getFreeHeap();
	/**
	 * @brief Get lowest level of free heap memory.
	 */
	uint32_t getMinFreeHeap();
	/**
	 * @brief Get largest block of heap that can be allocated at once.
	 */
	uint32_t getMaxAllocHeap();

  public: /* OTA-related */
	/**
	 * @brief Read the currently active OTA index, i.e. the one that will boot upon restart.
	 */
	uint8_t otaGetStoredIndex();
	/**
	 * @brief Check if the chip supports dual-OTA.
	 */
	bool otaSupportsDual();
	/**
	 * @brief Check if OTA1 image is valid.
	 */
	bool otaHasImage1();
	/**
	 * @brief Check if OTA2 image is valid.
	 */
	bool otaHasImage2();
	/**
	 * @brief Try to switch OTA index to the other image.
	 *
	 * Note: should return true for chips without dual-OTA. Should return false if one of two images is not valid.
	 *
	 * @param force switch even if other image already marked as active
	 * @return false if writing failed; true otherwise
	 */
	bool otaSwitch(bool force = false);
};

extern LibreTuya LT;
extern LibreTuya ESP;

#endif
