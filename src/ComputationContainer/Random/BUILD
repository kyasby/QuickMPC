cc_library(
    name = "random",
    srcs = [
        "Random.cpp"
    ],
    hdrs = [
        "Random.hpp",
    ],
    deps = [
        "//Random:csprng",
        "@libsodium//:sodium",
        "//FixedPoint:fixed_point",
        "//PrimeField:prime_field"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "csprng",
    srcs = [
        "Csprng.cpp"
    ],
    hdrs = [
        "Csprng.hpp",
    ],
    deps = [
        "@libsodium//:sodium",
        "//LogHeader:logger",
        "//Logging:log",
    ],
    visibility = ["//visibility:public"],
)
