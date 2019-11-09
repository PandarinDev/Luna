#pragma once

namespace luna {

    enum class SamplingStrategy {
        AA_0_SAMPLING,
        AA_2_SAMPLING,
        AA_4_SAMPLING
    };

    namespace Sampler {

        template<SamplingStrategy Strategy>
        constexpr int getTotalNumberOfIterations();
        
        template<SamplingStrategy Strategy>
        constexpr float getOffset(int iteration);

    }

    template<>
    constexpr int Sampler::getTotalNumberOfIterations<SamplingStrategy::AA_0_SAMPLING>() {
        return 1;
    }

    template<>
    constexpr float Sampler::getOffset<SamplingStrategy::AA_0_SAMPLING>(int iteration) {
        return 0.0f;
    }

    template<>
    constexpr int Sampler::getTotalNumberOfIterations<SamplingStrategy::AA_2_SAMPLING>() {
        return 2;
    }

    template<>
    constexpr float Sampler::getOffset<SamplingStrategy::AA_2_SAMPLING>(int iteration) {
        return (iteration + 1) * 0.25f;
    }

    template<>
    constexpr int Sampler::getTotalNumberOfIterations<SamplingStrategy::AA_4_SAMPLING>() {
        return 4;
    }

    template<>
    constexpr float Sampler::getOffset<SamplingStrategy::AA_4_SAMPLING>(int iteration) {
        return (iteration + 1) * 0.16666f;
    }

}