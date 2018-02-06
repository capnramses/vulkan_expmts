// TODO -- finish swapchain and surface setup
// TODO -- create a window with glfw or sdl2 (next demo)

#include "vk_utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO linux only
#include <unistd.h>
#include <vulkan/vk_sdk_platform.h>

#define APP_SHORT_NAME "anton does vulkan"

// manpages - https://www.khronos.org/registry/vulkan/specs/1.0/man/html

// checks a vk function call result for errors, and if found prints error string to stderr
//   res - result of a vk function call
//   file - filename of calling source code. eg __FILE__
//   line - line number of calling source code. eg __LINE__
//   returns - false on error, true on warning or success
static bool check_vk_errors( VkResult res, const char *file, int line ) {
  assert( file );

  switch ( res ) {
    case VK_SUCCESS: {
      break;
    }
    case VK_ERROR_OUT_OF_HOST_MEMORY: {
      fprintf( stderr, "ERROR: VK_ERROR_OUT_OF_HOST_MEMORY %s:%i\n", file, line );
      return false;
    }
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
      fprintf( stderr, "ERROR: VK_ERROR_OUT_OF_DEVICE_MEMORY %s:%i\n", file, line );
      return false;
    }
    case VK_ERROR_INITIALIZATION_FAILED: {
      fprintf( stderr, "ERROR: VK_ERROR_INITIALIZATION_FAILED %s:%i\n", file, line );
      return false;
    }
    case VK_ERROR_LAYER_NOT_PRESENT: {
      fprintf( stderr, "ERROR: VK_ERROR_LAYER_NOT_PRESENT %s:%i\n", file, line );
      return false;
    }
    case VK_ERROR_EXTENSION_NOT_PRESENT: {
      fprintf( stderr, "ERROR: VK_ERROR_EXTENSION_NOT_PRESENT %s:%i\n", file, line );
      return false;
    }
    case VK_ERROR_INCOMPATIBLE_DRIVER: {
      fprintf( stderr, "ERROR: VK_ERROR_INCOMPATIBLE_DRIVER %s:%i\n", file, line );
      return false;
    }
    case VK_INCOMPLETE: {
      fprintf( stderr, "WARNING: VK_INCOMPLETE %s:%i\n", file, line );
      break;
    }
    default: { fprintf( stderr, "WARNING: unhandled return condition from %s:%i\n", file, line ); }
  } // endswitch

  return true;
}

// TODO platform auto-detect and support option here
static void init_vk_instance_extension_names( vk_session_t *vk_session ) {
  printf( "init_vk_instance_extension_names\n" );
  assert( vk_session );

  vk_session->extension_names[vk_session->nextension_names++] = VK_KHR_SURFACE_EXTENSION_NAME;
  vk_session->extension_names[vk_session->nextension_names++] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
  //vk_session->extension_names[vk_session->nextension_names++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME; // EXTENSION NOT PRESENT!!!
  // vk_session->extension_names[vk_session->nextension_names++] =
  // VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
  // vk_session->extension_names[vk_session->nextension_names++] =
  // VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
  // vk_session->extension_names[vk_session->nextension_names++] =
  // VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
  // vk_session->extension_names[vk_session->nextension_names++] =
  // VK_MVK_IOS_SURFACE_EXTENSION_NAME;

  for ( int i = 0; i < vk_session->nextension_names; i++ ) {
    printf( "%i [%s]\n", i, vk_session->extension_names[i] );
  }
}

static bool init_vk_instance( vk_session_t *vk_session, const char *app_short_name ) {
  printf( "init_vk_instance...\n" );
  assert( vk_session );
  assert( app_short_name );

  VkApplicationInfo app_info = {.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                .pNext              = NULL,
                                .pApplicationName   = app_short_name,
                                .applicationVersion = 1,
                                .pEngineName        = app_short_name,
                                .engineVersion      = 1,
                                .apiVersion         = VK_API_VERSION_1_0};

  const char *const *ext_ptr     = vk_session->extension_names;
  VkInstanceCreateInfo inst_info = {.sType                 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                    .pNext                 = NULL,
                                    .flags                 = 0,
                                    .pApplicationInfo      = &app_info,
                                    .enabledLayerCount     = 0,
                                    .ppEnabledLayerNames   = NULL,
                                    .enabledExtensionCount = vk_session->nextension_names,
                                    // ppEnabledExtensionNames is a pointer to an array of
                                    // enabledExtensionCount null-terminated UTF-8 strings
                                    // containing the names of extensions to enable.
                                    .ppEnabledExtensionNames = vk_session->extension_names};
  // pCreateInfo, pAllocator, pInstance
  VkResult res = vkCreateInstance( &inst_info, NULL, &vk_session->instance );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }
  return true;
}

