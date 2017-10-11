#pragma once
#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdint.h>

// context/device/surface data that sticks around for app lifetime
typedef struct vk_session_t {
  VkInstance instance;

  VkPhysicalDevice *physical_devices;
  uint32_t nphysical_devices;

  VkDevice logical_device;

  VkQueueFamilyProperties *queue_family_properties;
  uint32_t nqueue_family_properties;

  uint32_t queue_family_index;

  VkCommandPool command_pool;
  // Note: could be an array of buffers
  VkCommandBuffer command_buffer;
} vk_session_t;

bool start_vulkan( vk_session_t *vk_session, const char *app_short_name );
void stop_vulkan( vk_session_t *vk_session );
