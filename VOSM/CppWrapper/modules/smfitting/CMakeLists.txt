include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../common/include" "${CMAKE_CURRENT_SOURCE_DIR}/../cvcommon/include"
"${CMAKE_CURRENT_SOURCE_DIR}/../comalgs/include" "${CMAKE_CURRENT_SOURCE_DIR}/../ensembletraining/include"
"${CMAKE_CURRENT_SOURCE_DIR}/../integraltransform/include" "${CMAKE_CURRENT_SOURCE_DIR}/../featureextraction/include"
"${CMAKE_CURRENT_SOURCE_DIR}/../smbuilding/include")
set(deps opencv_core opencv_imgproc opencv_highgui)
define_vosm_module(smfitting vosm_common vosm_cvcommon vosm_featureextraction vosm_integraltransform vosm_smbuilding ${deps})
