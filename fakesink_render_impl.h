#ifndef DS3D_DATARENDER_FAKESINK_IMPL_H
#define DS3D_DATARENDER_FAKESINK_IMPL_H

#include "fakesink_render.h"
#include "fakesink_render_config.h"

// include ds3d headers here
#include <ds3d/common/defines.h>
#include <ds3d/common/hpp/datamap.hpp>
#include <ds3d/common/hpp/frame.hpp>
#include <ds3d/common/hpp/datarender.hpp>
#include <ds3d/common/impl/impl_datarender.h>

namespace ds3d { namespace impl {   namespace render {

using namespace ds3d;

class FakeSink : public BaseImplDataRender {

public:
    FakeSink() = default;
    ~FakeSink() override = default;

protected:
    ErrCode startImpl(const std::string& content, const std::string& path) override;
    ErrCode stopImpl() override;
    ErrCode flushImpl() override;

    ErrCode prerollImpl(GuardDataMap datamap) override;
    ErrCode renderImpl(GuardDataMap datamap, OnGuardDataCBImpl dataDoneCb) override;

private:
    // some private functions


    // some private members
    Config _config;

};

}}} // namespace ds3d::impl::render



#endif  // DS3D_DATARENDER_FAKESINK_IMPL_H