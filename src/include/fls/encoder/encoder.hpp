// ────────────────────────────────────────────────────────
// |                      FastLanes                       |
// ────────────────────────────────────────────────────────
// src/include/fls/encoder/encoder.hpp
// ────────────────────────────────────────────────────────
#ifndef FLS_ENCODER_ENCODER_HPP
#define FLS_ENCODER_ENCODER_HPP

#include "fls/std/filesystem.hpp"

namespace fastlanes {
/*--------------------------------------------------------------------------------------------------------------------*/
class Connection;
class Buf;
/*--------------------------------------------------------------------------------------------------------------------*/

class Encoder {
public:
	static void     encode(const Connection& connection, const path& file_path);
	static uint32_t encode(const Connection& connection, char* dst, uint32_t length);
};
} // namespace fastlanes

#endif // FLS_ENCODER_ENCODER_HPP
