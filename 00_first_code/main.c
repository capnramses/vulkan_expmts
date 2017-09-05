// Anton Gerdelan <antonofnote@gmail.com> 5 Sep 2017
// Starting by following
// https://vulkan.lunarg.com/doc/sdk/1.0.57.0/windows/tutorial/html/00-intro.html

#include <stdio.h>

// Application -> Loader -> Layer 1 -> Layer n -> Driver
// * loader is a Vulkan library
// * creating vkInstance initialises the loader
// * loader also initialises the GPU driver
int main() {

  /* 1. create a vkInstance
  VkResult vkCreateInstance(
      const VkInstanceCreateInfo*                 pCreateInfo,
      const VkAllocationCallbacks*                pAllocator,
      VkInstance*                                 pInstance);
  */
  const VkInstanceCreateInfo *create_info_ptr = NULL;
  const VkAllocationCallbacks *allocator_ptr  = NULL;
  VkInstance *vulkan_inst_ptr                 = NULL;
  VkResult rslt                               = vkCreateInstance();

  printf( "Hello, World!\n" );
  return 0;
}
