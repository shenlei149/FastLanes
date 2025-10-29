// ────────────────────────────────────────────────────────
// |                      FastLanes                       |
// ────────────────────────────────────────────────────────
// src/include/fls/table/attribute.hpp
// ────────────────────────────────────────────────────────
#ifndef FLS_TABLE_ATTRIBUTE_HPP
#define FLS_TABLE_ATTRIBUTE_HPP

#include "fls/common/common.hpp"
#include "fls/common/double.hpp"
#include "fls/std/string.hpp"
#include "fls/table/rowgroup.hpp"

namespace fastlanes {
/*--------------------------------------------------------------------------------------------------------------------*/
template <typename PT>
class TypedCol;
enum class DataType : uint8_t;
/*--------------------------------------------------------------------------------------------------------------------*/

template <typename PT>
PT TypedNull() {
	static constexpr i08_pt I08_NULL   = 0;
	static constexpr i16_pt I16_NULL   = 0;
	static constexpr i32_pt I32_NULL   = 0;
	static constexpr i64_pt I64_NULL   = 0;
	static constexpr u08_pt U08_NULL   = 0;
	static constexpr u16_pt U16_NULL   = 0;
	static constexpr u32_pt U32_NULL   = 0;
	static constexpr u64_pt U64_NULL   = 0;
	static constexpr bol_pt BOOL_NULL  = false;
	static constexpr flt_pt FLOAT_NULL = 0.0;
	static constexpr dbl_pt DBL_NULL   = 0.0;
	static constexpr char   STR_NULL[] = "NULL";

	if constexpr (std::is_same_v<PT, i08_pt>) {
		return I08_NULL;
	} else if constexpr (std::is_same_v<PT, i16_pt>) {
		return I16_NULL;
	} else if constexpr (std::is_same_v<PT, i32_pt>) {
		return I32_NULL;
	} else if constexpr (std::is_same_v<PT, i64_pt>) {
		return I64_NULL;
	} else if constexpr (std::is_same_v<PT, u08_pt>) {
		return U08_NULL;
	} else if constexpr (std::is_same_v<PT, u16_pt>) {
		return U16_NULL;
	} else if constexpr (std::is_same_v<PT, u32_pt>) {
		return U32_NULL;
	} else if constexpr (std::is_same_v<PT, u64_pt>) {
		return U64_NULL;
	} else if constexpr (std::is_same_v<PT, str_pt>) {
		return STR_NULL;
	} else if constexpr (std::is_same_v<PT, bol_pt>) {
		return BOOL_NULL;
	} else if constexpr (std::is_same_v<PT, flt_pt>) {
		return FLOAT_NULL;
	} else if constexpr (std::is_same_v<PT, dbl_pt>) {
		return DBL_NULL;
	}

	FLS_UNREACHABLE();
}

/*--------------------------------------------------------------------------------------------------------------------*/

class Attribute {
public:
	Attribute() = delete;

public:
	template <typename PT>
	static void TypedIngestNumeric(TypedCol<PT>& typed_column, PT value, const ColumnDescriptorT& column_descriptor) {
		// init
		auto& n_nulls            = typed_column.m_stats.n_nulls;
		auto& min                = typed_column.m_stats.min;
		auto& max                = typed_column.m_stats.max;
		auto& is_double_castable = typed_column.m_stats.is_double_castable;

		if (typed_column.data.size() == 0) {
			typed_column.m_stats.last_seen_val = TypedNull<PT>();
		}

		// is_double_castable
		if constexpr (std::is_same_v<PT, dbl_pt>) {
			if (typed_column.data.empty()) {
				is_double_castable = true;
			}
		}

		// check if it  NULL
		// const bool is_null = IsNull(val_str);
		// if (is_null) {
		// 	n_nulls = n_nulls + 1;
		// }

		bool is_null = false;
		n_nulls      = 0;
		// ingest
		typed_column.null_map_arr.push_back(is_null);
		PT current_val = value;
		if (!is_null && column_descriptor.data_type == DataType::DECIMAL) {
			// if constexpr (std::is_same_v<PT, int64_t>) { // fix me
			// 	current_val                        = make_decimal(val_str,
			// column_descriptor.fix_me_decimal_type->scale); 	typed_column.m_stats.last_seen_val = current_val;
			// }
		} else if (!is_null) {
			// current_val                        = TypedCast<PT>(val_str, column_descriptor.data_type);
			// typed_column.m_stats.last_seen_val = current_val;
		} else {
			current_val = typed_column.m_stats.last_seen_val;
		}
		typed_column.data.push_back(current_val);

		min = std::min(min, current_val);
		max = std::max(max, current_val);

		// is_double_castable
		if constexpr (std::is_same_v<PT, dbl_pt>) {
			if (typed_column.data.empty()) {
				is_double_castable = true;
			}
			if (is_double_castable && !is_null && !Double::is_safely_castable_to_int64(current_val)) {
				is_double_castable = false;
			}
		}
	}

	template <typename PT>
	static void
	TypedIngestNumericVec(TypedCol<PT>& typed_column, const PT* values, const ColumnDescriptorT& column_descriptor) {
		// init
		auto& n_nulls            = typed_column.m_stats.n_nulls;
		auto& min                = typed_column.m_stats.min;
		auto& max                = typed_column.m_stats.max;
		auto& is_double_castable = typed_column.m_stats.is_double_castable;

		if (typed_column.data.size() == 0) {
			typed_column.m_stats.last_seen_val = TypedNull<PT>();
		}

		// is_double_castable
		if constexpr (std::is_same_v<PT, dbl_pt>) {
			if (typed_column.data.empty()) {
				is_double_castable = true;
			}
		}

		// check if it  NULL
		// const bool is_null = IsNull(val_str);
		// if (is_null) {
		// 	n_nulls = n_nulls + 1;
		// }

		bool is_null = false;
		n_nulls      = 0;
		// ingest
		for (size_t i = 0; i < CFG::VEC_SZ; i++) {
			typed_column.null_map_arr.push_back(is_null);
		}

		PT current_val = values[CFG::VEC_SZ - 1];
		if (!is_null && column_descriptor.data_type == DataType::DECIMAL) {
			// if constexpr (std::is_same_v<PT, int64_t>) { // fix me
			// 	current_val                        = make_decimal(val_str,
			// column_descriptor.fix_me_decimal_type->scale); 	typed_column.m_stats.last_seen_val = current_val;
			// }
		} else if (!is_null) {
			// current_val                        = TypedCast<PT>(val_str, column_descriptor.data_type);
			typed_column.m_stats.last_seen_val = current_val;
		} else {
			current_val = typed_column.m_stats.last_seen_val;
		}
		typed_column.data.insert(typed_column.data.end(), values, values + CFG::VEC_SZ);

		for (size_t i = 0; i < CFG::VEC_SZ; i++) {
			min = std::min(min, values[i]);
			max = std::max(max, values[i]);
		}

		// is_double_castable
		if constexpr (std::is_same_v<PT, dbl_pt>) {
			if (typed_column.data.empty()) {
				is_double_castable = true;
			}
			if (is_double_castable && !is_null && !Double::is_safely_castable_to_int64(current_val)) {
				is_double_castable = false;
			}
		}
	}

	static void   Ingest(col_pt& typed_column, const string& val_str, const ColumnDescriptorT& column_descriptor);
	static string ToStr(const col_pt& typed_column, n_t row_idx, const DataType& data_type);
};

} // namespace fastlanes
#endif // FLS_TABLE_ATTRIBUTE_HPP
