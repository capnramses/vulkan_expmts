#include "vk_utils.h"
#include <stdio.h>

int main() {
  vk_session_t vk_session;

  if ( !start_vulkan( &vk_session, "Anton does Vulkan\n" ) ) {
    fprintf( stderr, "Could not even start Vulkan. Major fail!" );
    return 1;
  }

  //
  // STUFF GOES HERE
  //

  // images and buffers. gpu mems
  // images not accessed directly - use an imageview object
  // buffers can be accessed directly but bufferview if used as texel store in shader
  // manually allocate gpu memory to store these - vkGetPhysicalDeviceMemoryProperties()
  // vkAllocateMemory()
  // https://renderdoc.org/vulkan-in-30-minutes.html

  stop_vulkan( &vk_session );

  return 0;
}
