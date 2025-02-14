add_executable(motiontest
        test_aesni.cpp
        test_agmw.cpp
        test_arithmetic_provider.cpp
        test_base_ot.cpp
        test_beavy.cpp
        test_beavy_tensor.cpp
        test_bgmw.cpp
        test_bitmatrix.cpp
        test_bitvector.cpp
        test_bmr.cpp
        test_communication_layer.cpp
        test_conversions.cpp
        test_dummy_transport.cpp
        test_fixed_point.cpp
        test_gmw.cpp
        test_gmw_tensor.cpp
        test_half_gates.cpp
        test_integer_operations.cpp
        test_linear_algebra.cpp
        test_linalg_triple_provider.cpp
        test_misc.cpp
        test_motion_main.cpp
        test_mt.cpp
        test_ot.cpp
        test_ot_flavors.cpp
        test_reusable_future.cpp
        test_rng.cpp
        test_sb.cpp
        test_sp.cpp
        test_type_traits.cpp
        test_tcp_transport.cpp
        test_yao.cpp
        test_yao_tensor.cpp
        )

set_property(TARGET motiontest PROPERTY CXX_STANDARD 20)
set_property(TARGET motiontest PROPERTY CXX_STANDARD_REQUIRED On)

target_link_libraries(motiontest PRIVATE
        MOTION::motion
        OpenMP::OpenMP_CXX
        gtest
        )
