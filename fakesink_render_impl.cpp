#include "fakesink_render_impl.h"


namespace ds3d { namespace impl {   namespace render {

ErrCode FakeSink::startImpl(const std::string& content, const std::string& path)
{
    std::unique_lock<ImplMutex> locker(mutex());

    ErrCode code = config::CatchYamlCall([&, this]() { return parseConfig(content, path, _config); });
    DS3D_ERROR_RETURN(code, "parse datareder fake_sink config: %s failed", path.c_str());

    setInputCaps(_config.compConfig.gstInCaps);

    LOG_DEBUG("FakeSink datarender is started");
    return ErrCode::kGood;
}

ErrCode FakeSink::stopImpl()
{
    return ErrCode::kGood;
}

ErrCode FakeSink::flushImpl()
{

    return ErrCode::kGood;
}

ErrCode FakeSink::prerollImpl(GuardDataMap datamap)
{
    printf("FakeSink preroll\n");

    return ErrCode::kGood;
}

ErrCode FakeSink::renderImpl(GuardDataMap datamap, OnGuardDataCBImpl dataDoneCb)
{
    DS_ASSERT(datamap);

    printf("FakeSink received Data\n");
    DS3D_FAILED_RETURN(datamap.hasData(_config.inputDatamapKey), ErrCode::kConfig,
        "No radar frame found in datamap");

    FrameGuard radarFrame;
    
    DS3D_ERROR_RETURN(
            datamap.getGuardData(_config.inputDatamapKey, radarFrame),
            "No radar frame found in datamap from render");

    // further process the radar frame here
    // uint32_t _pointSizeBytes = 4*dataTypeBytes( DataType::kFp32);
    // guint8* x_point = new guint8[dataTypeBytes( DataType::kFp32)];
    // guint8* y_point = new guint8[dataTypeBytes( DataType::kFp32)];
    // guint8* z_point = new guint8[dataTypeBytes( DataType::kFp32)];
    // guint8* i_point = new guint8[dataTypeBytes( DataType::kFp32)];
    // memcpy(x_point, pointPtr + 100*_pointSizeBytes, dataTypeBytes( DataType::kFp32));
    // memcpy(y_point, pointPtr + 100*_pointSizeBytes + dataTypeBytes( DataType::kFp32), dataTypeBytes( DataType::kFp32));
    // memcpy(z_point, pointPtr + 100*_pointSizeBytes + 2*dataTypeBytes( DataType::kFp32), dataTypeBytes( DataType::kFp32));
    // memcpy(i_point, pointPtr + 100*_pointSizeBytes + 3*dataTypeBytes( DataType::kFp32), dataTypeBytes( DataType::kFp32));
    // float x_100 = *(float*)x_point;
    // float y_100 = *(float*)y_point;
    // float i_100 = *(float*)i_point;
    // float z_100 = *(float*)z_point;
    // printf("\n[MetaPreprocessFilter] RadarMap value at x[100]=%f, y[100]=%f, z[100]=%f, i[100]=%f\n", x_100, y_100, z_100, i_100);


    emitError(ErrCode::kGood, "data rendered");
    dataDoneCb(ErrCode::kGood, datamap);

    // must invoke this clear() function to release the datamap for all previous elements
    datamap.clear();
    
    return ErrCode::kGood;
}

}}}

using namespace ds3d;

DS3D_EXTERN_C_BEGIN
DS3D_EXPORT_API abiRefDataRender*
createFakeSink()
{
    return NewAbiRef<abiDataRender>(new impl::render::FakeSink);
}

DS3D_EXTERN_C_END