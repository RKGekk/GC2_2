#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>

#include <DirectXMath.h>

std::string w2s(const std::wstring& var);
std::wstring s2w(const std::string& var);

std::string fixedfloat(float value, int precision);
std::ostream& operator<<(std::ostream& os, const DirectX::XMFLOAT2& v);
std::ostream& operator<<(std::ostream& os, const DirectX::XMFLOAT3& v);
std::ostream& operator<<(std::ostream& os, DirectX::FXMVECTOR v);
std::ostream& operator<<(std::ostream& os, DirectX::XMFLOAT4X4 m);
std::ostream& operator<<(std::ostream& os, DirectX::CXMMATRIX m);

template<typename C, typename T>
auto insert_in_container(C& c, T&& t) -> decltype(c.push_back(std::forward<T>(t)), void()) {
    c.push_back(std::forward<T>(t));
}
template<typename C, typename T>
auto insert_in_container(C& c, T&& t) -> decltype(c.insert(std::forward<T>(t)), void()) {
    c.insert(std::forward<T>(t));
}
template<typename Container>
Container splitR(const std::string& input, const std::string& delims) {
    Container out;
    size_t delims_len = delims.size();
    auto begIdx = 0u;
    auto endIdx = input.find(delims, begIdx);
    if (endIdx == std::string::npos && input.size() != 0u) {
        insert_in_container(out, input);
    }
    else {
        size_t w = 0;
        while (endIdx != std::string::npos) {
            w = endIdx - begIdx;
            if (w != 0) insert_in_container(out, input.substr(begIdx, w));
            begIdx = endIdx + delims_len;
            endIdx = input.find(delims, begIdx);
        }
        w = input.length() - begIdx;
        if (w != 0) insert_in_container(out, input.substr(begIdx, w));
    }
    return out;
}