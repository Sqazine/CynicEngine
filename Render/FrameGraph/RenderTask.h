#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
namespace CynicEngine
{
    class RenderTaskBuilder;

    class RenderTaskBase
    {
    public:
        explicit RenderTaskBase(std::string_view name, bool cullImmune)
            : mName(name), mCullImmune(cullImmune), mRefCount(0)
        {
        }

        virtual ~RenderTaskBase() = default;

        RenderTaskBase(const RenderTaskBase &) = delete;
        RenderTaskBase &operator=(const RenderTaskBase &) = delete;

        RenderTaskBase(RenderTaskBase &&) = default;
        RenderTaskBase &operator=(RenderTaskBase &&) = default;

        std::string_view GetName() const { return mName; }
        void SetName(std::string_view name) { mName = name; }

        bool IsCullImmune() const { return mCullImmune; }
        void SetCullImmune(bool value) { mCullImmune = value; }

    protected:
        virtual void Setup(RenderTaskBuilder &builder) = 0;
        virtual void Execute() = 0;

        friend class FrameGraph;
        friend class RenderTaskBuilder;

        std::string mName;
        bool mCullImmune;
        std::vector<ResourceBase *> mResourceCreates;
        std::vector<ResourceBase *> mResourceReads;
        std::vector<ResourceBase *> mResourceWrites;
        size_t mRefCount;
    };

    template <typename DataType>
    class RenderTask : public RenderTaskBase
    {
    public:
        explicit RenderTask(std::string_view name,
                            bool cullImmune,
                            const std::function<void(DataType *, RenderTaskBuilder &)> &setupFunction,
                            const std::function<void(DataType *)> &executeFunction)
            : RenderTaskBase(name, cullImmune), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mData = std::make_unique<DataType>();
        }

        explicit RenderTask(std::string_view name,
                            bool cullImmune,
                            const std::function<DataType *()> &createFunction,
                            const std::function<void(DataType *, RenderTaskBuilder &)> &setupFunction,
                            const std::function<void(DataType *)> &executeFunction)
            : RenderTaskBase(name, cullImmune), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mData.reset(createFunction());
        }

        RenderTask(const RenderTask &) = delete;
        RenderTask &operator=(const RenderTask &) = delete;

        RenderTask(RenderTask &&) = default;
        RenderTask &operator=(RenderTask &&) = default;

        const DataType *GetData() const { return mData.get(); }

    protected:
        void Setup(RenderTaskBuilder &builder) override
        {
            mSetupFunction(mData.get(), builder);
        }

        void Execute() override
        {
            mExecuteFunction(mData.get());
        }

        std::unique_ptr<DataType> mData;
        const std::function<void(DataType *, RenderTaskBuilder &)> mSetupFunction;
        const std::function<void(DataType *)> mExecuteFunction;
    };

}