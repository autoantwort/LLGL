/*
 * VKDeviceMemory.h
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015-2017 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef LLGL_VK_DEVICE_MEMORY_MANAGER_H
#define LLGL_VK_DEVICE_MEMORY_MANAGER_H


#include "../Vulkan.h"
#include "../VKPtr.h"
#include "VKDeviceMemory.h"
#include "VKDeviceMemoryRegion.h"
#include <vector>
#include <memory>


namespace LLGL
{


/*
Vulkan device memory manager. Memory allocations are stored in a small hierarchy:
 - Chunk: denotes a single Vulkan memory allocation of type VkDeviceMemory
 - Block: denotes one of multiple regions inside a chunk of type VkBuffer
 - Region: denotes a sub-range inside a block and holds a reference to the VkBuffer and its offset and size (both of type VkDeviceSize).
*/
class VKDeviceMemoryManager
{

    public:

        VKDeviceMemoryManager(const VKPtr<VkDevice>& device, const VkPhysicalDeviceMemoryProperties& memoryProperties);

        VKDeviceMemoryManager(const VKDeviceMemoryManager&) = delete;
        VKDeviceMemoryManager& operator = (const VKDeviceMemoryManager&) = delete;

        // Allocates a new device memory block of the specified size and with the specified attributes.
        VKDeviceMemoryRegion* Allocate(VkDeviceSize size, VkDeviceSize alignment, std::uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);

        // Releases the specified device memory block.
        void Release(VKDeviceMemoryRegion* region);

        // Queries the memory details of all chunks.
        VKDeviceMemoryDetails QueryDetails() const;

        #ifdef LLGL_DEBUG

        void PrintBlocks(std::ostream& s, const std::string& title = "") const;

        #endif

    private:

        // Finds a memory type index for the specified attributes.
        std::uint32_t FindMemoryType(std::uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) const;

        // Allocates a new VkDeviceMemory chunk of the specified size and memory type.
        VKDeviceMemory* AllocChunk(VkDeviceSize allocationSize, std::uint32_t memoryTypeIndex);

        // Finds a suitable device memory chunk or allocates a new one.
        VKDeviceMemory* FindOrAllocChunk(VkDeviceSize allocationSize, std::uint32_t memoryTypeIndex, VkDeviceSize minFreeBlockSize);

        const VKPtr<VkDevice>&                          device_;
        VkPhysicalDeviceMemoryProperties                memoryProperties_;

        VkDeviceSize                                    minAllocationSize_  = 1024*1024*10;
        std::vector<std::unique_ptr<VKDeviceMemory>>    chunks_;

};


} // /namespace LLGL


#endif



// ================================================================================
