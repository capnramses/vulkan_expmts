// Anton Gerdelan <antonofnote@gmail.com> 5 Sep 2017
// Starting by following
// https://vulkan.lunarg.com/doc/sdk/1.0.57.0/windows/tutorial/html/00-intro.html
// Note: inspect vulkan.h in samples folder of lunarg to see definitions of vk
// structs

#include <stdbool.h>
#include <stdio.h>
// TODO - vulkan utils, loaders, headers

VkInstance vulkan_inst;

bool start_vulkan() {
  // TODO -- some other loader/layer/startup stuff

  VkApplicationInfo app_info = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                .pNext = NULL,
                                .pApplicationName   = APP_SHORT_NAME,
                                .applicationVersion = 1,
                                .pEngineName        = APP_SHORT_NAME,
                                .engineVersion      = 1,
                                .apiVersion         = VK_API_VERSION_1_0};

  VkInstanceCreateInfo inst_info = {.sType =
                                        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                    .pNext                   = NULL,
                                    .flags                   = 0,
                                    .pApplicationInfo        = &app_info,
                                    .enabledExtensionCount   = 0,
                                    .ppEnabledExtensionNames = NULL,
                                    .enabledLayerCount       = 0,
                                    .ppEnabledLayerNames     = NULL};

  VkResult res = vkCreateInstance( &inst_info, NULL, &vulkan_inst );
  if ( VK_ERROR_INCOMPATIBLE_DRIVER == res ) {
    fprintf( stderr, "ERROR: cannot find compatible Vulkan ICD %s:%i\n",
             __FILE__, __LINE__ );
    return false;
  } else if ( res ) {
    fprintf( stderr, "ERROR: unknown %s:%i\n", __FILE__, __LINE__ );
    return false;
  }

  return true;
}

int main() {
  printf("Vulkun start/stop play-around.\nAnton Gerdelan <antonofnote@gmail.com> 5 Sep 2017\n")

  start_vulkan();

  //
  // STUFF GOES HERE
  //

  vkDestroyInstance( vulkan_inst, NULL );

  return 0;
}
