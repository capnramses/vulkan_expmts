# vulkan
experimenting with vulkan

## concepts

* more direct exposure to hardware
* targets mobile devices as well as high-end computers

### basic program structure

* Application -> Loader -> Layer 1 -> Layer n -> Driver
* vkInstance - vulkan stores all per-application state here. must be created by application before other tasks.
* loader is a Vulkan library that loads the layers, above.
* layers are optional, and can be loaded selectively.
* driver is thinner in vulkan than opengl because validation work is delegated to layers
* creating vkInstance initialises the loader
* loader also initialises the GPU driver

## tutorials and examples

* LunarXchange Samples Tutorial - https://vulkan.lunarg.com/doc/sdk/1.0.57.0/windows/tutorial/html/00-intro.html
* Dustin Land's blog series - https://www.fasterthan.life/blog/2017/7/11/i-am-graphics-and-so-can-you-part-1
* vkDoom3 - https://github.com/DustinHLand/vkDOOM3
* Alexander Overvoorde - https://vulkan-tutorial.com/
* http://www.duskborn.com/a-simple-vulkan-compute-example/
* Intel - https://software.intel.com/en-us/articles/api-without-secrets-introduction-to-vulkan-preface
* Oculus SDK has a minimal Vulkan main.cpp - https://developer.oculus.com/downloads/
* https://github.com/Erkaman/vulkan_minimal_compute
* Sascha Willems' demos - https://github.com/SaschaWillems/Vulkan                 <----- looks great
* Renderdoc, Vulkan in 30 Minutes - https://renderdoc.org/vulkan-in-30-minutes.html
* https://gist.github.com/graphitemaster/e162a24e57379af840d4
* ARM, articles and tutorials https://developer.arm.com/graphics/vulkan/vulkan-tutorials
* Khronos, slides https://www.khronos.org/assets/uploads/developers/library/2016-vulkan-devu-seoul/1-Vulkan-Tutorial_English.pdf
