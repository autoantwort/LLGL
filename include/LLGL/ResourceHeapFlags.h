/*
 * ResourceViewHeapFlags.h
 *
 * This file is part of the "LLGL" project (Copyright (c) 2015-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef LLGL_RESOURCE_VIEW_HEAP_FLAGS_H
#define LLGL_RESOURCE_VIEW_HEAP_FLAGS_H


#include <LLGL/Export.h>
#include <LLGL/Texture.h>
#include <LLGL/TextureFlags.h>
#include <LLGL/Buffer.h>
#include <LLGL/BufferFlags.h>
#include <vector>


namespace LLGL
{


class PipelineLayout;


/* ----- Enumerations ----- */

/**
\brief Flags for memory barriers in resource heaps.
\see ResourceHeapDescriptor::barrierFlags
*/
struct BarrierFlags
{
    enum
    {
        /**
        \brief Memory barrier for Buffer resources that were created with the BindFlags::Storage bind flags.
        \remarks Shader access to the buffer will reflect all data written to by previous shaders.
        \see BindFlags::Storage
        */
        StorageBuffer   = (1 << 0),

        /**
        \brief Memory barrier for Texture resources that were created with the BindFlags::Storage bind flags.
        \remarks Shader access to the texture will reflect all data written to by previous shaders.
        \see BindFlags::Storage
        */
        StorageTexture  = (1 << 1),
    };
};


/* ----- Structures ----- */

/**
\brief Resource view descriptor structure.
\see ResourceHeapDescriptor::resourceViews
*/
struct ResourceViewDescriptor
{
    //! Default constructor to initialize the resource view with a null pointer.
    ResourceViewDescriptor() = default;

    //! Constructor to initialize the descriptor with a resource. The resource view will access the entire resource.
    inline ResourceViewDescriptor(Resource* resource) :
        resource { resource }
    {
        /* Invalidate subresource views */
        textureView.format = Format::Undefined;
    }

    //! Constructor to initialize a descriptor with a texture subresource view.
    inline ResourceViewDescriptor(Texture* texture, const TextureViewDescriptor& subresourceDesc) :
        resource    { texture         },
        textureView { subresourceDesc }
    {
    }

    //! Constructor to initialize a descriptor with a buffer subresource view.
    inline ResourceViewDescriptor(Buffer* buffer, const BufferViewDescriptor& subresourceDesc) :
        resource   { buffer          },
        bufferView { subresourceDesc }
    {
        /* Invalidate subresource views */
        textureView.format = Format::Undefined;
    }

    //! Pointer to the hardware resource. This must not be null when passed to a ResourceHeap.
    Resource*               resource    = nullptr;

    /**
    \brief Optional texture view descriptor.
    \remarks Can be used to declare a subresource view of a texture resource.
    \remarks This attribute is ignored if \e one of the following sub members has the respective value listed below:
    - \c textureView.format is Format::Undefined
    - \c textureView.subresource.numMipLevels is 0
    - \c textureView.subresource.numArrayLayers is 0
    */
    TextureViewDescriptor   textureView;

    /**
    \brief Optional buffer view descriptor.
    \remarks Can be used to declare a subresource view of a buffer resource.
    \remarks This attribute is ignored if \e all of the following sub members have the respective value listed below:
    - \c bufferView.format is Format::Undefined;
    - \c bufferView.offset is 0.
    - \c bufferView.size is \c Constants::wholeSize.
    */
    BufferViewDescriptor    bufferView;
};

/**
\brief Resource heap descriptor structure.
\remarks For the render systems of modern graphics APIs (i.e. Vulkan and Direct3D 12), a resource heap is the only way to bind hardware resources to a shader pipeline.
The resource heap is a container for one or more resources such as textures, samplers, constant buffers etc.
\see RenderSystem::CreateResourceHeap
*/
struct ResourceHeapDescriptor
{
    ResourceHeapDescriptor() = default;

    //! Initializes the resource heap descriptor with the specified pipeline layout and optional secondary parameters.
    inline ResourceHeapDescriptor(PipelineLayout* pipelineLayout, std::uint32_t numResourceViews = 0, long barrierFlags = 0) :
        pipelineLayout   { pipelineLayout   },
        numResourceViews { numResourceViews },
        barrierFlags     { barrierFlags     }
    {
    }

    //! Reference to the pipeline layout. This must not be null, when a resource heap is created.
    PipelineLayout* pipelineLayout      = nullptr;

    /**
    \brief Specifies the number of resource views.
    \remarks If the number of resource views is non-zero, it \b must a multiple of the bindings in the pipeline layout.
    \remarks If the number of resource views is zero, the number will be determined by the initial resource views
    and they must \e not be empty and they \b must be a multiple of the bindings in the pipeline layout.
    \see PipelineLayoutDescriptor::bindings
    \see RenderSystem::CreateResourceHeap
    */
    std::uint32_t   numResourceViews    = 0;

    /**
    \brief Specifies optional resource barrier flags. By default 0.
    \remarks If the barrier flags are non-zero, they will be applied before any resource are bound to the graphics/compute pipeline.
    This should be used when a resource is bound to the pipeline that was previously written to.
    \see BarrierFlags
    */
    long            barrierFlags        = 0;
};


} // /namespace LLGL


#endif



// ================================================================================
