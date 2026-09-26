#pragma once

template<typename Uniform, typename In, typename Out>
class Shader {
    public:
    Shader() = default;
    virtual ~Shader() = default;
    Out operator()(const In& input, const Uniform& uniform) const {
        return this->process(input, uniform);
    }
    private:
    virtual Out process(const In& input, const Uniform& uniform) const = 0;
};