#pragma once
#include <vector>
#include "Core/Singleton.h"
#include "Core/Marco.h"

namespace CynicEngine
{
    struct DisplayInfo
    {
        uint32_t width{0};
        uint32_t height{0};
        float pixelDensity{0};
        float refreshRate{0};
        std::string_view format{};
        bool isPrimary{false};
        bool isHDR{false};
    };

    enum class CpuManufacturer
    {
        UNKNOWN,
        INTEL,
        AMD,
        ARM,
        APPLE,
    };

    struct CpuInfo
    {
        STRING name;
        CpuManufacturer manufacturer{CpuManufacturer::UNKNOWN};
        uint8_t logicCoreCount{0};
        uint8_t physicalCoreCount{0};
    };

    struct MemoryInfo
    {
        size_t totalMemory{0};
        size_t freeMemory{0};
        size_t usedMemory{0};
    };

    class HardwareInfo
    {
    public:
        HardwareInfo() = default;
        virtual ~HardwareInfo()
        {
            mDisplayInfos.clear();
        }

        static HardwareInfo *Create();

        const std::vector<DisplayInfo> &GetDisplayInfos() const
        {
            return mDisplayInfos;
        }

    protected:
        virtual void ObtainDisplayInfo() = 0;
        
        //TODO: Implement these methods in derived classes
        // virtual void ObtainCpuInfo() = 0;
        // virtual void ObtainMemoryInfo() = 0;

        std::vector<DisplayInfo> mDisplayInfos;
        CpuInfo mCpuInfo;
        MemoryInfo mMemoryInfo;
    };

    class PlatformInfo : public Singleton<PlatformInfo>
    {
    public:
        void Init()
        {
            mHardwareInfo.reset(HardwareInfo::Create());
        }

        void Destroy()
        {
            mHardwareInfo.reset();
        }

        HardwareInfo *GetHardwareInfo() const
        {
            return mHardwareInfo.get();
        }

    private:
        std::unique_ptr<HardwareInfo> mHardwareInfo;
    };
}