// Anton Gerdelan <antonofnote@gmail.com> 5 Sep 2017
// Starting by following
// https://vulkan.lunarg.com/doc/sdk/1.0.57.0/windows/tutorial/html/00-intro.html
// Note: inspect vulkan.h in samples folder of lunarg to see definitions of vk
// structs

// TODO -- how do i enumerate and print list of physical devices?

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
// TODO - vulkan utils, loaders, headers

// manpages - https://www.khronos.org/registry/vulkan/specs/1.0/man/html

typedef struct vk_session_t {
  VkInstance instance;

  VkPhysicalDevice physical_devices[ 16 ]; // TODO(allocate mem for this)
  uint32_t nphysical_devices;              // gpu count

  VkDevice logical_device;

  VkQueueFamilyProperties queue_family_properties[ 256 ]; // TODO(allocate mem for this)
  uint32_t nqueue_family_properties;
} vk_session_t;

/* summary from vulkan-tutorial.com
Create a VkInstance
Select a supported graphics card (VkPhysicalDevice)
Create a VkDevice and VkQueue for drawing and presentation
Create a window, window surface and swap chain
Wrap the swap chain images into VkImageView
Create a render pass that specifies the render targets and usage
Create framebuffers for the render pass
Set up the graphics pipeline
Allocate and record a command buffer with the draw commands for every possible
swap chain image
Draw frames by acquiring images, submitting the right draw command buffer and
returning the images back to the swap chain
*/

// returns false on error, true on warning or success
bool check_vk_errors( VkResult res, const char *file, int line ) {
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

bool start_vulkan( vk_session_t *vk_session ) {
  assert( vk_session );

  // TODO -- some other loader/layer/startup stuff

  { // init vulkan instance
    VkApplicationInfo app_info = {.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                  .pNext              = NULL,
                                  .pApplicationName   = APP_SHORT_NAME,
                                  .applicationVersion = 1,
                                  .pEngineName        = APP_SHORT_NAME,
                                  .engineVersion      = 1,
                                  .apiVersion         = VK_API_VERSION_1_0};
    VkInstanceCreateInfo inst_info = {.sType               = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                      .pNext               = NULL,
                                      .flags               = 0,
                                      .pApplicationInfo    = &app_info,
                                      .enabledLayerCount   = 0,
                                      .ppEnabledLayerNames = NULL,
                                      .enabledExtensionCount   = 0,
                                      .ppEnabledExtensionNames = NULL};
    // pCreateInfo, pAllocator, pInstance
    VkResult res = vkCreateInstance( &inst_info, NULL, vk_session->instance );
    if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
      return false;
    }
  }
  { // physical devices
    // instance, pPhysicalDeviceCount, pPhysicalDevices
    VkResult res = vkEnumeratePhysicalDevices( vk_session->instance, &vk_session->nphysical_devices,
                                               vk_session->physical_devices );
    if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
      return false;
    }
    printf( "phyiscal device (GPU) count=%i\n", vk_session->nphysical_devices );
    if ( vk_session->nphysical_devices < 1 ) {
      return false;
    }
  }

  // TODO(Anton) query hardware. VRAM size etc and set device preferences

  { // create 'logical' device
    // physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties
    vkGetPhysicalDeviceQueueFamilyProperties( vk_session->physical_devices[ 0 ],
                                              &vk_session->nqueue_family_properties,
                                              vk_session->queue_family_properties );
    assert( vk_session->nqueue_family_properties >= 1 );

    float queue_priorities[ 1 ]        = {0.0};
    VkDeviceQueueCreateInfo queue_info = {.sType      = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                          .pNext      = NULL,
                                          .flags      = 0, // ??? was missing in example
                                          .queueCount = 1,
                                          .pQueuePriorities = queue_priorities};

    // add family queue indices to queue family properties
    bool found = false;
    for ( uint32_t i = 0; i < vk_session->nqueue_family_properties; i++ ) {
      if ( vk_session->nqueue_family_properties[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
        queue_info.queueFamilyIndex = i;
        found                       = true;
        break;
      }
    }
    assert( found );

    VkDeviceCreateInfo device_info = {.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                      .pNext                 = NULL,
                                      .queueCreateInfoCount  = 1,
                                      .pQueueCreateInfos     = &queue_info,
                                      .enabledLayerCount     = 0,
                                      .ppEnabledLayerNames   = NULL,
                                      .enabledExtensionCount = 0,
                                      .ppEnabledExtensionNames = NULL,
                                      .pEnabledFeatures        = NULL};
    // physicalDevice, pCreateInfo, pAllocator, pDevice
    VkResult res = vkCreateDevice( vk_session->physical_devices[ 0 ], &device_info, NULL,
                                   &vk_session->logical_device );
    if ( !check_vk_errors( res, __FILE__, __LINE__ ) ) {
      return false;
    }
  }

  // TODO(Anton) set up window surface (using eg GLFW) and swap chain
  // Note - window surfaces etc are khronos KHR extensions and not part of main
  // API - GLFW does this stuff

  return true;
}

void stop_vulkan( vk_session_t *vk_session ) {
  assert( vk_session );

  vkDestroyDevice( vk_session->logical_device, NULL );
  vkDestroyInstance( vk_session->instance, NULL );
}

int main() {
  printf( "Vulkun start/stop play-around.\nAnton Gerdelan <antonofnote@gmail.com> 5 Sep 2017\n" );

  start_vulkan();

  //
  // STUFF GOES HERE
  //

  stop_vulkan();

  return 0;
}
