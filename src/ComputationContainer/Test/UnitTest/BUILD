cc_test(
    name = "MathTest",
    srcs = [
        "MathTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "@com_github_grpc_grpc//:grpc++",
        "//Math:math",
        "//Share:share",
        "//ConfigParse:config_parse",
        "//Server/ComputationToComputationContainer:server",
        "//LogHeader:logger",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "ShareTest",
    srcs = [
        "ShareTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "@com_github_grpc_grpc//:grpc++",
        "//Share:share",
        "//ConfigParse:config_parse",
        "//Server/ComputationToComputationContainer:server",
        "//LogHeader:logger",
    ],
)

cc_test(
    name = "RandomTest",
    srcs = [
        "RandomTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "//Random:random",
        "//FixedPoint:fixed_point",
        "//PrimeField:prime_field",
    ],
)

cc_test(
    name = "CtoCTest",
    srcs = [
        "CtoC_test.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "//Server/ComputationToComputationContainer:server",
        "//Client/Helper:helper",
        "//ConfigParse:config_parse",
        "@Proto//ComputationToComputationContainer:computation_to_computation_cc_grpc",
        "//Share:address",
        "//LogHeader:logger"
    ],
    linkopts = ["-pthread"],
)

cc_test(
    name = "FixedPointTest",
    srcs = [
        "FixedPointTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "//FixedPoint:fixed_point",
        "//FixedPoint:math_lib"
    ],
)

cc_test(
    name = "PrimeFieldTest",
    srcs = [
        "PrimeFieldTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "//PrimeField:prime_field"
    ],
)

cc_test(
    name = "TransactionQueueTest",
    srcs = [
        "TransactionQueueTest.cpp"
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
       "//TransactionQueue:transaction_queue"
     ],
)

cc_test(
    name = "CsprngTest",
    srcs = [
        "CsprngTest.cpp"
    ],
    copts = ["-Iexternal/gtest/include",],
    deps = [
        "@gtest//:main",
        "//Random:csprng",
        "//LogHeader:logger",
    ],
)

cc_test(
    name = "LogTest",
    srcs = [
        "LogTest.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:main",
        "//Logging:log",
    ],
    visibility = ["//visibility:public"],
)
