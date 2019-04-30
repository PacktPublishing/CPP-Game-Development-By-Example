#pragma once

#include <vulkan\vulkan.h>
#include <stdexcept>
#include <vector>



namespace vkTools {



	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);



}
