add_executable(AVEL_function_evaluator
    function_evalutator.cpp
)

## Function evaluator

target_compile_options(AVEL_function_evaluator PRIVATE "-O3" "-ffp-contract=off" "" "") #"-fsanitize=undefined")
target_link_options(AVEL_function_evaluator PRIVATE) #"-fsanitize=undefined")

target_compile_features(AVEL_function_evaluator PRIVATE cxx_std_20)



# Baseline evaluator

add_executable(AVEL_baseline_generator
    baseline_generator/Baseline_generator.cpp
)

target_compile_options(AVEL_baseline_generator PRIVATE "-O3" "-ffp-contract=off") #"-fsanitize=undefined")
target_link_options(AVEL_baseline_generator PRIVATE) #"-fsanitize=undefined")

target_compile_features(AVEL_baseline_generator PRIVATE cxx_std_20)

target_link_libraries(AVEL_baseline_generator PRIVATE AVEL)
