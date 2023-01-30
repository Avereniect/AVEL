@vector::primitive
$<SCALAR_NAME>

@vector::vector(mask)
content(-decay(m)) {}

@vector::vector(const arr&)
content = arr[0];

@vector::operator=(scalar)
content = x;

@vector::operator==(vec, vec)
return mask{decay(lhs) == decay(rhs)};

@vector::operator!=(vec, vec)
return mask{decay(lhs) != decay(rhs)};

@vector::operator<(vec, vec)
return mask{decay(lhs) < decay(rhs)};

@vector::operator<=(vec, vec)
return mask{decay(lhs) <= decay(rhs)};

@vector::operator>(vec, vec)
return mask{decay(lhs) > decay(rhs)};

@vector::operator>=(vec, vec)
return mask{decay(lhs) >= decay(rhs)};

@vector::operator+=(vec)
content += decay(rhs);

@vector::operator-=(vec)
content -= decay(rhs);

@vector::operator*=(vec)
content *= decay(rhs);

@vector::operator&=(vec)
content &= decay(rhs);

@vector::operator|=(vec)
content |= decay(rhs);

@vector::operator^=(vec)
content ^= decay(rhs);

@vector::operator<<=(long long)
content = (rhs >= $<SCALAR_WIDTH>) ? 0x00 : (content << rhs);

@vector::operator<<=(vec)
content = (decay(rhs) >= $<SCALAR_WIDTH>) ? 0x00 : (content << decay(rhs));

@vector::operator~()
return Vector{static_cast<primitive>(~content)};

@vector::operator mask()
return mask{static_cast<mask::primitive>(bool(content))};

@broadcast_mask(mask)
return $<VECTOR_ALIAS>{static_cast<$<VECTOR_ALIAS>::scalar>(-decay(m))};

@blend(mask, vec, vec)
return $<VECTOR_ALIAS>{blend(decay(m), decay(a), decay(b))};

@byteswap(vec)
return $<VECTOR_ALIAS>{byteswap(decay(v))};

@max(vec, vec)
return $<VECTOR_ALIAS>{max(decay(a), decay(b))};

@min(vec, vec)
return $<VECTOR_ALIAS>{min(decay(a), decay(b))};

@minmax(vec, vec)
return {min(a, b), max(a, b)};

@clamp(vec, vec, vec)
return $<VECTOR_ALIAS>{clamp(decay(x), decay(lo), decay(hi))};

@midpoint(vec, vec)
return $<VECTOR_ALIAS>{midpoint(decay(a), decay(b))};

@average(vec, vec)
return $<VECTOR_ALIAS>{average(decay(a), decay(b))};

@load(const scalar*, std::uint32_t)
if (n) {
    return $<VECTOR_ALIAS>{*ptr};
} else {
    return $<VECTOR_ALIAS>{0x00};
}

@load<width>(const scalar*)
return $<VECTOR_ALIAS>{*ptr};

@aligned_load(const scalar*, std::uint32_t)
if (n) {
    return $<VECTOR_ALIAS>{*ptr};
} else {
    return $<VECTOR_ALIAS>{0x00};
}

@aligned_load<width>(const scalar*)
return $<VECTOR_ALIAS>{*ptr};



@store<std::uint32_t>(scalar*, vec)
*ptr = decay(v);

@store<VECTOR_WIDTH>(scalar*, vec)
*ptr = decay(v);

@store(scalar*, vec, std::uint32_t)
if (n) {
    *ptr = decay(v);
}

@aligned_store<std::uint32_t>(scalar*, vec)
*ptr = decay(v);

@aligned_store<VECTOR_WIDTH>(scalar*, vec)
*ptr = decay(v);

@aligned_store(scalar*, vec, std::uint32_t)
if (n) {
    *ptr = decay(v);
}

@extract(vec)
return decay(v);

@insert(vec, scalar)
return $<VECTOR_ALIAS>{x};

@div(vec, vec)
div_type<$<VECTOR_ALIAS>> ret;
ret.quot = decay(x) / decay(y);
ret.rem  = decay(x) % decay(y);
return ret;

@popcount(vec)
return $<VECTOR_ALIAS>{popcount(decay(v))};

@countl_zero(vec)
return $<VECTOR_ALIAS>{countl_zero(decay(v))};

@countl_one(vec)
return $<VECTOR_ALIAS>{countl_one(decay(v))};

@countr_zero(vec)
return $<VECTOR_ALIAS>{countr_zero(decay(v))};

@countr_one(vec)
return $<VECTOR_ALIAS>{countr_one(decay(v))};

@bit_width(vec)
return $<VECTOR_ALIAS>{bit_width(decay(v))};

@bit_floor(vec)
return $<VECTOR_ALIAS>{bit_floor(decay(v))};

@bit_ceil(vec)
return $<VECTOR_ALIAS>{bit_ceil(decay(v))};

@has_single_bit(vec)
return $<MASK_ALIAS>{has_single_bit(decay(v))};
