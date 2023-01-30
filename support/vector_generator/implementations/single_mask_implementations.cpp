@mask::primitive
std::uint8_t

@mask::mask(primitive)
    content(p & 0x1) {}

@mask::mask(bool)
    content(-b) {}

@mask::mask(const arr&)
content = -arr[0];

@mask::operator=(bool)
content = -b;

@mask::operator==(vec, vec)
return decay(lhs) == decay(rhs);

@mask::operator!=(vec, vec)
return decay(lhs) != decay(rhs);

@mask::operator&=(vec)
content &= decay(rhs);

@mask::operator|=(vec)
content |= decay(rhs);

@mask::operator^=(vec)
content ^= decay(rhs);

@mask::operator!()
return Vector_mask{static_cast<primitive>(content ^ 0x1)};

@count(mask)
return decay(m);

@any(mask)
return decay(m);

@all(mask)
return decay(m);

@none(mask)
return !decay(m);
