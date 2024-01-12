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
 * Default: false (debugging disabled)
 */
#define DEBUG_USB_SERIAL  0

/**
 * @brief Enable reading attention values
 * 
 * Default: true (attention enabled)
 */
#define ENABLE_ATTENTION  1

/**
 * @brief Enable reading meditation values
 * 
 * Default: true (meditation enabled)
 */
#define ENABLE_MEDITATION 1

/**
 * @brief Enable reading raw values
 * 
 * Default: true (raw enabled)
 */
#define ENABLE_RAW        1

/**
 * @brief Enable reading all raw values
 * 
 * Default: true (all raw enabled)
 */
#define ENABLE_ALL_RAW    1

/**
 * @brief Enable using pin as output quality indicator
 * 
 * Default: true (pin enabled)
 */
#define ENABLE_QUALITY_INDICATOR 1


#endif