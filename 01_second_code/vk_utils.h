#pragma once

#define VK_USE_PLATFORM_XCB_KHR // TODO put in makefile

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdint.h>
#define MAX_EXTENSIONS 256

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

  // names of extensions to use
  const char *instance_extension_names[MAX_EXTENSIONS];
  uint32_t ninstance_extension_names;
  const char *device_extension_names[MAX_EXTENSIONS];
  uint32_t ndevice_extension_names;
} vk_session_t;

bool start_vulkan( vk_session_t *vk_session, const char *app_short_name );
void stop_vulkan( vk_session_t *vk_session );