static bool init_vk_physical_devices( vk_session_t *vk_session ) {
  printf( "init_vk_physical_devices...\n" );
  assert( vk_session );

  // instance, pPhysicalDeviceCount, pPhysicalDevices
  VkResult res =
      vkEnumeratePhysicalDevices( vk_session->instance, &vk_session->nphysical_devices, NULL );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }
  printf( "vk_session->nphysical_devices=%i\n", vk_session->nphysical_devices );
  vk_session->physical_devices =
      (VkPhysicalDevice *)malloc( vk_session->nphysical_devices * sizeof( VkPhysicalDevice ) );
  assert( vk_session->physical_devices );
  res = vkEnumeratePhysicalDevices( vk_session->instance, &vk_session->nphysical_devices,
                                    vk_session->physical_devices );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }
  if ( vk_session->nphysical_devices < 1 ) {
    return false;
  }
  for ( int i = 0; i < vk_session->nphysical_devices; i++ ) {
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties( vk_session->physical_devices[i], &properties );
    printf( "Device %i properties:\n", i );
    printf( "API version:    %u\n", properties.apiVersion );
    printf( "Driver version: %u\n", properties.driverVersion );
    printf( "Vendor ID:      %u\n", properties.vendorID );
    printf( "Device ID:      %u\n", properties.deviceID );
    printf( "Name:           %s\n", properties.deviceName );
    printf( "Type:           %i {other,integrated,discrete,virtual,cpu}\n", properties.deviceType );
    // there are also limits and sparseProperties structs with more info
    // limits doc:
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkPhysicalDeviceLimits.html
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties( vk_session->physical_devices[i], &mem_properties );
    printf( "Memory heap count %i:\n", mem_properties.memoryHeapCount );
    for ( int j = 0; j < mem_properties.memoryHeapCount; j++ ) {
      printf( "  heap size: %lu\n", (long unsigned int)mem_properties.memoryHeaps[j].size );
    }
  }
  // TODO note there is also GetPhysicalDeviceFeatures
  return true;
}

static bool init_vk_logical_device( vk_session_t *vk_session, int physical_device_index ) {
  printf( "init_vk_logical_device...\n" );
  assert( vk_session );
  // create 'logical' device NOTE: using device 0 as default here
  int device_to_use = 0;

  // physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties
  vkGetPhysicalDeviceQueueFamilyProperties( vk_session->physical_devices[device_to_use],
                                            &vk_session->nqueue_family_properties, NULL );
  printf( "vk_session->nqueue_family_properties=%i\n", vk_session->nqueue_family_properties );

  vk_session->queue_family_properties = (VkQueueFamilyProperties *)malloc(
      vk_session->nqueue_family_properties * sizeof( VkQueueFamilyProperties ) );
  assert( vk_session->queue_family_properties );

  vkGetPhysicalDeviceQueueFamilyProperties( vk_session->physical_devices[device_to_use],
                                            &vk_session->nqueue_family_properties,
                                            vk_session->queue_family_properties );

  float queue_priorities[1]          = {0.0};
  VkDeviceQueueCreateInfo queue_info = {.sType      = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                        .pNext      = NULL,
                                        .flags      = 0, // ??? was missing in example
                                        .queueCount = 1,
                                        .pQueuePriorities = queue_priorities};

  // there may be several queues split into groups or `families` based on their job;
  // (graphics/compute/etc or some combo thereof) see:
  // https://vulkan.lunarg.com/doc/sdk/1.0.57.0/windows/tutorial/html/03-init_device.html
  // assign each family property to a family via matching the index number
  bool found = false;
  for ( uint32_t i = 0; i < vk_session->nqueue_family_properties; i++ ) {
    // NOTE( there are also compute, transfer, sparse_binding bits ). we are only looking for gfx
    // bit
    if ( vk_session->queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
      queue_info.queueFamilyIndex    = i;
      found                          = true;
      vk_session->queue_family_index = i; // used later in command buffer pool
      break;
    }
  }
  assert( found );

  VkDeviceCreateInfo device_info = {
      .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext                   = NULL,
      .queueCreateInfoCount    = 1,
      .pQueueCreateInfos       = &queue_info,
      .enabledLayerCount       = 0,    // device layers were recently deprecated in Vulkan
      .ppEnabledLayerNames     = NULL, // device layers were recently deprecated in Vulkan
      .enabledExtensionCount   = 0,
      .ppEnabledExtensionNames = NULL,
      .pEnabledFeatures        = NULL};
  // physicalDevice, pCreateInfo, pAllocator, pDevice
  VkResult res = vkCreateDevice( vk_session->physical_devices[0], &device_info, NULL,
                                 &vk_session->logical_device );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }
  return true;
}

