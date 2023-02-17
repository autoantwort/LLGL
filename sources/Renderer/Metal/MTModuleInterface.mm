/*
 * MTModuleInterface.h
 *
 * This file is part of the "LLGL" project (Copyright (c) 2015-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../ModuleInterface.h"
#include "MTRenderSystem.h"


namespace LLGL
{


namespace ModuleMetal
{
    int GetRendererID()
    {
        return RendererID::Metal;
    }

    const char* GetModuleName()
    {
        return "Metal";
    }

    const char* GetRendererName()
    {
        return "Metal";
    }

    RenderSystem* AllocRenderSystem(const LLGL::RenderSystemDescriptor* /*renderSystemDesc*/)
    {
        return new MTRenderSystem();
    }
} // /namespace ModuleMetal


} // /namespace LLGL

#ifndef LLGL_BUILD_STATIC_LIB

extern "C"
{

LLGL_EXPORT int LLGL_RenderSystem_BuildID()
{
    return LLGL_BUILD_ID;
}

LLGL_EXPORT int LLGL_RenderSystem_RendererID()
{
    return LLGL::ModuleMetal::GetRendererID();
}

LLGL_EXPORT const char* LLGL_RenderSystem_Name()
{
    return LLGL::ModuleMetal::GetRendererName();
}

LLGL_EXPORT void* LLGL_RenderSystem_Alloc(const void* renderSystemDesc, int renderSystemDescSize)
{
    if (renderSystemDesc != nullptr && static_cast<std::size_t>(renderSystemDescSize) == sizeof(LLGL::RenderSystemDescriptor))
    {
        auto desc = reinterpret_cast<const LLGL::RenderSystemDescriptor*>(renderSystemDesc);
        return LLGL::ModuleMetal::AllocRenderSystem(desc);
    }
    return nullptr;
}

} // /extern "C"

#endif



// ================================================================================
