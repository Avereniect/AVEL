@vector::operator>>=(long long)
content = (rhs >= $<SCALAR_WIDTH>) ? 0x00 : (content >> rhs);

@vector::operator>>=(vec)
content = (decay(rhs) >= $<SCALAR_WIDTH>) ? 0x00 : (content >> decay(rhs));

@bit_shift_left(vec)
return $<VECTOR_ALIAS>{decay(v) << S};

@BIT_SHIFT_LEFT_SPECIALIZATIONS

@bit_shift_right(vec)
return $<VECTOR_ALIAS>{decay(v) >> S};

@rotl(vec)
return $<VECTOR_ALIAS>{rotl(decay(v), S)};

@rotl(vec, long long)
return $<VECTOR_ALIAS>{rotl(decay(v), s)};

@rotl(vec, vec)
return $<VECTOR_ALIAS>{rotl(decay(v), decay(s))};

@rotr(vec)
return $<VECTOR_ALIAS>{rotr(decay(v), S)};

@rotr(vec, long long)
return $<VECTOR_ALIAS>{rotr(decay(v), s)};

@rotr(vec, vec)
return $<VECTOR_ALIAS>{rotr(decay(v), decay(s))};
