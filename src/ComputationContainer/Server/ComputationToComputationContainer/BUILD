cc_library(
    name = "server",
    srcs = [
        "Server.cpp"
    ],
    hdrs = [
        "Server.hpp",
    ],
    deps = [
        "//ConfigParse:config_parse",
        "//Client/Helper:helper",
        "@Proto//ComputationToComputationContainer:computation_to_computation_cc_grpc",
        "//Client/ComputationToComputationContainer:client",
        "//LogHeader:logger",
        "//Logging:log",
    ],
    visibility = ["//visibility:public"],
)