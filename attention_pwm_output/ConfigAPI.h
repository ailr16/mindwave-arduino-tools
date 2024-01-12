/**
 * @file  ConfigAPI.h
 * @brief This file contains macros used as API configuration
 * 
 * Enable/disable some features like reading attention, meditation, raw and using pin as
 * output indicator for data quality. Disable it when the feature is not used to reduce
 * the memory usage
 */

#ifndef CONFIG_API_H
#define CONFIG_API_H


/**
 * @brief Enable debugging using Serial port
 * 
 * Default: 0 (debugging disabled)
 */
#define DEBUG_USB_SERIAL  0

/**
 * @brief Enable reading attention values
 * 
 * Default: 1 (attention enabled)
 */
#define ENABLE_ATTENTION  1

/**
 * @brief Enable reading meditation values
 * 
 * Default: 1 (meditation enabled)
 */
#define ENABLE_MEDITATION 0

/**
 * @brief Enable reading raw values
 * 
 * Default: 1 (raw enabled)
 */
#define ENABLE_RAW        0

/**
 * @brief Enable reading all raw values
 * 
 * Default: 1 (all raw enabled)
 */
#define ENABLE_ALL_RAW    0

/**
 * @brief Enable using pin as output quality indicator
 * 
 * Default: 1 (pin enabled)
 */
#define ENABLE_QUALITY_INDICATOR 1


#endif