static bool init_vk_command_buffer( vk_session_t *vk_session ) {
  printf( "init_vk_command_buffer...\n" );
  assert( vk_session );

  // create a command buffer. note after populating with commands must call vkQueueSubmit()
  VkCommandPoolCreateInfo cmd_pool_info = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                           .pNext = NULL,
                                           .queueFamilyIndex = vk_session->queue_family_index,
                                           .flags            = 0};

  VkResult res = vkCreateCommandPool( vk_session->logical_device, &cmd_pool_info, NULL,
                                      &vk_session->command_pool );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }

  VkCommandBufferAllocateInfo cmd_info = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                          .pNext = NULL,
                                          .commandPool        = vk_session->command_pool,
                                          .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                          .commandBufferCount = 1};

  res = vkAllocateCommandBuffers( vk_session->logical_device, &cmd_info,
                                  &vk_session->command_buffer );
  if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
    return false;
  }
  return true;
}

static bool init_vk_swapchain( vk_session_t *vk_session ) {
  printf( "init_vk_swapchain...\n" );
  assert( vk_session );
  // create swapchain (image buffers for swapping/display). platform-specific stuff is in
  // extensions like KHR

  /* from LunarG tutorial:
  The WSI extensions contain support for various platforms. The extensions are activated for a
  particular platform by defining:

  VK_USE_PLATFORM_ANDROID_KHR - Android
  VK_USE_PLATFORM_MIR_KHR - Mir
  VK_USE_PLATFORM_WAYLAND_KHR - Wayland
  VK_USE_PLATFORM_WIN32_KHR - Microsoft Windows
  VK_USE_PLATFORM_XCB_KHR - X Window System, using the XCB library
  VK_USE_PLATFORM_XLIB_KHR - X Window System, using the Xlib library
  The "KHR" portion of the name indicates that the symbol is defined in a Khronos extension.
  */
  return true;
}

bool start_vulkan( vk_session_t *vk_session, const char *app_short_name ) {
  printf( "start_vulkan...\n" );
  assert( vk_session );
  assert( app_short_name );

  // TODO -- some other loader/layer/startup stuff
  init_vk_instance_extension_names( vk_session );
  if ( !init_vk_instance( vk_session, app_short_name ) ) {
    return false;
  }
  if ( !init_vk_physical_devices( vk_session ) ) {
    return false;
  }
  if ( !init_vk_logical_device( vk_session, 0 ) ) {
    return false;
  }
  if ( !init_vk_command_buffer( vk_session ) ) {
    return false;
  }
  if ( !init_vk_swapchain( vk_session ) ) {
    return false;
  }

  // "The overall picture is that your initialisation mostly looks like vkCreateInstance() →
  // vkEnumeratePhysicalDevices() → vkCreateDevice().
  // For a quick and dirty hello world triangle program, you can do just that and pick the first
  // physical device, then come back to it
  // once you want error reporting & validation, enabling optional device features, etc."

  // TODO(Anton) set up window surface (using eg GLFW)
  // Note - window surfaces etc are khronos KHR extensions and not part of main
  // API - GLFW does this stuff
  // VkSurfaceKHR   vkCreateWin32SurfaceKHR(inst, &surfaceCreateInfo, NULL, &surf);

  // TODO(Anton) create swap chain vkCreateSwapchainKHR()

  return true;
}

void stop_vulkan( vk_session_t *vk_session ) {
  assert( vk_session );

  vkDestroyDevice( vk_session->logical_device, NULL );
  vkDestroyInstance( vk_session->instance, NULL );
  if ( vk_session->queue_family_properties ) {
    free( vk_session->queue_family_properties );
    vk_session->queue_family_properties = NULL;
  }
  vk_session->nqueue_family_properties = 0;
  if ( vk_session->physical_devices ) {
    free( vk_session->physical_devices );
    vk_session->physical_devices = NULL;
  }
  vk_session->nphysical_devices = 0;
}
