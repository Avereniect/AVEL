@vector::operator>>=(long long)
content = (rhs >= $<SCALAR_WIDTH>) ? (content >> ($<SCALAR_WIDTH> - 1)) : (content >> rhs);

@vector::operator>>=(vec)
content = (decay(rhs) >= $<SCALAR_WIDTH>) ? (content >> ($<SCALAR_WIDTH> - 1)) : (content >> decay(rhs));


@operator-(uvec)
return -$<VECTOR_ALIAS>{v};

@negate(mask, vec)
if (decay(m)) {
    return -v;
} else {
    return v;
}

@abs(vec)
if (decay(v) < 0) {
    return -v;
} else {
    return v;
}

@neg_abs(vec)
if (decay(v) < 0) {
    return v;
} else {
    return -v;
}

@neg_abs(uvec)
return neg_abs($<VECTOR_ALIAS>{v});



@gather(scalar*, vec)
return $<VECTOR_ALIAS>{ptr[decay(indices)]};

@gather<width>(scalar*, vec)
return $<VECTOR_ALIAS>{ptr[decay(indices)]};

@gather(scalar*, vec, std::uint32_t n)
if (n) {
    return $<VECTOR_ALIAS>{ptr[decay(indices)]};
} else {
    return $<VECTOR_ALIAS>{0x00};
}



@gather(uscalar*, uvec)
return $<VECTOR_ALIAS>{ptr[decay(indices)]};

@gather<width>(uscalar*, uvec)
return $<VECTOR_ALIAS>{ptr[decay(indices)]};

@gather(uscalar*, uvec, std::uint32_t n)
if (n) {
    return $<VECTOR_ALIAS>{ptr[decay(indices)]};
} else {
    return $<VECTOR_ALIAS>{0x00};
}



@scatter(const scalar*, vec, vec)
ptr[decay(indices)] = decay(v);

@scatter<width>(const scalar*, vec, vec)
ptr[decay(indices)] = decay(v);

@scatter(const scalar*, vec, vec, std::uint32_t)
if (n) {
    ptr[decay(indices)] = decay(v);
}



@scatter(const uscalar*, uvec, vec)
ptr[decay(indices)] = decay(v);

@scatter<width>(const uscalar*, uvec, vec)
ptr[decay(indices)] = decay(v);

@scatter(const uscalar*, uvec, vec, std::uint32_t)
if (n) {
    ptr[decay(indices)] = decay(v);
}
